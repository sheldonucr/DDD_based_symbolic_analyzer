/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: buildmna.cc,v $
 *    $Revision: 1.10 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/* buildmna.cpp:
 *   Contains analysis routines to build the MNA matrix for
 *   a circuit.
 *
 */

/* INCLUDES: */
#include <unistd.h>
#include "circuits.h"
#include "mna_form.h"
#include "mxexp.h"
#include "ddd_base.h"
#include "ckt_call.h"
#include "acan.h"
#include "rclcompx.h"

#define _DEBUG

/* DEFINES and TYPEDEFS: */

//extern FLabelMap *theFLabMap;

/*****************************************************
** Member function defintion of MNAElement
******************************************************/

MNAElement::MNAElement(Node *_node, Node *_pivot)
{
    node = _node;
    pivot = _pivot;
    flag = 0;
    deleted = 0;
    freq = -1e20;
    value = 0.0;
    blist = new BranchList;
    clist = NULL;
    freq  = 1.0;
    next = NULL;
    clist_sname = NULL;
	m_is_symbolic = false;
	
}

MNAElement::~MNAElement()
{
    delete blist;
    if(clist)
	{
		CompElemList *aux, *paux;
		aux = paux = clist;
		while(aux)
		{
			paux = aux->Next();
			delete aux;
			aux = paux;
		}
    }
    //if(next) delete next;
}

/* check to see if the element is invalid one ? */
boolean
MNAElement::check_valid()
{
    /*
    cout<<"blist: "<<blist->get_num()<<endl;
    cout<<"clist: "<<(clist?"NONULL":"NULL")<<endl;
    */
    if(blist->get_num() == 0 && !clist)
	{
        //cout<<"invliad MNA element"<<endl;
        return 0;
    }
    else
	{
        return 1;
	}
}

complex
MNAElement::elem_value
( 
	double _freq 
)
{
    complex Czero = 0.0;
    if(_freq == freq )
	{
        return value;
	}
    else
	{
        freq = _freq;
        if(!clist)
		{// is simple element
			value = ((blist->get_cvalue() ^ freq) + Czero);
		}
        else
		{// is a composite element
            CompElemList *claux = clist;
            complex ccvalue = 0.0;
            for(; claux; claux = claux->Next())
			{
                ccvalue += claux->evaluate(freq);
			}
            //cout <<"cvalue:"<<ccvalue<<endl;
            //cout <<"bvalue:"<<(blist->get_cvalue() ^ freq)<<endl;
            value = (blist->get_cvalue() ^ freq) - ccvalue;
        }
    }

    return value;
}

void    
MNAElement::add_clist
(
	CompElemList *_clist
)
{
    if(!_clist)
	{
        return;
	}
    if(!clist)
	{
        clist = _clist;
	}
    else
	{
        cout <<"more than one clist!!"<<endl;
		_clist->Next(clist);
        clist = _clist;
    }
}

void
MNAElement::copy_clist
( 
	CompElemList *_clist
)
{
    CompElemList *newList;
    if(!_clist)
	{		
        return;
	}
    if(!clist)
	{
        clist = _clist->copy_list();
	}
    else
	{
        cout <<"more than one clist!!"<<endl;
		newList =  _clist->copy_list();
		newList->Next(clist);
        clist = newList;
    }
}

/*
**  find the fcoefficient DDD index given complex ddd index and 
**  branch name
*/
int
MNAElement::find_findex
( 
	char *name, 
	int cind 
)
{
    int bindex;
    if(!name)
	{
        error_mesg(INT_ERROR,"No branch name given");
        return -1;
    }
    bindex = blist->get_branch_index(name);
    if(bindex == -1)
	{
        sprintf(_buf,"%s : is not found in the branchlist",name);
        error_mesg(FAT_ERROR,_buf);
        exit(-1);
        return -1;
    }

	DummBranch * dbch = blist->get_dummbranch(bindex);
	assert(dbch);
	int findex = theFLabMap->get_flabel(dbch);

	
    if(findex == -1)
	{
		// means that item does not exist in the theFLabMap. So we use
		// the initial map function to find the findex.
		findex = theFLabMap->ddd2fcoeff(cind, bindex);
		
	}
	

	return findex;
	
}

//////////////////////////////////////////////
//member function definition of Label2MNA
/////////////////////////////////////////////

Label2MNA::Label2MNA(int thesize)
{
	m_size = thesize;	
}

Label2MNA::~Label2MNA()
{
}


void 
Label2MNA::set_elem
(
	int _index, 
	MNAElement *_mna_elem
) 
{ 							
	label2ElemMap[_index]= _mna_elem;		
}

MNAElement * 
Label2MNA::get_elem
(
	int _index
) 
{ 
	Int2ElemMap::const_iterator i = label2ElemMap . find (_index);
	return (i !=  label2ElemMap.end ()) ?  (*i) . second  :  NULL;		
}
    
void 
Label2MNA::set_row
( 
	int _index , 
	int _row
)
{ 
	label2Row[_index]= _row;
}

int 
Label2MNA::get_row
(
	int _index
) 
{ 
	Int2IntMap::const_iterator i = label2Row . find (_index);
	return (i !=  label2Row.end ()) ?  (*i) . second  :  -1;	
}

void 
Label2MNA::set_col
( 
	int _index , 
	int _col
)
{ 
	label2Col[_index] = _col;
}

int 
Label2MNA::get_col
(
	int _index
) 
{ 
	Int2IntMap::const_iterator i = label2Col . find (_index);
	return (i !=  label2Col.end ()) ?  (*i) . second  :  -1;	
}

void
Label2MNA::swap_two_labels
(
	int label1,
	int label2
)
{

	// integrity check
	assert(label1 <= m_size);
	assert(label2 <= m_size);
	
	cout <<"\n**** Finally swap the complex  index maps ..."<< endl;

	cout <<" (" <<label1 <<",";	
	label2ElemMap[label1]->get_blist()->print_branch(cout);
	cout <<"); ";
	
	cout <<"(" <<label2 <<",";
	label2ElemMap[label2]->get_blist()->print_branch(cout);
	cout <<")";


	// first the label to MNA element map
	MNAElement *tmp = label2ElemMap[label1];
	label2ElemMap[label1] = label2ElemMap[label2];
	label2ElemMap[label2] = tmp;

	cout <<" -> ";

	cout <<" (" <<label1 <<",";	
	label2ElemMap[label1]->get_blist()->print_branch(cout);
	cout <<"); ";
	
	cout <<"(" <<label2 <<",";
	label2ElemMap[label2]->get_blist()->print_branch(cout);
	cout <<")";	
	cout << endl;

	// then the label to row map
	int tmp_row = label2Row[label1];
	label2Row[label1] = label2Row[label2];
	label2Row[label2] = tmp_row;
	
	// finall the label to column map
	int tmp_col = label2Col[label1];
	label2Col[label1] = label2Col[label2];
	label2Col[label2] = tmp_col;

    // we should also change the indices cached in the global index
    // matrix
	IndexMatrix *theIndexMax = theCkt->get_mna_matrix()->get_imatrix();	
	theIndexMax->set_label(label2Row[label1],label2Col[label1], label1);
	theIndexMax->set_label(label2Row[label2],label2Col[label2], label2);
	
}

//////////////////////////////////////////////
//member function definition of MNAMatrix
/////////////////////////////////////////////

MNAMatrix::MNAMatrix
(
	Circuit *_ckt, 
	DDDmanager *_ddd
)
{
    ckt        = _ckt;
    rowlist     = NULL;
    cur_row     = NULL;
    num_elem     = 0;
    size        = 0;
    index_mat     = NULL;
    label2elem     = NULL;
    ddd_mgr        = _ddd;
    init_label    = 0;
}

MNAMatrix::~MNAMatrix()
{
    if(rowlist)
	{
        MNARow *raux, *praux;
		raux = praux = rowlist;
		while(raux)
		{
			praux = raux->Next();
			delete raux;
			raux = praux;
		}
    }
    if(index_mat)
	{
        delete index_mat;
	}
    if(label2elem)
	{
        delete label2elem;
	}
}


/*----------------------------------------------------
Spec: build the MNA element related to the subckt call.
    There are two cases:
    (1) if the element alreays exist, make the composite
        expression for the element.
    (2) if the elemenet does not exist, first make it.
        then build the composite expression for the element.
-----------------------------------------------------*/
void
MNAMatrix::process_subckt_call
( 
	CktCall *_call 
)
{
    if(!_call)
	{
        return;
	}
    int     row, col, fnew;
    NList    *naux1, *naux2;
    MNARow    *raux;
    Node    *Nnode;
    MNAElement    *Eelem;

    // first build the system determinent
    _call->expand_subckt();

    // go through all the combinations of interface nodes
    naux1 = _call->interface; 
    for(; naux1; naux1 = naux1->Next())
	{
        row = naux1->get_node();
        if(!row)
		{// ignore the ground node
            continue;
		}
        raux = find_row_real( row );
#ifdef _DEBUG
        if(!raux)
		{
            sprintf(_buf,"Node: %d has no MNA row.",row );
            error_mesg(INT_ERROR,_buf);
        }
#endif
        for(naux2 = _call->interface; naux2; naux2 = naux2->Next())
		{
            col = naux2->get_node();
            if(!col)
			{// ignore the ground node
                continue;
			}
            Nnode = ckt->query_node(col);
#ifdef _DEBUG
            if(!Nnode)
			{
                sprintf(_buf,"Node: %d has not been created.",col);
                error_mesg(INT_ERROR,_buf);
            }
#endif
            // build the composite symbolic expressions in subckt
            cout <<"build composite expression for ("
				 <<row<<","
				 <<col<<")"
				 <<endl;
            // find or create the element if does not existing
            fnew = 0;
            Eelem = raux->query_element(Nnode);
            if(!Eelem)
			{
                Eelem = raux->create_element(Nnode);
                fnew = 1;
            }

            Eelem->add_clist(_call->build_expression(row, col));

            /* add the MNA element's contribution in subckt into
               its parent MNA entry.
			   The a^BB_u,v in our TCAD'paper (11)
			*/
			MNAElement *subcktElem = _call->find_element(row, col);
            if(subcktElem)
			{
                Eelem->blist->merge_blist( subcktElem->get_blist() );
				Eelem->copy_clist(subcktElem->get_clist());
			}

            if(fnew && Eelem->check_valid())
			{
                raux->add_element(Eelem);
			}
        }
    }
}

/*----------------------------------------------------
Spec: find the specific MNARow, 
      given the row's pivot node's real number(local).
-----------------------------------------------------*/
MNARow *
MNAMatrix::find_row_real
( 
	int _node 
)
{
    MNARow    *raux = rowlist;
    if(!raux)
	{
        return NULL;
	}
    for(; raux; raux = raux->Next())
	{
        if(raux->pivot->get_real_num() == _node)
		{
            return raux;
		}
    }

    return NULL;
}

/*----------------------------------------------------
Spec: find all the a x delta x b forms in the 
subckt for given MNA element in parent's ckt.
input: _row and _col now is assumed as the real number of
subckt.
-----------------------------------------------------*/
CompElemList  *
MNAMatrix::build_comp_expr
(
	int _row, 
	int _col
)
{
	int k1, k2;
	SubCircuit    *ckt_aux = (SubCircuit *)ckt;
	MNARow    *raux = rowlist;
	Cofactor *cof_aux;

	CompElemList *cur_comp_list = new CompElemList((SubCircuit *)ckt);

	if(!rowlist)
	{
		error_mesg(INT_ERROR,"No MNA matrix exist.\n");
		return NULL;
	}

	MNARow    *raux1 = find_row_real(_row);
#ifdef _DEBUG
	if(!raux1)
	{
		sprintf(_buf,"can't find row: %d.",_row);
		error_mesg(INT_ERROR,_buf);
		return NULL;
	}
#endif
	// first, we find the a(k2,_col)
	for(;raux;raux = raux->next)
	{
		// skip boundary node
		if(raux->get_pivot()->get_type() == nBound)
		{
			continue;
		}
		k2 = raux->get_pivot()->get_index();
		Node *Nnode = ckt->query_node(_col);
#ifdef _DEBUG
		if(!Nnode)
		{
			sprintf(_buf,"can't find node: %d in ckt node list.",_col);
			error_mesg(INT_ERROR,_buf);
			return NULL;
		}
#endif
		MNAElement *Eelem_r = raux->query_element(Nnode);
		if(!Eelem_r)
		{// elemet a(k2,col) does not exist.
			continue;
		}
		// if exist, find a(row, k1)
		MNAElement    *Eelem_l = raux1->elem_list;
		for(; Eelem_l; Eelem_l = Eelem_l->next)
		{
			// skip boundary element
			if(Eelem_l->get_node()->get_type() == nBound)
			{
				continue;
			}
			k1 = Eelem_l->get_node()->get_index();
			cof_aux = ckt_aux->query_cofactor(k2,k1);
			if(!cof_aux)
			{
				// cout << ckt_aux->get_name() << "; " << k2 << " " << k1 << endl;
				DDDnode *Dddd = ckt->ac_mgr->build_cofactor_ddd(k2,k1);
				if(!Dddd)
				{
					print_warn("NULL Cofactor DDD, ignored.");
					continue;
				}
				cof_aux = new Cofactor(ddd_mgr,Dddd, PLUS, k2, k1);
				ckt_aux->add_cofactor(cof_aux);
			}
			CompElem *Celem = new CompElem(Eelem_l, Eelem_r, cof_aux);
			cur_comp_list->add_elem(Celem);
		}
	}
	if(cur_comp_list->get_num())
	{
		return cur_comp_list;
	}
	else
	{
		cout <<"NULL return!"<<endl;
		return NULL;
	}
}

/*----------------------------------------------------
Spec: find the specified mna element.
input: _row and _col now is assumed as the real number of
subckt.
-----------------------------------------------------*/
MNAElement  *
MNAMatrix::get_mna_element_real
( 
	int _row, 
	int _col
)
{
    Node    *Nnode;

    if(!rowlist)
	{
        error_mesg(INT_ERROR,"No MNA matrix exist.\n");
        return NULL;
    }
    MNARow    *raux = find_row_real(_row);
#ifdef _DEBUG
    if(!raux)
	{
        sprintf(_buf,"can't find row: %d.",_row);
        error_mesg(INT_ERROR,_buf);
        return NULL;
    }
#endif
    //int node = raux->get_pivot()->get_real_num();
    Nnode = ckt->query_node(_col);
    MNAElement *Eelem = raux->query_element(Nnode);
    if(!Eelem)
	{
        return NULL;
	}
    else
	{
        return Eelem;
	}
}
    

/*----------------------------------------------------
Spec: find the specified mna element.
input: _row and _col now is assumed as the internal row
      and column indice.
-----------------------------------------------------*/
MNAElement  *
MNAMatrix::get_mna_element
( 
	int _row, 
	int _col
)
{
    if(!rowlist)
	{
        error_mesg(INT_ERROR,"No MNA matrix exist.\n");
        return NULL;
    }

	MNARow    *raux = rowlist;   
	MNARow    *row = NULL;

	for(; raux; raux = raux->Next())
	{
        if(raux->pivot->get_index() == _row)
		{
            row = raux;
		}
    }

    assert(row);

    MNAElement *Eelem = row->query_element_by_index(_col);
    if(!Eelem)
	{
        return NULL;
	}
    else
	{
        return Eelem;
	}
}

/*----------------------------------------------------
Spec: Create the index-to-MNA element mapping function.  In case of
hierarchical analysis, each non-boundary element also needs a unique
label throughout whole circuit hierarchy. While, boundary elements
don't have to be assigned a label. Since labels are consecutive within
each sub-circuit. So we need to remember the initial label value for
each sub-circuit to perform the mapping.  Now, we use different DDD
manager for each subckt, so the initial label of each ckt can be 0.
-----------------------------------------------------*/
void 
MNAMatrix::create_mapping()
{
    MNAElement     *elem_aux;
    MNARow         *row_aux;
    int            index, row, col;

    if(!rowlist || !num_elem )
	{
        print_error(INT_ERROR,"Invalid rowlist or num_elem");
        return;
    }
    
    //label2elem = new Label2MNA(num_elem, init_label);
	label2elem = new Label2MNA(num_elem);

    for(row_aux = rowlist; row_aux; row_aux = row_aux->Next())
	{
        if(ckt->which_level() == cktSub &&
           row_aux->get_pivot()->get_type() == nBound)
		{
               continue;
		}
        for(elem_aux = row_aux->elem_list; elem_aux;
                        elem_aux = elem_aux->Next())
		{
            if(ckt->which_level() == cktSub &&
               elem_aux->node->get_type() == nBound )
			{
                continue;
			}
            row = row_aux->pivot->get_index();
            col = elem_aux->node->get_index(); 
            index = index_mat->get_label(row,col);
            if(!index)
			{
                print_error(FAT_ERROR,"Invalid index");
                sprintf(_buf,"row: %d col: %d\n",row, col);
                print_mesg(_buf);
                exit(-1);
            }
            label2elem->set_elem(index,elem_aux);
            label2elem->set_row(index,row);
            label2elem->set_col(index,col);
        }
    }

#ifdef _DEBUG
    for(int i = init_label+1; i <= num_elem + init_label; i++ )
	{
        if(!label2elem->get_elem(i))
		{
            sprintf(_buf,"index: %d has not been assigned",i);
            error_mesg(INT_ERROR,_buf);
        }
    }
#endif

    /* set DDD manager */
    ddd_mgr->set_label2elem(label2elem);
}


/*----------------------------------------------------
Spec: begin a new rows in MNA. Note that rows in MNA
      are increasingly sorted according to the "num"
-----------------------------------------------------*/
MNARow *
MNAMatrix::new_mna_row
(
	Node *node
)
{
    if(!node)
	{
        print_error(INT_ERROR,"node = NULL!");
        return NULL;
    }

    if(!rowlist)
	{
        rowlist = cur_row = new MNARow(node,this,NULL);
	}
    else
	{
        MNARow *row_aux, *row_prev;
        row_aux = row_prev = rowlist;

        // search to find an appropriate position for the new row
        while(row_aux && row_aux->pivot->get_num() < node->get_num())
		{
                    row_prev = row_aux;
                    row_aux = row_aux->Next();
        }
        MNARow *next_row = row_prev->Next();
        cur_row = new MNARow(node,this,next_row);
        row_prev->Next(cur_row);
    }
    size++;
    return cur_row;
}    

/* 
**    calculate all the rlc value assocaied with all the 
**    element (include diagonal elements )in the MNA
*/
void
MNAMatrix::calc_value()
{
    MNARow *row_aux = rowlist;

    for(; row_aux; row_aux = row_aux->Next())
	{
        row_aux->calc_elems_value(); 
	}
}

void
MNAMatrix::calc_value(PolyType ptype)
{
    MNARow *row_aux = rowlist;

    for(; row_aux; row_aux = row_aux->Next())
	{
        row_aux->calc_elems_value(ptype); 
	}
}

/*----------------------------------------------------
Spec: given the real_num of node, return the internal
    index number. On failure, -1 will be returned.
-----------------------------------------------------*/
int 
MNAMatrix::real2label
( 
	int real 
)
{
    MNARow *row_aux = rowlist;
    Node *node_aux;

    for(; row_aux; row_aux = row_aux->Next())
	{
        node_aux = row_aux->pivot;
        if(node_aux->get_real_num() == real)
		{
            return node_aux->get_index();
		}
    }
    return -1;
}

DDDnode *
MNAMatrix::build_sys_ddd()
{
    if(ckt->which_level() == cktTop)
	{
        index_mat->reset_rowcol();
	}

    index_mat->initial_rowcol();
    sys_det = index_mat->matrix_recursive_expR(-1, -1);
    if(!sys_det)
	{
        error_mesg(FAT_ERROR,"NULL system DDD.");
        return NULL;
    }
#ifndef COLLECT_COEFF_DATA 
    if(ckt->which_level() != cktTop)
	{
        ddd_mgr->Keep(sys_det);
	}
#endif
    index_mat->ddd_statistic(sys_det);

    // initialized the system ddd for numerical evaluation
	HasherTree *prec_ht = new HasherTree(FALSE);
    ddd_mgr->PreCalculationR(sys_det, prec_ht);
	delete prec_ht;	
    
	return sys_det;
}

complex 
MNAMatrix::evaluate_sys_det
( 
	double freq, 
	HasherTree *ht
)
{
    return ddd_mgr->GetDetermValueR(sys_det,freq, ht);
}
        
/*
**    Query the label of a nonzero element in DDD
**    Note, the _row and _col here is refered to the
**    internal node's index
*/
int
MNAMatrix::get_label
(
	int _row,
	int  _col
)
{
    if(index_mat)
	{
        return index_mat->get_label(_row,_col);
	}
    else
	{
        error_mesg(INT_ERROR,"No index matrix exists");
        return 0;
    }
}

void    
MNAMatrix::delete_element
( 
	int label 
)
{
    MNAElement *elem;
    int row, col;
    if(!index_mat)
	{
        error_mesg(INT_ERROR,"No index matrix exists");
        return;
    }
    elem = label2elem->get_elem(label);
    row = elem->get_row();
    col = elem->get_col();
    index_mat->delete_element(row,col);
}

void
MNAMatrix::print_mna()
{
    if(!rowlist)
	{
        error_mesg(INT_ERROR,"No MNA matrix created, printing abort!");
        return;
    }

    MNARow    *row_aux = rowlist;
    MNAElement *elem_aux;
    rcl_complex cvalue;
    cout << "row(real, index)"<<endl;
    cout << "---------------------"<<endl;
    for(; row_aux; row_aux = row_aux->Next())
	{
        cout << "row("<<row_aux->pivot->get_real_num() <<","
                    <<row_aux->pivot->get_index() <<")  ";
        elem_aux = row_aux->elem_list;

        for(; elem_aux; elem_aux = elem_aux->Next())
		{
            /*
            cvalue = elem_aux->get_cvalue();
            cout << "("<<row_aux->pivot->get_index()<<"," <<
                elem_aux->node->get_index()<<")"<<cvalue<<";";
            */
            cout << "("<<row_aux->pivot->get_index()<<"," <<
                elem_aux->node->get_index()<<")";
                elem_aux->blist->print_branch(cout);
            cout <<";";
        }
        cout <<endl;
    }
    cout << "---------------------"<<endl<<endl;
}

void
MNAMatrix::print_mna(PolyType ptype)
{
    if(!rowlist)
	{
        error_mesg(INT_ERROR,"No MNA matrix created, printing abort!");
        return;
    }

    MNARow    *row_aux = rowlist;
    MNAElement *elem_aux;
    rcl_complex cvalue;

    cout << "row(real, index)"<<endl;
    cout << "---------------------"<<endl;

    for(; row_aux; row_aux = row_aux->Next())
	{
        cout << "row("<<row_aux->pivot->get_real_num() <<","
                    <<row_aux->pivot->get_index() <<")  ";
        elem_aux = row_aux->elem_list;
        for(; elem_aux; elem_aux = elem_aux->Next())
		{
            cvalue = elem_aux->get_cvalue(ptype);
            cout << "("<<row_aux->pivot->get_index()
				 <<"," <<elem_aux->node->get_index()<<")"
				 <<cvalue<<";";
            
			cout << "("<<row_aux->pivot->get_index()<<"," 
				 <<elem_aux->node->get_index()<<")";
                
			elem_aux->blist->print_branch(cout,ptype);
			cout <<";";
        }
        cout <<endl;
    }

    cout << "---------------------"<<endl<<endl;
}

void
MNAMatrix::print_bool()
{
    index_mat->print_bool();
}

/*------------------------------------------------
calculate the maximum number of branches among
all the MNA element.
-------------------------------------------------*/
int
MNAMatrix::max_branch_num()
{
    MNARow    *row_aux = rowlist;
    MNAElement    *elem_aux;
    int        max_branch = 0;
    for(; row_aux; row_aux = row_aux->Next())
	{
        elem_aux = row_aux->elem_list;
        for(; elem_aux; elem_aux = elem_aux->Next())
		{
            if(elem_aux->blist->get_num() > max_branch)
			{
                max_branch = elem_aux->blist->get_num();
			}
        }
    }
    return max_branch;
}
            
//////////////////////////////////////////////////////
//member function definition of MNARow
//////////////////////////////////////////////////////

MNARow::MNARow
(
	Node *_node, 
	MNAMatrix *_matrix, 
	MNARow *_next)
{
    pivot     = _node;
    next     = _next;
    matrix     = _matrix;
    elem_list = NULL;
    size    = 0;

}

MNARow::~MNARow()
{
    MNAElement *eaux, *paux;
    if(elem_list)
	{
		eaux = paux = elem_list;
		while(eaux)
		{
			paux = eaux->next;
			delete eaux;
			eaux = paux;
		}
    }
    /*
	  if(next) delete next;
    */
}

/* add or create a MNA element if necessary */

MNAElement *
MNARow::add_create_elem(Node *_node)
{
    if(!_node)
	{
        return NULL;
	}

    // for ground node (real_num = 0), no action taken
    if(!_node->get_real_num())
	{
        return NULL;
	}

    MNAElement *elem = query_element(_node);

    if(elem)
	{
        return elem;
	}

    /* new element */
    elem = new MNAElement(_node,pivot);

    if(!elem)
	{
        return NULL;
	}

    if(!elem_list)
	{		
        elem_list = elem;
	}
    else
	{
        MNAElement *elem_aux, *elem_prev;
        elem_aux = elem_prev = elem_list;

        /* find an appropriate position for the new element 
		   (increasing order sorted by num in the Node) */
        while(elem_aux && elem_aux->node->get_num() < elem->node->get_num())
		{
			elem_prev = elem_aux;
			elem_aux = elem_aux->Next();
        }

        MNAElement *elem_aux1 = elem_prev->Next();
        elem_prev->Next(elem);
        elem->Next(elem_aux1);
    }
    size++;
    return elem;
}    

/*
**    Create a MNAElement
*/

MNAElement *
MNARow::create_element
(
	Node *_node
)
{
    MNAElement *elem;

    if(!_node)
	{
        return NULL;
	}

    // for ground node (real_num = 0), no action taken
    if(!_node->get_real_num())
	{
        return NULL;
	}

    /*
    MNAElement *elem = query_element(_node);
    if(elem)
        return elem;
    */

    /* new element */
    elem = new MNAElement(_node,pivot);
    
	return elem;
}

/*
**    Put a MNAElement into the linked list in the row.
*/
void
MNARow::add_element(MNAElement *elem)
{
    if(!elem)
	{
        return;
	}

    if(!elem->node->get_num())
	{
        return;
	}
    if(!elem_list)
	{
        elem_list = elem;
	}
    else
	{
        MNAElement *elem_aux, *elem_prev;
        elem_aux = elem_prev = elem_list;

        /* find an appropriate position for the new element 
		   (increasing order sorted by num in the Node) */
        while(elem_aux && elem_aux->node->get_num() < elem->node->get_num())
		{
			elem_prev = elem_aux;
			elem_aux = elem_aux->Next();
        }
        MNAElement *elem_aux1 = elem_prev->Next();
        elem_prev->Next(elem);
        elem->Next(elem_aux1);
    }
    size++;
}    

/*
** if the MNA element with the node given exist?
*/
MNAElement *
MNARow::query_element
(
	Node *node
)
{
    MNAElement    *elem_aux = elem_list;
    if(!elem_list)
	{
        return NULL;
	}
    for(; elem_aux; elem_aux = elem_aux->next)
	{
        if(elem_aux->get_node()->get_num() == node->get_num())
		{
            return elem_aux;
		}
    }
    return NULL;
}

/*
** if the MNA element with the node given exist?
*/
MNAElement *
MNARow::query_element_by_index
(
	int index
)
{
    MNAElement    *elem_aux = elem_list;
    if(!elem_list)
	{
        return NULL;
	}
    for(; elem_aux; elem_aux = elem_aux->next)
	{
        if(elem_aux->get_node()->get_index() == index)
		{
            return elem_aux;
		}
    }
    return NULL;
}

/*
** check if there are duplicated elements or the element
** with the same node num with the pivot node's
*/
int
MNARow::check_element()
{
    MNAElement *elem_aux = elem_list;
    int    _size = 0;

    for(;elem_aux;elem_aux = elem_aux->Next())
	{
        _size++;
        if(elem_aux->get_node() == pivot)
		{
            error_mesg(INT_ERROR,"pivot equals to one of its element!");
            return -1;
        }
    }
    if(size != _size)
	{
        print_error(INT_ERROR,"inconsistant row size");
        return -1;
    }
    
	return 0;
}


/* 
** calculate all the rcl value of element
*/
void
MNARow::calc_elems_value()
{
    MNAElement *elem_aux = elem_list;
    complex        Czero = 0.0;

    for(; elem_aux; elem_aux = elem_aux->Next())
	{
        elem_aux->blist->calculate_rcl_value();
        // we assume unit frequency is given
        elem_aux->value = (elem_aux->blist->get_cvalue() + Czero);
    }
}

void
MNARow::calc_elems_value(PolyType ptype)
{
    MNAElement *elem_aux = elem_list;

    for(; elem_aux; elem_aux = elem_aux->Next())
	{
        elem_aux->blist->calculate_rcl_value(ptype);
	}
}


/*
**    General interface for processing branch
**    to build the MNA entry
*/

void
MNARow::process_branch(Branch *branch)
{
    switch(branch->get_type())
	{

	case dRES:
	case dCAP:
	case dIND:
		process_rcl(branch);
		break;

	case dMUIND:
		process_mutual_ind(branch);
		break;		

	case dVCCS:
		process_vccs(branch);
		break;

	case dCCCS:
		process_cccs(branch);
		break;
        
	case dCCVS: 
	case dVCVS: 
		process_cvs(branch);
		break;
        
	case dCUR:
	case dVOL:
		process_vi(branch);
		break;
        
	case dMOSFET:
		process_mosfet(branch);
		break;

	case dJFET:
		process_jfet(branch);
		break;
        
	case dDIODE:
		process_diode(branch);
		break;

	case dBJT:
	case dMESFET:
	case dUNKNOWN:
	default:
	    char buf[256];
		sprintf(buf,"Unknow branch type: %s",branch->get_name());
		error_mesg(INT_ERROR,buf);
		break;
    }
}

/*
** perform the filling of element stamp for R C L element

cess_rlc:

   Element Stamp RLC

            i    |              i    j
            | Iy |       i      g   -g
            _   \|/
           | |           j     -g    g
           |g|
           |_|
            |
            j

   ------------------------------------------------------------------------ */

void
MNARow::process_rcl(Branch *branch)
{
    Node *node_aux;
    MNAElement * elem_aux;

    if(!branch)
	{
        print_error(INT_ERROR,"Invalid branch");
        return;
    }
#ifdef _DEBUG
    if(    branch->get_type() != dRES && 
        branch->get_type() != dCAP &&
           branch->get_type() != dIND)
	{

        print_error(INT_ERROR,"Invalid branch type");
        return;
    }
#endif

    /* first add the branch  to the pivot node */
    elem_aux = add_create_elem(pivot);
    if(elem_aux)
	{
        elem_aux->add_branch(branch, PLUS);
	}

    node_aux = (branch->get_node1() == pivot)? 
                branch->get_node2(): branch->get_node1();
    
	elem_aux = add_create_elem(node_aux);
    if(elem_aux)
	{
        elem_aux->add_branch(branch, MINUS);
	}

}


/*
** perform the filling of element stamp for mutual inductor element

process_mutual_ind():

   Element Stamp mutual inductor

            i    |   k    |             i    j      m   k
            | I1 |   |    | I2   i      A1   -A1   -B   B
            _   \|/  _   \|/
           | |      | |          j      -A1   A1    B   -B
           |k|      | |
           |_|      |_|          k      -B    B     A2  -A2
            |        |
            j        m           m       B    -B    -A2  A2

   A1 = 1/(sL1*(1-k))
   A2 = 1/(sL2*(1-k))
   B = k/(s*srt(L1*L2)*(1-k))

   ------------------------------------------------------------------------ */

void
MNARow::process_mutual_ind
(
	Branch *branch
)
{

	cout <<"process row: " << pivot->get_real_num() << endl;
	
    if(!branch)
	{
        print_error(INT_ERROR,"Invalid branch");
        return;
    }

    assert(branch->get_type() == dMUIND);	

	double k = branch->get_dvalue();
	assert(k < 1.0);
	double ratio = 1.0/(1.0 - k);

    /* first we need update all the existing mne elements of L1 and L2
	   to multiple them with 1/(1-k). This corresponding to the A1 and
	   A2 element stamps shwon at the beginning of the function. Note
	   that we assume all the L1 and L2 have been processed before the
	   mutual inductor statements.
	*/

	// first find the inductor is physically incident on the node
	// i.e. one of its nodes is a pivot node of the row.
	
	Branch *aux_bchL1 =  branch->get_mut_ind1();
	assert(aux_bchL1);
	Node *aux_L1node1 = aux_bchL1->get_node1();
	Node *aux_L1node2 = aux_bchL1->get_node2();

	Branch *aux_bchL2 =  branch->get_mut_ind2();
	assert(aux_bchL2);
	Node *aux_L2node1 = aux_bchL2->get_node1();
	Node *aux_L2node2 = aux_bchL2->get_node2();
	
	cout <<"L1node1: " << aux_L1node1->get_real_num() << endl;
	cout <<"L1node2: " << aux_L1node2->get_real_num() << endl;
	cout <<"L2node1: " << aux_L2node1->get_real_num() << endl;
	cout <<"L2node2: " << aux_L2node2->get_real_num() << endl;

	Branch *bchL1, *bchL2;
	Node *L1node1, *L1node2, *L2node1, *L2node2;
	
	// we want to make sure that bchL1 share the pivot node of the
	// row.
	bool pos_pivot = true; // means the positive node is pivot
	                       // node.
	if((aux_L1node1->get_num() == pivot->get_num()) || 
	   (aux_L1node2->get_num() == pivot->get_num() ))
	{
		if(aux_L1node1->get_num() == pivot->get_num())
		{
			pos_pivot = true;
		}
		else
		{
			pos_pivot = false;
		}
		bchL1 = aux_bchL1;
		bchL2 = aux_bchL2;
		L1node1 = aux_L1node1; L1node2 = aux_L1node2;
		L2node1 = aux_L2node1; L2node2 = aux_L2node2;
	}
	else
	{
		assert((aux_L2node1->get_num() == pivot->get_num()) || 
			   (aux_L2node2->get_num() == pivot->get_num()) );

		if(aux_L2node1->get_num() == pivot->get_num())
		{
			pos_pivot = true;
		}
		else
		{
			pos_pivot = false;
		}
		bchL1 = aux_bchL2;
		bchL2 = aux_bchL1;
		L1node1 = aux_L2node1; L1node2 = aux_L2node2;
		L2node1 = aux_L1node1; L2node2 = aux_L1node2;
	}

	// Process the inductor Lx which is incident on the pivot node. We
	// only process non-ground node.
	if(L1node1->get_real_num())
	{
		cout <<"Linode1: " << L1node1->get_real_num() << endl;
		
		MNAElement *elem = query_element(L1node1);
		assert(elem);
		
		int bindex = elem->get_blist()->get_branch_index(bchL1->get_name());
		DummBranch *dbch = elem->get_blist()->get_dummbranch(bindex);
		assert(dbch);

		dbch->set_mut_ind_ratio(ratio*dbch->get_mut_ind_ratio());
		dbch->calc_bvalue();		
	}
	
	if(L1node2->get_real_num())
	{
		cout <<"Linode2: " << L1node2->get_real_num() << endl;

		MNAElement *elem = query_element(L1node2);
		assert(elem);

		int bindex = elem->get_blist()->get_branch_index(bchL1->get_name());
		DummBranch *dbch = elem->get_blist()->get_dummbranch(bindex);
		assert(dbch);

		dbch->set_mut_ind_ratio(ratio*dbch->get_mut_ind_ratio());
		dbch->calc_bvalue();		
	}

	// Process the inductor Lx which is NOT incident on the pivot
	// node. We only process non-ground node.
	MNAElement * elem_aux;
    
    // compute the  B = k/(s*srt(L1*L2)*(1-k))`
	double l1 = bchL1->get_dvalue();
	double l2 = bchL2->get_dvalue();
	double B = k/( 2*PI*sqrt(l1*l2)*(1-k) );
	
	if(L2node1->get_real_num())
	{
		elem_aux = add_create_elem(L2node1);
		if(elem_aux)
		{
			// treat the dummey branch as a special branch
			if(pos_pivot)
			{
				elem_aux->add_branch(branch, B, MINUS);
			}
			else
			{
				elem_aux->add_branch(branch, B, PLUS);
			}
		}
	}
	
	if(L2node2->get_real_num())
	{	
		elem_aux = add_create_elem(L2node2);
		if(elem_aux)
		{
			// treat the dummy branch as a special branch
			if(pos_pivot)
			{
				elem_aux->add_branch(branch, B, PLUS);
			}
			else
			{
				elem_aux->add_branch(branch, B, MINUS);
			}
		}
	}

}


/* 
   Element Stamp VCCS

         |   i       k                   k    l
   ivccs |   |       |            i      B   -B
        \|/ /\       _    +
           /+ \     | |           j     -B    B
   BVg     \- /     |g|  Vg
            \/      |_|
             |       |    -
             j       l

   This procedure processes this controlled current source by adding
   it's contribution in the current row ()in the column
   corresponding to it's current variable. Also add the row
   corresponding to it's current variable
   ------------------------------------------------------------------------ */

void
MNARow::process_vccs(Branch *branch)
{
    MNAElement     *elem_aux;
    Node        *node_aux;
    
    if(branch->get_type() != dVCCS )
	{
        print_error(INT_ERROR,"Invalid branch type");
        return;
    }
    
  /* ----------------------------------------------------------------------
     At positive node of branch, add entry at first control node, add entry
     at second control node. (i,k) and (i,l)
     ---------------------------------------------------------------------- */
    if( pivot == branch->get_node1() )
	{
        /* element a(i,k) */
        node_aux = branch->get_node3();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, PLUS);
		}

        /* element -a(i,l) */
        node_aux = branch->get_node4();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, MINUS);
		}
    } 
	else
	{
/* ---------------------------------------------------------------------
    At negative node of branch, add entry at first control node, add entry
    at second control node. (j,k) and (j,l)
   --------------------------------------------------------------------- */
        /* element -a(j,k) */
        node_aux = branch->get_node3();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, MINUS);
		}
    
        /* element a(j,l) */
        node_aux = branch->get_node4();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, PLUS);
		}
    }
}

/* ------------------------------------------------------------------------
   process_cccs:

   Element Stamp CCCS

         |   i       k    |              k    l
   icccs |   |       | Iy |       i    Bg   -Bg
        \|/ /\       _   \|/
           /+ \     | |           j   -Bg    Bg
    BIy    \- /     |g|
            \/      |_|
             |       |
             j       l

   This procedure processes this controlled current source by adding
   it's contribution in the current row ()in the column
   corresponding to it's current variable. Also add the row
   corresponding to it's current variable

   Note: If the controlling branch is of adimettanc type, with
   of introduction of branch adimettance, CCCS can be viewed as
   VCCS, so it has the same element stamp as the VCCS.
   ------------------------------------------------------------------------ */

void
MNARow::process_cccs
(
	Branch *branch
)
{
    Node *node_aux;
    MNAElement * elem_aux;
    
    if(branch->get_type() != dCCCS )
	{
        print_error(INT_ERROR,"Invalid branch type");
        return;
    }
    
  /* ----------------------------------------------------------------------
     At positive node of branch, add entry at first control node, add entry
     at second control node. (i,k) and (i,l)
     ---------------------------------------------------------------------- */
    if( pivot == branch->get_node1() )
	{
        /* element a(i,k) */
        node_aux = branch->get_node3();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, PLUS);
		}


        /* element -a(i,l) */
        node_aux = branch->get_node4();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, MINUS);
		}

    } 
	else 
	{
    /* --------------------------------------------------------------------
       At negative node of branch, add entry at first control node, add entry
       at second control node. (j,k) and (j,l)
      -------------------------------------------------------------------- */
        /* element -a(j,k) */
        node_aux = branch->get_node3();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, MINUS);
		}
    
        /* element a(j,l) */
        node_aux = branch->get_node3();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, PLUS);
		}
    }
}


/* ------------------------------------------------------------------------
   process_vi:

   Element Stamp VI

       |    i                          i    j     iv    RHS
 iv    |    |                   i                 1
      \|/  ___
          /   \                 j                 -1
   v_VS  |  +  |
         |  -  |                iv     1    -1          -1
          \___/
            |
            j

   This procedure processes this independent current source by adding
   it's contribution in the current row () in the column
   corresponding to it's current variable. Also add the row
   corresponding to it's current variable
   ------------------------------------------------------------------------ */

void
MNARow::process_vi
(
	Branch *branch
)
{
    print_error(INT_ERROR,"we do not process indep current and voltage now");
}

/* ------------------------------------------------------------------------
   process_cvs:

   Element Stamp VCVS

         |   i       k                   i    j    k    l    Ivcvs
   ivcvs |   |       |            i                           1
        \|/ /\       _    +
           /+ \     | |           j                          -1
   rVg     \- /     |g|  Vg
            \/      |_|         Ivcvs    1   -1   -r   r
             |       |    -
             j       l

   Element Stamp CCVS

         |   i       k    |              i    j    k    l    Iccvs
   iccvs |   |       | Iy |       i                           1
        \|/ /\       _   \|/
           /+ \     | |           j                          -1
   rIy     \- /     |g|
            \/      |_|         Iccvs    1   -1   -r*g  r*g
             |       |
             j       l

   This procedure processes this controlled voltage source by adding
   it's contribution in the current row (cur_ptr) in the column
   corresponding to it's current variable. Also add the row
   corresponding to it's current variable

   ------------------------------------------------------------------------ */
void
MNARow::process_cvs
(
	Branch *branch
)
{
    Node *node_aux;
    MNAElement * elem_aux;
    
    if(branch->get_type() != dVCVS && branch->get_type() != dCCVS)
	{
        print_error(INT_ERROR,"Invalid branch type");
        return;
    }

	/* --------------------------------------------------------------------
	   The branch current here is a variable, the entry in the extra current
	   variable column is 1 if this is the positive node and -1 if not. This
	   is the equivelant of adding (i,Icvs) or (j,Icvs) dependang on the
	   row being processed.
	   -------------------------------------------------------------------- */

    
    if( pivot == branch->get_node1() )
	{
        /* element 1(i,Icvs) */
        node_aux = branch->get_cur_var();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, 1.0, PLUS);
		}
    }
    else
	{
        /* element -1(i,Icvs) */
        node_aux = branch->get_cur_var();
        elem_aux = add_create_elem(node_aux);
        if(elem_aux)
		{
            elem_aux->add_branch(branch, 1.0, MINUS);
		}
    }

	/* --------------------------------------------------------------------
       Now Add an extra row for this extra current variable. It is added
       to the bottom of the matrix. Add to the extra variable rows in a
       sorted order (if needed), and add the contributions from this branch
       -------------------------------------------------------------------- */
	if(!branch->was_processed())
	{
		MNARow * extra_row = matrix->new_mna_row(branch->get_cur_var());
		cout <<"Extra row generated!"<<endl;

		/* element (Icvs,i) = 1  */
		node_aux = branch->get_node1();
		elem_aux = extra_row->add_create_elem(node_aux);
		if(elem_aux)
		{
			elem_aux->add_branch(branch, 1.0, PLUS);
		}
            
		/* element (Icvs, j) = -1 */
		node_aux = branch->get_node2();
		elem_aux = extra_row->add_create_elem(node_aux);
		if(elem_aux)
		{
			elem_aux->add_branch(branch, 1.0, MINUS);
		}

		/* --------------------------------------------------------------------
           Add (Icvs,k) (A negative entry) and (Icvs,l) (A positive entry)
           -------------------------------------------------------------------- */
		if( branch->get_type() == dVCVS)
		{ 
			/* element -a(Icvs,k) */
			node_aux = branch->get_node3();
			elem_aux = extra_row->add_create_elem(node_aux);
			if(elem_aux)
			{
				elem_aux->add_branch(branch, MINUS);
			}

			/* element a(Icvs,l) */
			node_aux = branch->get_node4();
			elem_aux = extra_row->add_create_elem(node_aux);
			if(elem_aux){
				elem_aux->add_branch(branch, PLUS);
			}
		}
		else
		{ // CCVS
			/* element -a(Icvs,k) */
			node_aux = branch->get_cbranch()->get_node1();
			elem_aux = extra_row->add_create_elem(node_aux);
			if(elem_aux)
			{
				elem_aux->add_branch(branch, MINUS);
			}
                
			/* element a(Icvs,l) */
			node_aux = branch->get_cbranch()->get_node2();
			elem_aux = extra_row->add_create_elem(node_aux);
			if(elem_aux)
			{
				elem_aux->add_branch(branch, PLUS);
			}
		}

		branch->set_was_process(TRUE);
    }
}
        
/* other devices, for nonlinear device, a prior linearizing
   process will be invoked to convert them into linear
   small signal equivalent circuit, so by now, we only need
   the MNA process for linear devices
*/ 

void
MNARow::process_mosfet
(
	Branch *branch
)
{
    print_error(INT_ERROR,"we do not process MOSFET now");
}

void
MNARow::process_jfet
(
	Branch *branch
)
{
    print_error(INT_ERROR,"we do not process JFET now");
}

void
MNARow::process_bjt
(
	Branch *branch
)
{
    print_error(INT_ERROR,"we do not process BJT now");
}

void
MNARow:: process_diode
(
	Branch *branch
)
{
    print_error(INT_ERROR,"we do not process DIODE now");
}

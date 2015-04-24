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
 *    $RCSfile: branch.cc,v $
 *    $Revision: 1.6 $
 *    $Date: 2002/11/21 02:40:15 $
 */

#include "globals.h"
#include "branch.h"

/* global static variable defintion */

////////////////////////////////////////////////
// member function definition of Branch class 
///////////////////////////////////////////////

Branch::Branch
(
	char *_keyword, 
	DeviceType _type, 
    Node *_node1, 
	Node *_node2, 
	Node *_node3, 
    Node *_node4, 
	double _value, 
	int _index 
)
{
    stat = B_USED;
    wasProcessed = 0;
    num = _index;
    type = _type;
    name = ToLower(CopyStr(_keyword));
    var_name = NULL;
    node1 = _node1;
    node2 = _node2;
    extra1.node3 = _node3;
    extra2.node4 = _node4;
    value = _value;
    cvar = NULL;
    evar = NULL;
    cvalue = 0.0;

	// mutual inductor
	m_ind1 = NULL;
	m_ind2 = NULL;

    cbranch = NULL;
    model_name = NULL;

	// for semi-symbolic analysis
	m_is_symbolic = false;
	
    next = NULL;

    check_branch();
    calc_rcl_value();
}

/* 
**    symbolic branch. we do not check and calculate branch rlc value.
*/
Branch::Branch
(
	char *_keyword, 
	DeviceType _type, 
    Node *_node1, 
	Node *_node2, 
	Node *_node3, 
    Node *_node4, 
	char *_var_name, 
	int _index 
)
{
    stat = B_SYMB;
    wasProcessed = 0;
    num = _index;
    type = _type;
    name = ToLower(CopyStr(_keyword));
    var_name = ToLower(CopyStr(_var_name));
    node1 = _node1;
    node2 = _node2;
    extra1.node3 = _node3;
    extra2.node4 = _node4;
    value = 0.0;
    cvar = NULL;
    evar = NULL;
    cvalue = 0.0;

	// mutual inductor
	m_ind1 = NULL;
	m_ind2 = NULL;

	// for semi-symbolic analysis
	m_is_symbolic = false;

    cbranch = NULL;
    model_name = NULL;

    next = NULL;
}

Branch::Branch
(
	char *_keyword, 
	DeviceType _type,
	Node *_node1, 
	Node *_node2, 
	double _value, 
	int _index
)
{
    stat = B_USED;
    wasProcessed = 0;
    type = _type;
    num = _index;
    name = ToLower(CopyStr(_keyword));
    var_name = NULL;
    node1 = _node1;
    node2 = _node2;
    extra1.node3 = NULL;
    extra2.node4 = NULL;
    cvar = NULL;
    evar = NULL;
    value = _value;
    cvalue = 0.0;

	// mutual inductor
	m_ind1 = NULL;
	m_ind2 = NULL;

	// for semi-symbolic analysis
	m_is_symbolic = false;

	cbranch = NULL;
    model_name = NULL;

    next = NULL;

    check_branch();
    calc_rcl_value();
}

/* 
**    symbolic branch. we do not check and calculate branch rlc value.
*/
Branch::Branch
(
	char *_keyword, 
	DeviceType _type,
	Node *_node1, 
	Node *_node2, 
	char *_var_name, 
	int _index
	)
{
    stat = B_SYMB;
    wasProcessed = 0;
    type = _type;
    num = _index;
    name = ToLower(CopyStr(_keyword));
    var_name = ToLower(CopyStr(_var_name));
    node1 = _node1;
    node2 = _node2;
    extra1.node3 = NULL;
    extra2.node4 = NULL;
    cvar = NULL;
    evar = NULL;
    value = 0.0;
    cvalue = 0.0;

	// mutual inductor
	m_ind1 = NULL;
	m_ind2 = NULL;

	// for semi-symbolic analysis
	m_is_symbolic = false;

	cbranch = NULL;
    model_name = NULL;

    next = NULL;
}

/*
** Arbitrary branch type ( only require keywork, device type and
** index). It is also a symbolic branch.
*/

Branch::Branch
(
	char *_keyword, 
	DeviceType _type, 
	int _index
	)
{
    stat = B_SYMB;
    wasProcessed = 0;
    type = _type;
    num = _index;
    name = ToLower(CopyStr(_keyword));
    var_name = NULL;	
    node1 = NULL;
    node2 = NULL;
    extra1.node3 = NULL;
    extra2.node4 = NULL;
    cvar = NULL;
    evar = NULL;
    value = 0.0;
    cvalue = 0.0;
	
	// mutual inductor
	m_ind1 = NULL;
	m_ind2 = NULL;
	
	// for semi-symbolic analysis
	m_is_symbolic = false;

    cbranch = NULL;
    model_name = NULL;

    next = NULL;
}

///////////////////////////////////

Branch::~Branch()
{
    delete [] name;
    if(var_name)
        delete [] var_name;
    if(model_name)
        delete  [] model_name;
    if(next)
        delete next;
}

/*
** Check for abnormal branch condition
*/
void
Branch::check_branch()
{
    if(node1->get_real_num() == node2->get_real_num())
	{
        stat = B_DELETED;
        return;
    }
    if((extra1.node3 && extra2.node4) &&
       (extra1.node3->get_real_num() == extra2.node4->get_real_num())){
        stat = B_DELETED;
        return;
    }
        
    if(value == 0.0)
	{
        switch(type) 
		{
		case dRES:
		case dIND:
			stat = B_EUQAL;
			break;
		case dCAP:
			stat = B_DELETED;
			break;
		default:
			break;
        }
    }
}


/*
**    Calculate the adimattance value for the branch
*/

void
Branch::calc_rcl_value()
{
    cvalue = 0.0;
    if(stat == B_USED)
	{
        switch(type) 
		{
		case dRES:
			cvalue.set_r(value);
			break;
		case dCAP:
			cvalue.set_c(value);
			break;
		case dIND:
			cvalue.set_l(value);
			break;
		case dVCCS:
		case dVCVS:
		case dCCVS:
		case dCCCS:
			cvalue = value;
			break;

		default:
			cvalue = 0.0;
			break;
        }
        //cout <<name<<": Value: "<<cvalue<<endl;
    } 
	else if( stat == B_SYMB )
	{
        sprintf(_buf,"%s : symbolic branch,",name);
        print_mesg(_buf);
    }
    else
	{
        cvalue = 0.0;
        sprintf(_buf,"%s : abnormal branch, ignored",name);
        print_warn(_buf);
    }
}
/*
**    print the branch
*/

void
Branch::print_branch(ostream & out)
{

	if(stat == B_USED)
	{
		switch(type)
		{
		case dRES:
			out << "1/" << name;
			break;
		case dCAP:
			out << "s*(" << name <<")";
			break;
		case dIND:			
			out << "1/(s*" << name <<")";
			break;
		case dVCCS:
		case dVCVS:
		case dCCVS:
		case dCCCS:
			out << name;
			break;

		default:
			out << name;
			break;
		}
		//cout <<name<<": Value: "<<cvalue<<endl;
	} 
	else if( stat == B_SYMB )
	{	 
		//TODO: consider the mutual inductor
		switch(type)
		{
		case dMUIND:
			out << "(" << name << "/(s*C*(1-" 
				<< name << "))" <<")";
			break;
			
		default:
			out << name;			
		}		
	}      
}

/*
** lump two branchs togather 
** We assume two branches are RLC type
*/
void 
Branch::lump_branch
(
	char *_keyword, 
	double _value
	)
{
    char buf[256];
    if((WhichType(_keyword) == dRES) || (WhichType(_keyword) == dIND))
	{		
        value = _value*value/(_value+value);
	}	
    else
	{		
        value += _value;
	}	

    sprintf(buf,"%s+%s",name, _keyword);
    delete [] name;
    name = CopyStr(ToLower(buf));

    check_branch();
    calc_rcl_value();

    /*
	  sprintf(_buf,"lumped branch %s.",name);
	  print_mesg(_buf);
    */
}

void 
Branch::add_model_name(char *model)
{
    model_name = CopyStr(model);
}

/////////////////////////////////////////////////////
// the member function defintion for DummBranch Class 
/////////////////////////////////////////////////////

/*---------------------------------------------------------------
  Spec: calculate the dummey branch value in a MNA element.  This
  basically is the place for us to compute the real value of each
  individual symbol of an element in MNA matrix for s-expanded DDD
  (sddd).  Note that to compute the complex value for a whole MNA
  element, it is done in BranchList::calculate_rcl_value().
  ---------------------------------------------------------------*/
void
DummBranch::calc_bvalue()
{
    if(!branch)
        return;
    
    if(is_special())
	{
		// mutual inductor is treated as inductor here.
		if(branch->get_type() == dMUIND)
		{			
			bvalue = value*sign;
			type = IndPart;
		}
		else
		{
			bvalue = value*sign;
			type = ResPart;
		}		
        return;
    }

    if(branch->get_type() == dCAP)
	{
        bvalue = sign * branch->get_cvalue().get_c();
        type = CapPart;
		
    }
    else if(branch->get_type() == dIND)
	{
        /* (-1) -- we nagete L value before */
        bvalue = -1 * sign * branch->get_cvalue().get_l() * mut_ind_ratio;
        type = IndPart;
    }
    else if(branch->get_type() == dRES ||
			branch->get_type() == dVCCS ||
			branch->get_type() == dVCVS )
	{
        bvalue = sign * branch->get_cvalue().get_r();
        type = ResPart;
    }
    else if(branch->get_type() == dCCCS ||
			branch->get_type() == dCCVS )
	{
        bvalue = sign * branch->get_dvalue() * 
			branch->get_cbranch()->get_dvalue();
        type = ResPart;
    }
}

/////////////////////////////////////////////////////
// the member function defintion for BranchList Class 
/////////////////////////////////////////////////////

BranchList::~BranchList()
{ 
    if(blist)
	{
		DummBranch *baux, *paux;
		baux = paux = blist;
		while(baux)
		{
			paux = baux->next;
			delete baux;
			baux = paux;
		}
    }
}

/*----------------------------------------------------
  Spec: add the branch into the list with the order of
  adding.
  -----------------------------------------------------*/
void
BranchList::add_branch
(
	Branch *branch,int sign
) 
{
    DummBranch *b_aux = blist;

    // first check the existance of the branch
    if(!branch)
	{
        print_error(INT_ERROR,"NULL branch");
        return;
    }

    if(chk_exist(branch))
        return;

    if(!blist)
        blist = new DummBranch(branch,sign);
    else
	{
        while(b_aux->next) b_aux = b_aux->next;
        b_aux->next = new DummBranch(branch,sign);
    }
    num++; Nnum++; Dnum++;
}

void
BranchList::add_branch
(
	Branch *branch,
	double value,
	int sign
	) 
{
    DummBranch *b_aux = blist;

    // first check the existance of the branch
    if(chk_exist(branch))
        return;

    if(!branch)
	{
        print_error(INT_ERROR,"NULL branch");
        return;
    }
    if(!blist)
	{		
        blist = new DummBranch(branch,value, sign);
	}	
    else
	{
        while(b_aux->next) b_aux = b_aux->next;
        b_aux->next = new DummBranch(branch,value, sign);
    }

    num++; Nnum++; Dnum++;
}

/*----------------------------------------------------
  Spec: find a branch given the branch name in the branchlist
  -----------------------------------------------------*/
Branch *
BranchList::query_branch
(
	const char *name
)
{
    if(!name)
        return NULL;

    DummBranch *db_aux;
    char *str = CopyStr(name);

    for(db_aux = blist; db_aux; db_aux = db_aux->next)
	{
        if(!strcmp(db_aux->get_branch()->name, ToLower(str)))
		{
            delete [] str;
            return db_aux->get_branch();
        }
    }

    delete [] str;
    return (Branch *)NULL;
}

/*-----------------------------------------------------------------
  Spec: calucalete the accumulated rcl value of the branch This
  basically is the place for us to compute the complex value of an
  element for a MNA matrix for complex DDD (CDDD).  Note that to
  compute the real value of each individual symbol of an element for
  SDDD, its done in DummBranch level at DummBranch::calc_bvalu().
  ----------------------------------------------------------------*/
void 
BranchList::calculate_rcl_value()
{
    DummBranch *list;
    rcl_complex the_value = 0.0;

    if(!blist)
        return;
    
    list = blist;
    for(; list; list = list->Next() )
	{
        if(list->is_special())
		{		
			if(list->branch->get_type() == dMUIND)
			{
				// We treat coupling terms as inductor so -1 is
				// multiplied to reflect this.
				rcl_complex val(0,0,list->value*(-1));
				the_value = val*list->sign;
			}
			else
			{
				the_value = the_value + list->value*list->sign;
			}
		}		
        else
		{
            if(list->branch->get_type() == dRES ||
               list->branch->get_type() == dCAP ||               
               list->branch->get_type() == dVCCS ||
               list->branch->get_type() == dVCVS )
			{
                the_value = the_value + 
					(list->sign * list->branch->get_cvalue());
            }
			else if(list->branch->get_type() == dIND)
			{
				// we need to consider mutual indutance if applicable.
				// (in this case, mut_ind_ratio != 1.0).
				the_value = the_value + 
					(list->sign * list->branch->get_cvalue()*
						list->get_mut_ind_ratio());
			}			
            else if(list->branch->get_type() == dCCCS ||
                    list->branch->get_type() == dCCVS )
			{
                the_value = the_value +
					(list->sign * 
					 (list->branch->get_dvalue() * 
					  list->branch->get_cbranch()->get_dvalue()));
			}
        }
    }

    //cout <<"old cval: "<<cvalue;
    cvalue = the_value;
    //cout <<"new cval: "<<cvalue;
}    

/*----------------------------------------------------
  Spec: calucalete the accumulated rcl value of the branch
  -----------------------------------------------------*/
void 
BranchList::calculate_rcl_value
(
	PolyType ptype
	)
{
    DummBranch *list;
    rcl_complex the_value = 0.0;

    if(!blist)
        return;
    
    list = blist;
    for(; list; list = list->Next() )
	{
        if(ptype == NUM && list->num_deleted)
            continue;
        else if(ptype == DEN && list->den_deleted)
            continue;
    
		if(list->is_special())
            the_value = the_value + list->value*list->sign;
        else
		{
            if(list->branch->get_type() == dRES ||
               list->branch->get_type() == dCAP ||
               list->branch->get_type() == dIND ||
               list->branch->get_type() == dVCCS ||
               list->branch->get_type() == dVCVS )
			{
                the_value = the_value + 
					(list->sign * list->branch->get_cvalue());
            }
            else if(list->branch->get_type() == dCCCS ||
                    list->branch->get_type() == dCCVS )
			{
                the_value = the_value +
					(list->sign * 
					 (list->branch->get_dvalue() * 
					  list->branch->get_cbranch()->get_dvalue()));
			}
        }
    }
    cvalue = the_value;
}    

/*----------------------------------------------------
  Spec: Make sure the branch name is unique  
  -----------------------------------------------------*/
int
BranchList::chk_exist
(
	const Branch *branch
	)
{
    DummBranch *aux_branch = blist;

    for(; aux_branch; aux_branch = aux_branch->Next())
	{
        if(!strcmp(aux_branch->branch->name, branch->name))
		{
			sprintf(_buf,"abnormal device : %s,ignored",branch->name);
			error_mesg(FAT_ERROR,_buf);
			return 1;
        }
    }
    return 0;
}
        
/*----------------------------------------------------
  Spec: print all the linked branches info
  -----------------------------------------------------*/
void
BranchList::print_branch
( 
	ostream & out
)
{
    DummBranch *laux = blist;

    out << "(" ;
    for(; laux; laux = laux->Next())
	{
        if(laux->sign > 0)
			out << " + ";
		else 
			out << " - ";

		laux->branch->print_branch(out);
		if(laux->branch->get_type() == dIND)
		{			
			if(laux->get_mut_ind_ratio() != 1.0)
			{
				out << "*(1/(1-k))";
			}
		}
		
		out << " ";
	}
    out <<")";
}

/*----------------------------------------------------
  Spec: print all the linked branches info
  -----------------------------------------------------*/
void
BranchList::print_branch
( 
	ostream & out, 
	PolyType ptype
)
{
    DummBranch *laux = blist;

    for(; laux; laux = laux->Next())
	{
        if(ptype == NUM && laux->num_deleted)
            continue;
        else if(ptype == DEN && laux->den_deleted)
            continue;
        if(laux->sign > 0)
            out << "+"<<laux->branch->name ;
        else
            out << "-"<<laux->branch->name ;
    }
}

/*----------------------------------------------------
  Spec: merge two BranchList
  -----------------------------------------------------*/
void
BranchList::merge_blist
(
	BranchList *_blist
	)
{
    DummBranch    *laux = blist;
    BranchList     *blist_new = copy_blist(_blist); 

    if(!blist)
	{
        blist = blist_new->blist;
    }
	else if(laux->next)
	{
        while(laux->next) laux = laux->next;
        laux->next = blist_new->blist;
    }
    else
	{
        laux->next = blist_new->blist;
	}

    num += blist_new->num;
}

/*----------------------------------------------------
  Spec: add a DummBranch type branch into list
  -----------------------------------------------------*/
void
BranchList::add_dbranch
( 
	DummBranch *dbranch
	)
{
    DummBranch    *laux = blist;
    if(!blist)
	{
        blist = dbranch;
	}
    else if(laux->next)
	{
        while(laux->next) laux = laux->next;
        laux->next = dbranch;
    }
    else
	{
        laux->next = dbranch;
	}
    num++;
}

/*----------------------------------------------------
  Spec: copy a blist
  -----------------------------------------------------*/
BranchList *
BranchList::copy_blist
(
	BranchList *_blist
	)
{
    BranchList    *lnew = new BranchList;
    DummBranch    *laux = _blist->blist;

    for(;laux; laux = laux->Next())
	{
        DummBranch  *daux = new DummBranch( laux );
        lnew->add_dbranch(daux);
    }
    
    return lnew;
}

/*----------------------------------------------------
  Spec: return the DummBranch indexed by given index
  We assume that index begins with 0.
  -----------------------------------------------------*/
DummBranch *
BranchList::get_dummbranch
( 
	int index 
	)
{
    int i = 0;
    DummBranch * daux = blist;
    
	for(; daux && i < index; daux = daux->Next(), i++);
    
	return daux;
}

/*
**    Find the branch index given its name
**    -1 is returned in case of failure.
*/

int
BranchList::get_branch_index
(
	char *name
	)
{
    int i = 0;
    DummBranch * daux = blist;
    for(i=0; daux && i < num; daux = daux->Next(), i++)
	{
        if(!strcmp(daux->branch->get_name(),name))
		{			
            return i;
		}		
    }
    return -1;
}

/* we remove the numerical contribuation of given branch without
   really deleting it in the linked list.
*/
void
BranchList::update_cvalue
(
	char *name, 
	PolyType ptype
	)
{
    DummBranch * daux = blist;
    if(!name)
        return;
    for(; daux; daux = daux->Next())
	{
        if(!strcmp(daux->branch->get_name(),name))
		{
            cvalue = cvalue - daux->sign*daux->branch->get_cvalue();
            if((NUM == ptype && Nnum == 1) ||
               (DEN == ptype && Dnum == 1))
                cvalue = 0.0;
            //cout<<"cvalue:" <<cvalue<<endl;
            return;
        }
    }

    sprintf(_buf,"Can't find branch: %s in the branchlist",name);
    error_mesg(INT_ERROR,_buf);
}

void
BranchList::restore_cvalue(char *name)
{
    DummBranch * daux = blist;
    if(!name)
        return;
    for(; daux; daux = daux->Next())
	{
        if(!strcmp(daux->branch->get_name(),name))
		{
            cvalue = cvalue + daux->sign*daux->branch->get_cvalue();
            return;
        }
    }
    sprintf(_buf,"Can't find branch: %s in the branchlist",name);
    error_mesg(INT_ERROR,_buf);
}

/*
  Delete the branch from the branchlist for numerator or
  denominator. The function return the number of remained branches in
  the branchlist
*/

int
BranchList::delete_branch
(
	char *name, 
	PolyType ptype
	)
{    
    DummBranch *daux;

    if(!name)
	{
        return num;
	}

    daux = blist;
    for(;daux; daux = daux->Next())
	{
        if(!strcmp(daux->branch->get_name(),name))
		{
			// update the linked list
            if(ptype == NUM)
			{
                daux->num_deleted = 1;
                Nnum--;
                return Nnum;
            }
            else
			{ 
                daux->den_deleted = 1;
                Dnum--;
                return Dnum;
            }
        }
    }

    sprintf(_buf,"Can't find and delete branch: %s in the branchlist",name);
    error_mesg(INT_ERROR,_buf);
    return num;
}

int
BranchList::num_branch(PolyType ptype)
{   

    if(ptype == NUM)
        return Nnum;
    else 
        return Dnum;
}

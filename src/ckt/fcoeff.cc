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
 *    $RCSfile: fcoeff.cc,v $
 *    $Revision: 1.11 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/*
 * fcoeff.cc:
 * 
 *    routines related to calculation and manipulation of fully expaned
 *    coefficients represented by DDD
 *
 */

/* INCLUDES: */

#include "globals.h"
#include "control.h"
#include "complex.h"
#include "node.h"
#include "branch.h"
#include "mxexp.h"
#include "ddd_base.h"
#include "rclcompx.h"
#include "acan.h"
#include "fcoeff.h"
#include "cancel_flab_map.h"

FLabelMap *theFLabMap;

#define _DEBUG

#define _PRINT_SYMBOLIC_TERM // print out all the symbolic terms

/*****************************************************
member function of FLabelMap
******************************************************/

FLabelMap::FLabelMap
( 
	int _offset, 
	int _mod
)
{

	modulus = _mod;
	if(_offset%2 == 0)
	{
		offset = _offset;
	}
	else
	{
		offset = _offset + 1;
	}
}

int
FLabelMap::ddd2fcoeff
(
	int _olabel, 
	int _index
)
{
#ifdef _DEBUG
	if(_index >= modulus)
	{
		error_mesg(INT_ERROR,"invalid mapping index");
	}
#endif
  
	return (_olabel*modulus + offset + _index);
}

DummBranch *
FLabelMap::get_dummbranch
(
	int flabel
)
{		
	//return flabel2brchMap[flabel];	
	Int2DummbchMap::const_iterator i = flabel2brchMap . find (flabel);
	return (i !=  flabel2brchMap. end ()) ?  (*i) . second  :  NULL;
}

int
FLabelMap::get_flabel
(
	DummBranch *bch
)
{    
	Dummbch2IntMap::const_iterator i = brch2flabelMap . find (bch);
	return (i != brch2flabelMap . end ()) ?  (*i) . second  :  -1;
	
}

int 
FLabelMap::get_complex_ddd_label
(
	int flabel
)
{
	//return flabel2labeMap[flabel];		
	Int2IntMap::const_iterator i = flabel2labelMap . find (flabel);
	return (i !=  flabel2labelMap. end ()) ?  (*i) . second  :  -1;
}

void 
FLabelMap::update_maps
(
	int label,
	int flabel, 
	DummBranch *dbch
)
{
	assert(dbch);

	// update the two maps.
	flabel2brchMap[flabel] = dbch;
	brch2flabelMap[dbch] = flabel;
	flabel2labelMap[flabel] = label;	
	
}

void
FLabelMap::swap_two_labels
(
	int flabel1,
	int flabel2
)
{

	cout <<"**** Finally swap the index maps ..."<< endl;
	
	// swap the flabel to branch map
	DummBranch *dbch1 = flabel2brchMap[flabel1];
	DummBranch *dbch2 = flabel2brchMap[flabel2];

	assert(dbch1);
	assert(dbch2);	

	flabel2brchMap[flabel1] = dbch2;
	flabel2brchMap[flabel2] = dbch1;

   	// swap the branch to flabel map
	brch2flabelMap[dbch1] = flabel2;
	brch2flabelMap[dbch2] = flabel1;

	// swap the flabel to label map
	int label1 = flabel2labelMap[flabel1];
	int label2 = flabel2labelMap[flabel2];

	cout <<" (" <<label1 <<"," << flabel1 <<",";
	dbch1->get_branch()->print_branch(cout);
	cout <<"); ";
	
	cout <<"(" <<label2 <<"," << flabel2 <<",";
	dbch2->get_branch()->print_branch(cout);
	cout <<")";
	
	cout <<" -> ";
		
	// finally make the changes.
	flabel2labelMap[flabel1] = label2;
	flabel2labelMap[flabel2] = label1;	
	

	cout <<" (" << flabel2labelMap[flabel2] <<"," << flabel2  <<",";
	dbch1->get_branch()->print_branch(cout);
	cout <<"); ";
	
	cout <<"(" << flabel2labelMap[flabel1] <<"," << flabel1 <<",";
	dbch2->get_branch()->print_branch(cout);
	cout <<")";

	cout << endl << endl;
   
	
}

void 
FLabelMap::printFlabel2BchMap
(
	ostream & out
)
{

	Int2DummbchMap::iterator itr = flabel2brchMap.begin();	
	for(; itr != flabel2brchMap.end(); itr++)
	{
	
		DummBranch *bch = itr->second;
		int findex =  itr->first;		
		
		cout <<"findex: " << findex << " index: " << flabel2labelMap[findex] 
			 <<" bch: ";		
		if(bch->get_sign() < 0)
		{
			cout <<"-";
		}		
		bch->get_branch()->print_branch(out);
		out << endl;		
	}
	
}

/*----------------------------------------------------
Recursively construct all the DDD fully expanded 
coefficient associated with the original DDD P.
----------------------------------------------------*/
FCoeffList *
DDDmanager::generate_all_fcoeffs
( 
	DDDnode *P,
	int nprod, 
	PolyType ptype,
	bool decancel
)
{
	FCoeffList *list;
	HasherTree *ht    = new HasherTree(FALSE);
	list = generate_all_fcoeffsR
		(
			P,
			nprod, 
			ht,
			ptype,
			decancel
		);

	delete ht;
	return list;
}

FCoeffList * 
DDDmanager::generate_all_fcoeffsR
( 
	DDDnode *P, 
	int nprod,
	HasherTree *ht, 
	PolyType ptype, 
	bool decancel
)
{
	FCoeffList *L1, *L0;
 
	if(!P)
	{
		return NULL;
	}

	if ( P == one || P == zero)
	{
		return NULL;
	}

	if(!ht->Put(*P,P)) 
	{// Already calculated
		return P->fcoefflist;
	}

	L1 = generate_all_fcoeffsR
		(
			P->Then(), 
			nprod, 
			ht, 
			ptype, 
			decancel
		);

	if(L1 == NULL)
	{
		L1 = new FCoeffList(this, nprod); 
		L1 = L1->first_fcoefflist(P, ptype);
	}
	else
	{
		L1 = L1->fco_copy();// create a new one
		L1 = L1->list_ddd_product(L1, P, ptype, decancel);
	}

	L0 = generate_all_fcoeffsR
		(
			P->Else(), 
			nprod, 
			ht, 
			ptype,
			decancel
		);
	L1 = L1->fco_union(L1,L0);
  
	P->fcoefflist = L1;

	return L1;
}

void
DDDmanager::fcoeff_print_symbolic_path
(
	DDDnode *path, 
	double mul_adj,
	ostream& out
)
{
	double     sign = 1.0;    
	char       str[1023];
	DDDnode    *daux;

	str[0] = 0;
	if(!path)
	{
		return;
	}
	if( path == Empty())
	{
		return;
	}
	for(daux = path; daux != Base(); daux = daux->Then())
	{
		strcat(str, "(");
		//sign *= FCoeffCalcSign(daux, daux->Then());
		sign = sign * (daux->getSign()?1:-1);  
	
		DummBranch *dbch = theFLabMap->get_dummbranch(daux->Index());
		assert(dbch);
		if(dbch -> get_sign() > 0)
		{
			if((dbch->get_branch()->get_type() == dRES) ||
			   (dbch->get_branch()->get_type() == dIND))
			{
				strcat(str,"1/");				
			}
			if(dbch->is_special())
			{
				char buf[256];
				// special treatment for mutual indutance
				if(dbch->get_branch()->get_type() == dMUIND)
				{
					sprintf(buf,"%s/(s*C*(1-%s))", 
							dbch->get_branch()->get_name(),
							dbch->get_branch()->get_name());
					strcat(str, buf);
				}
				else
				{					
					sprintf(buf,"%g", dbch->get_value());
					
				}
			}
			else
			{				
				strcat(str, dbch -> get_bname());
				if(dbch->get_branch()->get_type() == dIND)
				{
					if(dbch->get_mut_ind_ratio() != 1.0)
					{
						strcat(str, "*(1/(1-k))");
					}
				}
			}
		}
		else		  
		{
			strcat(str, "-");
			if((dbch->get_branch()->get_type() == dRES) ||
			   (dbch->get_branch()->get_type() == dIND))
			{
				strcat(str,"1/");				
			}
			if(dbch->is_special())
			{
				char buf[256];
				// special treatment for mutual indutance
				if(dbch->get_branch()->get_type() == dMUIND)
				{
					sprintf(buf,"%s/(s*C*(1-%s))", 
							dbch->get_branch()->get_name(),
							dbch->get_branch()->get_name());
					strcat(str, buf);
				}
				else
				{					
					sprintf(buf,"%g", dbch->get_value());
					strcat(str, buf);
				}
			}
			else
			{
				strcat(str, dbch -> get_bname());
				if(dbch->get_branch()->get_type() == dIND)
				{
					if(dbch->get_mut_ind_ratio() != 1.0)
					{
						strcat(str, "*(1/(1-k))");
					}
				}
			}
		}
		//str = str + "-" + dbch -> get_bname();
	  
		strcat(str, ") ");

		/*
		char buf[16];
		sprintf(buf,"(%d) ", daux->Index());
		strcat(str, buf); 
		*/
	} 

	bool compute_sign = true;
	double path_value = mul_adj*FCPathValue(path, compute_sign);
	// out <<"mul_adj: " << mul_adj << "\t";
	
	sprintf(_buf,",%g", path_value);
	if(sign > 0)
	{	  
		out<<"\t+ "<<str<<_buf<<endl;
	}
	else
	{
		out<<"\t- "<<str<<_buf<<endl;
	}
}

/*****************************************************
member function of FCoeff
******************************************************/
FCoeff::FCoeff()
{
	order    = 0;
	value  = 0.0;
	dval  = 0.0;
	fcoeff_ddd = NULL;
	num_paths = 0;
	keep = 1; // all the coefficient are lept initially
	prev_path = NULL;
	prev_pval = 0.0;
}

FCoeff::~FCoeff()
{;}

/*----------------------------------------------------
We assume the _num is the number of elements in
a product terms.
-----------------------------------------------------*/
void
FCoeff::calc_fcoeff
( 
	DDDmanager *_ddd_mgr,
	HasherTree *prec_ht,
	HasherTree *ht,
	int offset
)
{

	if( order%4 == 0)
	{
		value = complex(1,0);
	}
	else if(order%4 == 1)
	{
		value = complex(0,1);
	}
	else if(order%4 == 2)
	{
		value = complex(-1,0);
	}
	else if(order%4 == 3)
	{
		value = complex(0,-1);
	}

	_ddd_mgr->PreFCoeffCalculationR(fcoeff_ddd, prec_ht);

	// dump the fcoeff DDD structure
	//_ddd_mgr->PrintDDDTree(fcoeff_ddd);
	
	dval = _ddd_mgr->GetFCoeffValueR(fcoeff_ddd,ht);
	value = value * dval;

#ifdef _DEBUG
	// remove the 2*pi from all L and C elements
	double mul_adj = pow(2*PI, (double)(offset - order));
	
	cout <<"order:"<<order - offset;
	// cout <<", fcoeff: "<<value
	// cout <<" mul_adj:"<<mul_adj<< endl;
	// cout <<" raw value:"<<dval<< endl;
	cout <<" raw value*mul_adj:"<<dval*mul_adj<< endl;
  
	_ddd_mgr->statistic(fcoeff_ddd);
	cout << endl;
#endif

}

/*****************************************************
member function of FCoeffList
******************************************************/

/*----------------------------------------------------
We assume the _num is the number of elements in
a product terms.
-----------------------------------------------------*/
FCoeffList::FCoeffList
( 
	DDDmanager *_ddd_mgr, 
	int _num
)
{
	ddd_mgr = _ddd_mgr;
	offset = _num;
	max_order = 2*_num+1; // we may have negative order

	// max_order = _num+1; // we may have 0 order and _num order
	fcoeff = new FCoeff[max_order];
	supp_init = 0;
	fcoeff_calced = 0;
	cratio = 0.0;

	// initialize all the DDD coefficient
	for(int i = 0; i < max_order; i++)
	{
		fcoeff[i].fcoeff_ddd = ddd_mgr->Empty();
		fcoeff[i].set_order(i);
	}
}

FCoeffList::~FCoeffList()
{
	delete [] fcoeff;
}

/*----------------------------------------------------
proform the production of FCoeffList and a DDD node.
_index is assumed to be the orignal DDD node label.
-----------------------------------------------------*/

FCoeffList *
FCoeffList::fco_product
(
	FCoeffList *_list, 
	int new_label,  
	ElemPart _part
)
{
	int i;
	DDDnode *new_ddd;

	if(!_list)
	{
		return NULL;
	}

	if(_part == ResPart)
	{
	
		new_ddd = ddd_mgr->Change(ddd_mgr->Base(), new_label);
		
		for( i = 0; i < max_order; i++)
		{
			_list->fcoeff[i].fcoeff_ddd = 
				ddd_mgr->Product(_list->fcoeff[i].fcoeff_ddd,new_ddd);
		}

		return _list;
	}
	else if(_part == CapPart )
	{
		new_ddd = ddd_mgr->Change(ddd_mgr->Base(), new_label);
		
		for( i = 0; i < max_order; i++)
		{
			_list->fcoeff[i].fcoeff_ddd = 
				ddd_mgr->Product(_list->fcoeff[i].fcoeff_ddd,new_ddd);
		}
		/*
		  if(_list->fcoeff[max_order -1].fcoeff_ddd != ddd_mgr->Empty())
		  cout <<"Some thing wrong here1"<<endl;
		*/

		for( i = max_order-1; i > 0; i--)
		{
			_list->fcoeff[i].fcoeff_ddd = _list->fcoeff[i-1].fcoeff_ddd;
		}
		/*
		  cout <<"the max order: "<<_list->fcoeff[max_order -1].get_order() <<endl;
		*/
            
		_list->fcoeff[0].fcoeff_ddd = ddd_mgr->Empty();
		return _list;
	}
	else if(_part == IndPart)
	{
		new_ddd = ddd_mgr->Change(ddd_mgr->Base(), new_label);
		for( i = 0; i < max_order; i++)
		{
			_list->fcoeff[i].fcoeff_ddd = 
				ddd_mgr->Product(_list->fcoeff[i].fcoeff_ddd, new_ddd);
		}
		
		if(_list->fcoeff[0].fcoeff_ddd != ddd_mgr->Empty())
			cout <<"Some thing wrong here0"<<endl;
		

		for( i = 0; i < max_order - 1; i++)
		{
			_list->fcoeff[i].fcoeff_ddd = _list->fcoeff[i+1].fcoeff_ddd;
		}
		/*
		  cout <<"the min order: "<<_list->fcoeff[0].get_order() <<endl;
		*/
		_list->fcoeff[max_order-1].fcoeff_ddd = ddd_mgr->Empty();
	
		return _list;
	}
	else
	{
		error_mesg(INT_ERROR,"Invalid element type");
		return NULL;
	}
}

/*----------------------------------------------------
proform the production of FCoeffList and a DDD node.
_index is assumed to be the orignal DDD node label
with the consideration of removal of cancelling terms.
(decancel)
-----------------------------------------------------*/

FCoeffList *
FCoeffList::fco_product_with_decancel
(
	FCoeffList *_list, 
	int new_label,
	ElemPart _part
)
{
	int i;
	DDDnode *new_ddd;

	if(!_list)
	{
		return NULL;
	}

	if(_part == ResPart)
	{
		new_ddd = ddd_mgr->Change(ddd_mgr->Base(), new_label);
		
		for( i = 0; i < max_order; i++)
		{
			DDDnode *coeff_ddd = _list->fcoeff[i].fcoeff_ddd;

			if(theCancelFLabMap)
			{
				CancelLab * cancel_labs = 
					theCancelFLabMap->get_cancel_lab_list(new_label);
				if(cancel_labs)
				{
					CancelLab *tmp_clab = cancel_labs;
					for(; tmp_clab != NULL; tmp_clab = tmp_clab->m_next)
					{
						int cancel_lab = tmp_clab->m_label;
						coeff_ddd = ddd_mgr->Subset0(coeff_ddd, cancel_lab);
						/*
						cout << "de-cancel (" << new_label
							 << "," << cancel_lab
							 << ")" << endl;
						*/
					}
				}
			}

			_list->fcoeff[i].fcoeff_ddd = 
					ddd_mgr->Product(coeff_ddd, new_ddd);
		}
		return _list;
	}
	else if(_part == CapPart )
	{
		new_ddd = ddd_mgr->Change(ddd_mgr->Base(), new_label);
		
		for( i = 0; i < max_order; i++)
		{
			DDDnode *coeff_ddd = _list->fcoeff[i].fcoeff_ddd;

			if(theCancelFLabMap)
			{
				CancelLab * cancel_labs = 
					theCancelFLabMap->get_cancel_lab_list(new_label);
				if(cancel_labs)
				{
					CancelLab *tmp_clab = cancel_labs;
					for(; tmp_clab != NULL; tmp_clab = tmp_clab->m_next)
					{
						int cancel_lab = tmp_clab->m_label;
						coeff_ddd = ddd_mgr->Subset0(coeff_ddd, cancel_lab);
						/*
						cout << "de-cancel (" << new_label
							 << "," << cancel_lab
							 << ")" << endl;
						*/
					}
				}
			}

			_list->fcoeff[i].fcoeff_ddd = 
				ddd_mgr->Product(coeff_ddd ,new_ddd);
			
		}
		
		if(_list->fcoeff[max_order -1].fcoeff_ddd != ddd_mgr->Empty())
			cout <<"Some thing wrong here1"<<endl;
		

		for( i = max_order-1; i > 0; i--)
		{
			_list->fcoeff[i].fcoeff_ddd = _list->fcoeff[i-1].fcoeff_ddd;
		}
		/*
		  cout <<"the max order: "<<_list->fcoeff[max_order -1].get_order() <<endl;
		*/
            
		_list->fcoeff[0].fcoeff_ddd = ddd_mgr->Empty();
		return _list;
	}
	else if(_part == IndPart)
	{
		new_ddd = ddd_mgr->Change(ddd_mgr->Base(), new_label);
		for( i = 0; i < max_order; i++)
		{
			DDDnode *coeff_ddd = _list->fcoeff[i].fcoeff_ddd;

			if(theCancelFLabMap)
			{
				CancelLab * cancel_labs = 
					theCancelFLabMap->get_cancel_lab_list(new_label);
				if(cancel_labs)
				{
					CancelLab *tmp_clab = cancel_labs;
					for(; tmp_clab != NULL; tmp_clab = tmp_clab->m_next)
					{
						int cancel_lab = tmp_clab->m_label;
						coeff_ddd = ddd_mgr->Subset0(coeff_ddd, cancel_lab);
						/*
						cout << "de-cancel (" << new_label
							 << "," << cancel_lab
							 << ")" << endl;
						*/
					}
				}
			}

			_list->fcoeff[i].fcoeff_ddd = 
				ddd_mgr->Product(coeff_ddd ,new_ddd);
			
		}
		/*
		  if(_list->fcoeff[0].fcoeff_ddd != ddd_mgr->Empty())
		  cout <<"Some thing wrong here0"<<endl;
		*/

		for( i = 0; i < max_order - 1; i++)
		{
			_list->fcoeff[i].fcoeff_ddd = _list->fcoeff[i+1].fcoeff_ddd;
		}
		/*
		  cout <<"the min order: "<<_list->fcoeff[0].get_order() <<endl;
		*/
		_list->fcoeff[max_order-1].fcoeff_ddd = ddd_mgr->Empty();
	
		return _list;
	}
	else
	{
		error_mesg(INT_ERROR,"Invalid element type");
		return NULL;
	}
}

/*----------------------------------------------------
Perform the merge of two coefficient lists by unioning
all the DDD fully expanded coefficient elements individually.
-----------------------------------------------------*/
FCoeffList *
FCoeffList::fco_union
(
	FCoeffList *_list1, 
	FCoeffList *_list2
)
{
	if(!_list1)
	{
		return _list2;
	}
	else if(!_list2)
	{
		return _list1;
	}

	for( int i = 0; i < max_order; i++)
	{
		_list1->fcoeff[i].fcoeff_ddd = 
			ddd_mgr->Union(_list1->fcoeff[i].fcoeff_ddd, 
						   _list2->fcoeff[i].fcoeff_ddd);
	}

	return _list1;
}

/*----------------------------------------------------
create and copy a fully expanded coefficient list.
-----------------------------------------------------*/
FCoeffList *
FCoeffList::fco_copy()
{
	FCoeffList *clist = new FCoeffList(ddd_mgr, offset);

	for(int i = 0; i <max_order; i++)
	{
		clist->fcoeff[i].fcoeff_ddd = fcoeff[i].fcoeff_ddd;
	}

	return clist;

}

/*----------------------------------------------------
Perfome the "product" of fully expanded coefficient list with a
DDD node.
-----------------------------------------------------*/
FCoeffList *
FCoeffList::list_ddd_product
(
	FCoeffList *_list, 
	DDDnode *_node, 
	PolyType ptype,
	bool decancel
)
{

	int bindex, num_branch, index = _node->Index();
	BranchList *blist ;
	DummBranch *dblist;
	FCoeffList  *L1, *L2;
	FCoeffList *result = new FCoeffList(ddd_mgr, offset);
 
	blist = ddd_mgr->get_label2elem()->get_elem(index)->get_blist();
	num_branch = blist->get_num();
	dblist = blist->get_blist();
 
	L1 = _list;
 
	for(bindex=0 ; bindex<num_branch && dblist;
		bindex++, dblist = dblist->Next())
	{

		int new_label = theFLabMap->ddd2fcoeff(index, bindex);		
		theFLabMap->update_maps(index, new_label, dblist);

		if(ptype == NUM && dblist->get_num_deleted())
		{
			continue;
		}
		else if(ptype == DEN && dblist->get_den_deleted())
		{
			continue;
		}

		if(dblist->get_type() == ResPart)
		{
			L2 = L1->fco_copy();
			if(decancel)
			{			
				L2 = fco_product_with_decancel(L2, new_label, ResPart);
			}
			else
			{
				L2 = fco_product(L2, new_label, ResPart);
			}

			result = fco_union(result,L2);
			delete L2;
		}
		else if(dblist->get_type() == CapPart)
		{
			L2 = L1->fco_copy();
			if(decancel)
			{
				L2 = fco_product_with_decancel(L2, new_label, CapPart);			
			}
			else
			{
				L2 = fco_product(L2, new_label, CapPart);
			}

			result = fco_union(result,L2);
			delete L2;
		}
		else if(dblist->get_type() == IndPart)
		{
			L2 = L1->fco_copy();
			if(decancel)
			{
				L2 = fco_product_with_decancel(L2, new_label, IndPart);
			}
			else
			{
				L2 = fco_product(L2, new_label, IndPart);
			}

			result = fco_union(result,L2);
			delete L2;
		}
	}

	delete L1;
	return result;
}

/*----------------------------------------------------
Build the first fully expanded coefficient list 
with a0 and a1 properly set by the DDD node given.
-----------------------------------------------------*/
FCoeffList *
FCoeffList::first_fcoefflist
(
	DDDnode *_node, 
	PolyType ptype
)
{
	int i, num_branch, index = _node->Index();
	BranchList *blist ;
	DummBranch *dblist;
	DDDnode *daux;
 
	blist = ddd_mgr->get_label2elem()->get_elem(index)->get_blist();
	num_branch = blist->get_num();
	dblist = blist->get_blist();
 
	for(i=0;i < num_branch && dblist; i++, dblist = dblist->Next())
	{

		int new_label = theFLabMap->ddd2fcoeff(index, i);		
		theFLabMap->update_maps(index, new_label, dblist);

		if(ptype == NUM && dblist->get_num_deleted())
		{
			continue;
		}
		else if(ptype == DEN && dblist->get_den_deleted())
		{
			continue;
		}
		if( dblist->get_type() == ResPart )
		{
			if(fcoeff[offset].fcoeff_ddd == ddd_mgr->Empty())
			{
				fcoeff[offset].fcoeff_ddd = ddd_mgr->Change(ddd_mgr->Base(),new_label);
			}
			else
			{
				daux = ddd_mgr->Change(ddd_mgr->Base(), new_label);
				fcoeff[offset].fcoeff_ddd = ddd_mgr->Union(fcoeff[offset].fcoeff_ddd,daux);
			}
		}
		else if( dblist->get_type() == CapPart )
		{
			if(fcoeff[offset + 1].fcoeff_ddd == ddd_mgr->Empty())
			{
				fcoeff[offset + 1 ].fcoeff_ddd = ddd_mgr->Change(ddd_mgr->Base(),new_label);
			}
			else
			{
				daux = ddd_mgr->Change(ddd_mgr->Base(), new_label);
				fcoeff[offset + 1].fcoeff_ddd = ddd_mgr->Union(fcoeff[offset + 1].fcoeff_ddd,daux);
			}
		}
		else if( dblist->get_type() == IndPart )
		{
			if(fcoeff[offset - 1].fcoeff_ddd == ddd_mgr->Empty())
			{
				fcoeff[offset - 1 ].fcoeff_ddd = ddd_mgr->Change(ddd_mgr->Base(), new_label);
			}
			else
			{
				daux = ddd_mgr->Change(ddd_mgr->Base(), new_label);
				fcoeff[offset - 1].fcoeff_ddd =
					ddd_mgr->Union(fcoeff[offset - 1].fcoeff_ddd,daux);
			}            
		}
	}
	return this;
}


/*----------------------------------------------------
Calculate the numerical value of fcoefficient
for a polynomial.
-----------------------------------------------------*/

void
ExpList::calc_fcoeff_list()
{
	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);

	Cofactor *cofact_aux = num_list;
	for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->get_fcoeff_list()->calc_fcoeff_list( prec_ht, ht );
	}

	cofact_aux = den_list;
	for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->get_fcoeff_list()->calc_fcoeff_list(prec_ht, ht );
	}

	delete ht;
	delete prec_ht;	
}

void
ExpList::calc_sfcoeff_list()
{
	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);

	Cofactor *cofact_aux = num_list;
	for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->get_sfcoeff_list()->calc_fcoeff_list( prec_ht, ht );
	}

	cofact_aux = den_list;
	for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->get_sfcoeff_list()->calc_fcoeff_list( prec_ht, ht );
	}

	delete ht;
	delete prec_ht;
	
}

/*----------------------------------------------------
We assume the _num is the number of elements in
a product terms.
-----------------------------------------------------*/
void
FCoeffList::calc_fcoeff_list
(
	HasherTree *prec_ht,
	HasherTree *ht
)
{
	for( int i = 0; i < max_order; i++)
	{
		if(fcoeff[i].keep)
		{
			fcoeff[i].calc_fcoeff(ddd_mgr, prec_ht, ht, offset);
			//fcoeff[i].set_order(i);
		}
	}
	//fcoeff_calced = 1;
}

complex
FCoeffList::evaluate
( 
	double freq 
)
{
	complex res = 0.0;
	register double    nfreq = 1.0;

	float pwr = (max_order-1)/2;
	
	// the frequency should start with (f)^(-pwr)
	nfreq = pow(freq, -pwr);
		
	//assert(fcoeff_calced);

	//cout <<"freq: " << freq << endl;	
	//cout <<"begin nfreq: " << nfreq << endl;

	for( int i = 0; i < max_order; i++, nfreq = nfreq * freq)
	{
		
		
		if(!fcoeff[i].keep)
		{
			//cout <<"skip power: "<<i<<endl;
			continue;
		}
		res = res + fcoeff[i].fcoeff_value()*nfreq;
		//cout <<"power: "<<i<<", val: "<<nfreq<<endl;
	}

	//cout <<"end nfreq: " << nfreq << endl;

	return res;
}

void
FCoeffList::keep_fcoeff_ddd()
{
	for(int i = 0; i < max_order; i++)
	{
		if(fcoeff[i].keep)
		{
			ddd_mgr->Keep(fcoeff[i].fcoeff_ddd);
			if(fcoeff[i].Prev_path())
			{
				ddd_mgr->Keep(fcoeff[i].Prev_path());
			}
		}
	}
}

void
FCoeffList::unkeep_fcoeff_ddd()
{
	for(int i = 0; i < max_order; i++)
	{
		if(fcoeff[i].keep)
		{
			ddd_mgr->UnKeep(fcoeff[i].fcoeff_ddd);
			if(fcoeff[i].Prev_path())
			{
				ddd_mgr->UnKeep(fcoeff[i].Prev_path());
			}
		}
	}
}

/*
    we suppress all the coefficient whose numeric size is
    smaller than a fraction of the value of its polynomial
    for all the frquency points.
    if( coeff * _freq^n >= reg_mag )
        mark the coefficient as undeletable.
*/

void
FCoeffList::fcoeff_suppress
(
	double _freq, 
	double mag_ref
)
{

  register double    nfreq = 1.0;
  complex cval;
  double  rval;
    
  if(!supp_init)
  {
	  fcoeff_supp_init();
	  supp_init = 1;
  }
        
  //cout <<"freq:"<<_freq<<endl;
  for( int i = 0; i < max_order; i++, nfreq = nfreq * _freq)
  {
	  if(fcoeff[i].keep)
	  {
		  continue;
	  }
	  cval = fcoeff[i].fcoeff_value()*nfreq;
	  if(real(cval) == 0)
	  {
		  rval = fabs(imag(cval));
	  }
	  else
	  {
		  rval = fabs(real(cval));
	  }
	  if( rval > mag_ref )
	  {
		  cout <<"Power: "<<i<<" kept"<<endl;
		  //cout <<" rval:"<<rval<<" mag_ref:"<<mag_ref<<endl;
		  fcoeff[i].keep = 1;
	  }
  }    
  statistic(cout);
  //cout <<"nfreq:"<<nfreq<<endl;
}

/* only keep the coefficient specificed.
   For zeropole extraction.
*/

void
FCoeffList::fcoeff_suppress
(
	int pindex
)
{
    
  fcoeff_supp_init();
  fcoeff[pindex].keep = 1;
  fcoeff[pindex + 1].keep = 1;
  return;
}

/*
    Mark all the coefficient as deletable at first
*/
void FCoeffList::fcoeff_supp_init()
{
  for(int i = 0; i < max_order; i++)
  {
	fcoeff[i].keep = 0;
  }
}


/*
    Delete all the DDD nodes with given index whose
    contribion is insignificant.
*/
void
FCoeffList::fcoeff_elem_elim
(
	int index, 
	int bindex
)
{
	int     i;
	int    findex;
	DDDnode *new_cof;
	BranchList *blist;
	double exa_val, simp_val;

	//fcoeff_calced = 0;
	blist = ddd_mgr->get_label2elem()->get_elem(index)->get_blist();
	DummBranch * dbch = blist->get_dummbranch(bindex);
	assert(dbch);
	findex = theFLabMap->get_flabel(dbch);

	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);

	for( i = 0; i < max_order; i++)
	{
		if(!fcoeff[i].keep)
		{
			continue;
		}

		new_cof = ddd_mgr->Subset0(fcoeff[i].fcoeff_ddd,findex);
		if(new_cof == ddd_mgr->Empty())
		{
			cout<<"order:"<<i;
			cout <<",not cofactor found for "<<findex<<endl;
			continue;
		}

		// calculation intialization
		ddd_mgr->PreFCoeffCalculationR(new_cof, prec_ht);

		// calculate the exact DDD value
		//ddd_mgr->PreFCoeffCalculationR(fcoeff[i].fcoeff_ddd);
		//exa_val = ddd_mgr->GetFCoeffValueR(fcoeff[i].fcoeff_ddd);
		exa_val = fcoeff[i].dval;

		// then we calculate the simplied DDD value
		//elem_val = blist->get_dbranch_val( bindex );
		
		simp_val = ddd_mgr->GetFCoeffValueR(new_cof,ht);
	

		if(fabs((exa_val-simp_val)/exa_val) < opMagErr)
		{
			cout<<"order:"<<i;
			cout <<",findex:"<<findex<<" deleted"<<endl;
			cout <<"exa_val:"<<exa_val<<" simp:"<<simp_val<<endl;
			fcoeff[i].fcoeff_ddd = new_cof;
			/*
			  ddd_mgr->Subset0(fcoeff[i].fcoeff_ddd,findex);
			*/
		}
    
	}
	//fcoeff_calced = 1;

	delete ht;
	delete prec_ht;
	
}


/*
        Perform the elimination of cancellation terms
        The four index is the DDD index for fcofficients.
        There are position is as follows:
        ind1    ind2
        ind3    ind4
        So, the valid combination is ind1*ind4 and ind2*ind3
 
*/

void
FCoeffList::fcoeff_decancel
(
	int ind1, 
	int ind2, 
	int ind3, 
	int ind4
)
{
  DDDnode *sec1, *sec2, *cosec;
  DDDnode *dn1, *dn2; 
  int i;

  //fcoeff_calced = 0;
  //cout<<" ind1: "<<ind1<<" ind2: "<<ind2<<" ind3: "<<ind3<<" ind4: "<<ind4<<endl;
  for( i = 0; i < max_order; i++)
  {
	  if(!fcoeff[i].keep)
	  {
		  continue;
	  }
    
	  cosec = ddd_mgr->Subset1(fcoeff[i].fcoeff_ddd,ind1);
	  if(cosec == ddd_mgr->Empty())
	  {
		  continue;
	  }
	  cosec = ddd_mgr->Subset1(cosec,ind4);
	  if(cosec == ddd_mgr->Empty())
	  {
		  continue;
	  }

#ifdef _DEBUG
	   static  double path_deleted = 0.0;
	  path_deleted += 2*ddd_mgr->PathsInFunction(cosec);
	  cout <<path_deleted;
	  cout<<"path deleted."<<endl;
#endif

	  /*
		sec2 = ddd_mgr->Subset1(fcoeff[i].fcoeff_ddd,ind2);
		if(sec2 == ddd_mgr->Empty())
		continue;
		sec2 = ddd_mgr->Subset1(sec2,ind3);
		if(sec2 == ddd_mgr->Empty())
		continue;
    
		cosec = ddd_mgr->Intsect(sec1,sec2);
		if(cosec == ddd_mgr->Empty())
		continue;
	  */
        
	  // we find cancellation terms
	  // And, we perform the DDD node reduction
	  dn1 = ddd_mgr->Base();
	  dn2 = ddd_mgr->Base();

	  dn1 = ddd_mgr->Change(dn1,ind1);
	  dn1 = ddd_mgr->Change(dn1,ind4);

	  dn2 = ddd_mgr->Change(dn2,ind2);
	  dn2 = ddd_mgr->Change(dn2,ind3);
        
	  sec1 = ddd_mgr->Product(cosec,dn1);
	  sec2 = ddd_mgr->Product(cosec,dn2);
	  //cosec = ddd_mgr->Union(sec1,sec2);
	  //cout<<endl<<"Path deleted"<<endl;
	  //ddd_mgr->statistic(cosec);
	  fcoeff[i].fcoeff_ddd = 
		  ddd_mgr->Diff(fcoeff[i].fcoeff_ddd,sec1);
	  fcoeff[i].fcoeff_ddd = 
		  ddd_mgr->Diff(fcoeff[i].fcoeff_ddd,sec2);
  }
}

/*
        Perform the elimination of cancellation terms
        The four index is the DDD index for fcofficients.
        There are position is as follows:
        ind1    ind2
        ind3    ind4
        So, the valid combination is ind1*ind4 and ind2*ind3
		Note this function only operates on one coefficient DDD
		instead of whole coefficient DDD lists
 
*/

DDDnode *
FCoeffList::fcoeff_decancel_index
(
	DDDnode *_fcddd, 
	int ind1, 								 
	int ind2, 
	int ind3, 
	int ind4
)
{
	DDDnode *sec1, *sec2, *cosec;
	DDDnode *dn1, *dn2; 

	//fcoeff_calced = 0;

	cosec = ddd_mgr->Subset1(_fcddd,ind1);
	if(cosec == ddd_mgr->Empty())
	{
		return _fcddd;
	}
	cosec = ddd_mgr->Subset1(cosec,ind4);
	if(cosec == ddd_mgr->Empty())
	{
		return _fcddd;
	}

	/*
	  sec2 = ddd_mgr->Subset1(_fcddd,ind2);
	  if(sec2 == ddd_mgr->Empty())
	  return _fcddd;
	  sec2 = ddd_mgr->Subset1(sec2,ind3);
	  if(sec2 == ddd_mgr->Empty())
	  return _fcddd;

	  cosec = ddd_mgr->Intsect(sec1,sec2);
	  if(cosec == ddd_mgr->Empty())
	  return _fcddd;
	*/
	// we find cancellation terms
	// And, we perform the DDD node reduction
	dn1 = ddd_mgr->Base();
	dn2 = ddd_mgr->Base();

	dn1 = ddd_mgr->Change(dn1,ind1);
	dn1 = ddd_mgr->Change(dn1,ind4);

	dn2 = ddd_mgr->Change(dn2,ind2);
	dn2 = ddd_mgr->Change(dn2,ind3);
    
	sec1 = ddd_mgr->Product(cosec,dn1);
	sec2 = ddd_mgr->Product(cosec,dn2);
	cosec = ddd_mgr->Union(sec1,sec2);

	cout<<endl<<"# Path deleted:"<<endl;
	ddd_mgr->statistic(cosec);
	return ddd_mgr->Diff(_fcddd,cosec);
	/*
	  fcoeff[co_ind].fcoeff_ddd = 
	  ddd_mgr->Diff(fcoeff[co_ind].fcoeff_ddd,cosec);
	  fcoeff[co_ind].fcoeff_ddd = 
	  ddd_mgr->Diff(fcoeff[co_ind].fcoeff_ddd,sec2);
	*/
}

/* Instead of statically eliminate the cancelling terms,
   we find the two pairs of identical symbolics in two
   canceling paths and eliminate all the canceling terms
   the specific DDDs may have by calling
*/
DDDnode *
FCoeffList::fcoeff_dyn_decancel
(
	DDDnode *_fcddd, 
	DDDnode *P1, 
	DDDnode *P2
) 
{
	DDDnode *set1, *set2, *coset;
	DDDnode *daux, *daux1, *daux2;
	int    ind1, ind2, ind3, ind4;
	int    cind1, cind2, cind3, cind4;
	MNAElement *elem1, *elem2, *elem3, *elem4;

	/* we first generate two DDD sets with each path
	   in the two set containing only one DDD node in 
	   the corresponding DDD path given.
	*/
	if(P1 == ddd_mgr->Empty() || P2 == ddd_mgr->Empty())
	{
		return _fcddd;
	}

	set1 = ddd_mgr->Empty();
	set2 = ddd_mgr->Empty();
    
	for(daux1=P1, daux2 = P2; 
		(daux1 != ddd_mgr->Base()) && (daux2 != ddd_mgr->Base()); 
		daux1 = daux1->Then(), daux2 = daux2->Then() )
	{ 

		daux = ddd_mgr->Base(); 
		daux = ddd_mgr->Change(daux, daux1->Index());
		set1 = ddd_mgr->Union(set1,daux);

		daux = ddd_mgr->Base(); 
		daux = ddd_mgr->Change(daux, daux2->Index());
		set2 = ddd_mgr->Union(set2,daux);
	}
	/*
	  ddd_mgr->statistic(set1);
	  ddd_mgr->statistic(set2);
	*/
	coset = ddd_mgr->Intsect(set1,set2);
	set1 = ddd_mgr->Diff(set1,coset);
	set2 = ddd_mgr->Diff(set2,coset);

#ifdef _DEBUG
	cout <<"# of ddd in set1: "<<ddd_mgr->PathsInFunction(set1) <<endl;
	cout <<"# of ddd in set2: "<<ddd_mgr->PathsInFunction(set2) <<endl;
#endif
	if((ddd_mgr->PathsInFunction(set1) != 2) ||
	   (ddd_mgr->PathsInFunction(set2) != 2))
	{
		return _fcddd; // no further cancelling terms can be found
	}

	ind1 = set1->Index();
	ind4 = set1->Else()->Index();
	ind2 = set2->Index();
	ind3 = set2->Else()->Index();

#ifdef _DEBUG
	cout <<"ind1:"<<ind1
		 <<" ind2:"<<ind2
		 <<" ind3:"<<ind3
		 <<" ind4:"<<ind4<<endl;
#endif
	/* we then obtain the corresponding complex DDD index */
	cind1 = theFLabMap->get_complex_ddd_label(ind1);
	cind2 = theFLabMap->get_complex_ddd_label(ind2);
	cind3 = theFLabMap->get_complex_ddd_label(ind3);
	cind4 = theFLabMap->get_complex_ddd_label(ind4);

#ifdef _DEBUG
	cout <<"cind1:"<<cind1
		 <<" cind2:"<<cind2
		 <<" cind3:"<<cind3
		 <<" cind4:"<<cind4<<endl;

#endif
	elem1 = ddd_mgr->get_label2elem()->get_elem(cind1);
	elem2 = ddd_mgr->get_label2elem()->get_elem(cind2);
	elem3 = ddd_mgr->get_label2elem()->get_elem(cind3);
	elem4 = ddd_mgr->get_label2elem()->get_elem(cind4);

#ifdef _DEBUG
	cout <<"Elem1("<<elem1->get_row()
		 <<", "<<elem1->get_col()<<")"<<endl;
	cout <<"Elem2("<<elem2->get_row()
		 <<", "<<elem2->get_col()<<")"<<endl;
	cout <<"Elem3("<<elem3->get_row()
		 <<", "<<elem3->get_col()<<")"<<endl;
	cout <<"Elem4("<<elem4->get_row()
		 <<", "<<elem4->get_col()<<")"<<endl;
#endif
	if( ((elem4->get_row() == elem2->get_row()) &&
		 (elem1->get_col() == elem2->get_col()) &&
		 (elem1->get_row() == elem3->get_row()) &&
		 (elem4->get_col() == elem3->get_col()) ) ||
		((elem4->get_row() == elem3->get_row()) &&
		 (elem1->get_col() == elem3->get_col()) &&
		 (elem1->get_row() == elem2->get_row()) &&
		 (elem4->get_col() == elem2->get_col()) ) )
	{
		return fcoeff_decancel_index(_fcddd, ind1,ind2,ind3,ind4);
	}
	else
	{
		return _fcddd;
	}
}

/* print out all the dominant terms (pathes) in the simplified 
   in DDD coefficient list. Sign is coming from the cofactor 
   to which the DDD coefficient list belong.
*/

static int num_paths;

void
FCoeffList::print_fcoeff_list
(
	ostream& out
)
{

	num_paths = 0;
	
	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);
	calc_fcoeff_list(prec_ht, ht);

	// As PI(3.14) is multiplied with all the capacitance and divided for
	// for the inductance, we need to remove them here to correctly 
	// display the numerical value.
	double mul_adj = 1;
	
	for(int j = 0; j <= offset; j++)
	{
		mul_adj = 2*PI*mul_adj;		
	}
	
	for(int i = 0; i < max_order; i++)
	{				
	
		// First, update mul_adj for the current i. Must be the the first
		// statement in the for loop.
		mul_adj = mul_adj/(2*PI);

		if(!fcoeff[i].keep)
		{
			continue;
		}
		if(fcoeff[i].fcoeff_ddd == ddd_mgr->Empty())
		{
			continue;
		}
	
		out <<"\ts^"<<i - offset<<"(";
		ddd_mgr->PreFCoeffCalculationR(fcoeff[i].fcoeff_ddd, prec_ht);
		double value = ddd_mgr->GetFCoeffValueR(fcoeff[i].fcoeff_ddd, ht);
		value = value*mul_adj;
		
		out << value << endl;

#ifdef _PRINT_SYMBOLIC_TERM
		print_fcoeffs(fcoeff[i].fcoeff_ddd, mul_adj, out);
#endif
		out <<"\t) + " <<endl;

	
	}

	delete ht;
	delete prec_ht;
	out<<"\tnumber of terms:"<<num_paths<<endl;
}

void
FCoeffList::print_fcoeffs
(
	DDDnode *P, 
	double mul_adj,
	ostream& out
)
{
	DDDnode *coeff_ddd = P;
	DDDnode *msp_ddd;
	while(1)
	{

		//ddd_mgr->statistic(coeff_ddd);
		cout.flush();
		msp_ddd = ddd_mgr->FCDDDShortestPath(coeff_ddd);
		num_paths++;

		//ddd_mgr->statistic(coeff_ddd);
		ddd_mgr->fcoeff_print_symbolic_path(msp_ddd,mul_adj,out);
		coeff_ddd = ddd_mgr->Diff(coeff_ddd,msp_ddd);

		if(coeff_ddd == ddd_mgr->Empty())
		{
			break;
		}
	}
}

// print the polynomial in reverse order (matlab compatible) where
// D[0] is the nonzero coefficient of highest order of s.

void
FCoeffList::print_transfer_func
(
	ostream& out,
	int sign
)
{

	num_paths = 0;

	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);
	//calc_fcoeff_list(prec_ht, ht);

	// As PI(3.14) is multiplied with all the capacitance and divided for
	// for the inductance, we need to remove them here to correctly 
	// display the numerical value.
	double mul_adj = 1;
	
	for(int j = 0; j <= offset; j++)
	{
		mul_adj = mul_adj/(2*PI);		
	}
	
	for(int i = max_order - 1; i >= 0; i--)
	{				
	
		// First, update mul_adj for the current i. Must be the the first
		// statement in the for loop
		
		mul_adj = mul_adj*(2*PI);
		cout <<"order: " << i - offset <<" adjust: " << mul_adj << endl;
		
		if(fcoeff[i].fcoeff_ddd == ddd_mgr->Empty())
		{
			out <<0.0 << " ";			
			continue;
		}
			
		ddd_mgr->PreFCoeffCalculationR(fcoeff[i].fcoeff_ddd, prec_ht);
		double value = ddd_mgr->GetFCoeffValueR(fcoeff[i].fcoeff_ddd, ht);
		if(sign > 0)
		{			
			value = value*mul_adj;
		}		
		else
		{
			value = value*mul_adj*-1.0;			
		}
					
		
		out << value <<" ";
				
	}

	delete ht;	
	delete prec_ht;
  
}

void
FCoeffList::statistic
(
	ostream& out
)
{
	double npath, tpath = 0.0 ;

	out <<"########## statistics for fully expanded coefficient list"<<endl;

	//keep_fcoeff_ddd();
	//ddd_mgr->GC();
	for( int i = 0; i < max_order; i++)
	{
		if(!fcoeff[i].keep)
		{
			continue;
		}

		out <<endl;
		out <<"fcoefficient of order: "<<i - offset <<endl;    
		npath = ddd_mgr->PathsInFunction(fcoeff[i].fcoeff_ddd);
		//int nnode = ddd_mgr->NodesInFunction(fcoeff[i].fcoeff_ddd);
		tpath += npath;

		//ddd_mgr->statistic(fcoeff[i].fcoeff_ddd);
		cout << "#paths (DDD): " << npath <<endl;
		// cout << "#nodes (DDD): " << nnode << endl;
		// ddd_mgr->PrintSetVal(fcoeff[i].fcoeff_ddd,0.0,cout);
	}
	out <<"The total paths is "<<tpath<<endl;
	out <<"########## end of fcoefficient list"<<endl;

}

/*
 * compute the statisitcs for semisymbolic DDDs
 */

void
FCoeffList::semi_symbolic_statistic
(
	ostream& out,
	HasherTree *node_ht,
	HasherTree *term_ht 
)
{
	double npath, tpath = 0.0 ;

	double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    double e_time = 0.0;	

	out <<"########## statistics for fully expanded semi-symbolic coefficient list"<<endl;

	//keep_fcoeff_ddd();
	//ddd_mgr->GC();

	//HasherTree *node_ht    = new HasherTree(FALSE);
	//HasherTree *term_ht    = new HasherTree(FALSE);

	for( int i = 0; i < max_order; i++)
	{
		if(!fcoeff[i].keep)
		{
			continue;
		}

		out <<endl;
		out <<"fcoefficient of order: "<<i - offset <<endl;    
		npath = ddd_mgr->FSymbPathsInFunction(fcoeff[i].fcoeff_ddd);
		//int nnode = ddd_mgr->NodesInFunction(fcoeff[i].fcoeff_ddd);
		tpath += npath;
		
		cout << "#paths (DDD): " << npath <<endl;
		time1 = 1.0 * clock() / CLOCKS_PER_SEC;
		ddd_mgr->FSymbNodesInFunctionR(fcoeff[i].fcoeff_ddd, node_ht);
		time2 = 1.0 * clock() / CLOCKS_PER_SEC;
		e_time = e_time + time2 - time1;
		
		ddd_mgr->FSymbTerminalsInFunctionR(fcoeff[i].fcoeff_ddd, term_ht); 
		
	}
	out <<"The total #symbolic paths in the fcoeff list:"<<tpath<<endl;
	out <<"The total #symnolic nodes so far: " << node_ht->Count() << endl;
	out <<"The total #numerical terminals so far: " << term_ht->Count() << endl;
	out <<"The evaluation time for the semi-symbolic foceff list: " << e_time << endl;
	
	out <<"########## end of fcoefficient list"<<endl;

	//delete node_ht;
	//delete term_ht;	
}

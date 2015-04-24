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
 *    $RCSfile: coeff.cc,v $
 *    $Revision: 1.4 $
 *    $Date: 2001/11/25 05:42:30 $
 */


/*
 * coeff.cc:
 * 
 *    routines related to calculation and manipulation of complicated
 *    coefficients represented by DDD
 *
 */

/* INCLUDES: */

#include "globals.h"
#include "complex.h"
#include "node.h"
#include "branch.h"
#include "mxexp.h"
#include "ddd_base.h"
#include "rclcompx.h"
#include "acan.h"
#include "coeff.h"

LabelMap *theLabMap;

/*****************************************************
member function of LabelMap
******************************************************/

LabelMap::LabelMap( int _offset)
{
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
LabelMap::ddd2coeff
(
	int _olabel, 
	ElemPart _part
)
{
    switch(_part)
	{
	case ResPart: 
		return 3*_olabel + offset;
		break;
	case CapPart:
		return 3*_olabel + 1 + offset;
		break;
	case IndPart:
		return 3*_olabel + 2 + offset;
		break;
	default:
		error_mesg(INT_ERROR,"Invalid element part type");
		return 3*_olabel + offset;
		break;
	}
}

int
LabelMap::coeff2ddd
(
	int _mlabel
)
{
    int m = (_mlabel - offset)%3;
    int result = 0;

	switch(m)
	{
	case 0:
		result = (_mlabel - offset)/3;
		break;
	case 1:
		result = (_mlabel - 1 - offset)/3;
		break;
	case 2:
		result = (_mlabel - 2 - offset)/3;
		break;
    }
	return result;
}

ElemPart
LabelMap::which_part
(
	int _mlabel
)
{
    int m = (_mlabel - offset)%3;
    ElemPart result = ResPart;

	switch(m)
	{
	case 0:
		result =  ResPart;
		break;
	case 1:
		result = CapPart;
		break;
	case 2:
		result = IndPart;
		break;
    }
	return result;
}
    
/*----------------------------------------------------
recursively construct all the DDD coefficient associated
with the original DDD P.
----------------------------------------------------*/
CoeffList * 
DDDmanager::generate_all_coeffs
(
	DDDnode *P, 
	int nprod 
)
{
	CoeffList *list;
	HasherTree *ht    = new HasherTree(FALSE);
	list = generate_all_coeffsR(P, nprod, ht);
	delete ht;
	return list;

}
CoeffList * 
DDDmanager::generate_all_coeffsR
( 
	DDDnode *P, 
	int nprod, 
	HasherTree *ht 
)
{
	CoeffList *L1, *L0;

	if(!P)
	{
		return NULL;
	}
 
	if ( P == one || P == zero)
	{
		return NULL;
	}


	if(!ht->Put(*P,P)) // Already calculated
	{
        return P->coefflist;
	}

	L1 = generate_all_coeffsR(P->Then(), nprod, ht);
	if(L1 == NULL)
	{
		L1 = new CoeffList(this,nprod); 
		L1 = L1->first_coefflist(P);
	}
	else
	{
		L1 = L1->co_copy();// create a new one
		L1 = L1->list_ddd_product(L1, P);
	}

	L0 = generate_all_coeffsR(P->Else(), nprod, ht);
	L1 = L1->co_union(L1,L0);
	P->coefflist = L1;

	return L1;
}

/*****************************************************
member function of Coeff
******************************************************/
Coeff::Coeff()
{
    order    = 0;
    value  = 0.0;
    coeff_ddd = NULL;
    num_paths = 0;
}

Coeff::~Coeff()
{;}

/*----------------------------------------------------
We assume the _num is the number of elements in
a product terms.
-----------------------------------------------------*/
void
Coeff::calc_coeff
( 
	DDDmanager *_ddd_mgr, 
	HasherTree *ht
)
{
    double    coeff_val;

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

    _ddd_mgr->PreCoeffCalculationR(coeff_ddd);
    coeff_val = _ddd_mgr->GetCoeffValueR(coeff_ddd, ht);

#ifdef COLLECT_COEFF_DATA
    _ddd_mgr->statistic(coeff_ddd);
#endif 

    value = value * coeff_val;
    cout <<"order:"<<order;
    cout <<", coeff: "<<value<<endl;
}
/*****************************************************
member function of CoeffList
******************************************************/

/*----------------------------------------------------
We assume the _num is the number of elements in
a product terms.
-----------------------------------------------------*/
CoeffList::CoeffList
(
	DDDmanager *_ddd_mgr, 
	int _num
)
{
    ddd_mgr = _ddd_mgr;
    offset = _num;
    max_order = 2*_num+1; // we may have 0 order and _num order
    coeff = new Coeff[max_order];

    // initialize all the DDD coefficient
    for(int i = 0; i < max_order; i++)
	{
        coeff[i].coeff_ddd = ddd_mgr->Empty();
        coeff[i].set_order(i);
    }
}

CoeffList::~CoeffList()
{
    delete [] coeff;
}

/*----------------------------------------------------
proform the production of CoeffList and a DDD node.
_index is assumed to be the orignal DDD node label.
-----------------------------------------------------*/

CoeffList *
CoeffList::co_product
(
	CoeffList *_list, 
	int _index, 
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
        new_ddd =ddd_mgr->Change(ddd_mgr->Base(), 
								 theLabMap->ddd2coeff(_index,ResPart));
        for( i = 0; i < max_order; i++)
		{
            _list->coeff[i].coeff_ddd = 
				ddd_mgr->Product(_list->coeff[i].coeff_ddd,new_ddd);
		}
        return _list;
    }
    else if(_part == CapPart )
	{
        new_ddd = ddd_mgr->Change(ddd_mgr->Base(),
								  theLabMap->ddd2coeff(_index,CapPart));
        for( i = 0; i < max_order; i++)
		{
            _list->coeff[i].coeff_ddd = 
				ddd_mgr->Product(_list->coeff[i].coeff_ddd,new_ddd);
        }
        /*
		  if(_list->coeff[max_order -1].coeff_ddd != ddd_mgr->Empty())
		  cout <<"Some thing wrong here1"<<endl;
        */

        for( i = max_order -1; i > 0; i--)
		{
			_list->coeff[i].coeff_ddd = _list->coeff[i-1].coeff_ddd;
		}
        /*
		  cout <<"the max order: "<<_list->coeff[max_order -1].get_order() <<endl;
        */
            
        _list->coeff[0].coeff_ddd = ddd_mgr->Empty();
        
		return _list;
    }
    else if(_part == IndPart)
	{
        new_ddd = ddd_mgr->Change(ddd_mgr->Base(),
								  theLabMap->ddd2coeff(_index,IndPart));
        for( i = 0; i < max_order; i++)
		{
            _list->coeff[i].coeff_ddd = 
				ddd_mgr->Product(_list->coeff[i].coeff_ddd,new_ddd);
        }
        /*
		  if(_list->coeff[0].coeff_ddd != ddd_mgr->Empty())
		  cout <<"Some thing wrong here0"<<endl;
        */

        for( i = 0; i < max_order - 1; i++)
		{
			_list->coeff[i].coeff_ddd = _list->coeff[i+1].coeff_ddd;
		}
        /*
		  cout <<"the min order: "<<_list->coeff[0].get_order() <<endl;
        */
        _list->coeff[max_order-1].coeff_ddd = ddd_mgr->Empty();
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
all the DDD coefficient elements individually.
-----------------------------------------------------*/
CoeffList *
CoeffList::co_union
(
	CoeffList *_list1, 
	CoeffList *_list2
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
        _list1->coeff[i].coeff_ddd = 
            ddd_mgr->Union
			(
				_list1->coeff[i].coeff_ddd, 		  
				_list2->coeff[i].coeff_ddd
			);
	}
    return _list1;
}

/*----------------------------------------------------
create and copy a coefficient list.
-----------------------------------------------------*/
CoeffList *
CoeffList::co_copy()
{
    CoeffList *clist = new CoeffList(ddd_mgr, offset);
    for(int i = 0; i <max_order; i++)
	{
        clist->coeff[i].coeff_ddd = coeff[i].coeff_ddd;
	}

    return clist;

}

/*----------------------------------------------------
Perfome the "product" of coefficient list with a
DDD node.
-----------------------------------------------------*/
CoeffList *
CoeffList::list_ddd_product
(
	CoeffList *_list, 
	DDDnode *_node
)
{
    int    index = _node->Index();
    rcl_complex cval; 
    CoeffList    *L1, *L2, *result = NULL;

    cval = ddd_mgr->get_label2elem()->get_elem(index)->get_cvalue();

    L1 = _list->co_copy();
    if(cval.get_r() != 0.0)
	{
        result = co_product(_list,index,ResPart);
	}
    if(cval.get_c() != 0.0)
	{
        L2 = L1->co_copy();
        L2 = co_product(L2,index,CapPart);
        if(result)
		{
            result = co_union(result,L2);
            delete L2;
        }
        else
		{
            result = L2;
		}
    }
    if(cval.get_l() != 0.0)
	{
        L2 = L1->co_copy();
        L2 = co_product(L2,index,IndPart);
        if(result)
		{
            result = co_union(result,L2);
            delete L2;
        }
        else
		{
            result = L2;
		}
    }

    delete L1;
    return result;
}
    
/*----------------------------------------------------
Build the first coefficient list with a0 and a1 properly
set by the DDD node given.
-----------------------------------------------------*/
CoeffList *
CoeffList::first_coefflist
(
	DDDnode *_node
)
{
    int    index = _node->Index();
    rcl_complex cval; 

    cval = ddd_mgr->get_label2elem()->get_elem(index)->get_cvalue();

    if(cval.get_r() != 0.0)
	{
        coeff[offset ].coeff_ddd = 
			ddd_mgr->Change
			(
				ddd_mgr->Base(),
				theLabMap->ddd2coeff(index,ResPart)
			);
	}
    if(cval.get_c() != 0.0)
	{
        coeff[offset + 1].coeff_ddd = 
			ddd_mgr->Change
			(
				ddd_mgr->Base(),
				theLabMap->ddd2coeff(index,CapPart)
			);
	}
    if(cval.get_l() != 0.0)
	{
        coeff[offset -1].coeff_ddd = 
			ddd_mgr->Change
			(
				ddd_mgr->Base(),
				theLabMap->ddd2coeff(index,IndPart)
			);
	}

    return this;
}

/*----------------------------------------------------
We assume the _num is the number of elements in
a product terms.
-----------------------------------------------------*/
void
CoeffList::calc_coeff_list
(
	HasherTree *ht
)
{
    for( int i = 0; i < max_order; i++)
	{
        //coeff[i].set_order(i);
        coeff[i].calc_coeff(ddd_mgr, ht);
    }
}

complex
CoeffList::evaluate
( 
	double freq
)
{
    complex res = 0.0;
    register double    nfreq = 1.0;
    for( int i = 0; i < max_order; i++)
	{
		if(coeff[i].coeff_value() != 0.0)
		{
			res = res + coeff[i].coeff_value()*nfreq;
		}
        nfreq = nfreq * freq;
        //cout <<"power: "<<i<<", val: "<<nfreq<<endl;
    }
    return res;
}

void
CoeffList::keep_coeff_ddd()
{
    for( int i = 0; i < max_order; i++)
	{
        ddd_mgr->Keep(coeff[i].coeff_ddd);
	}
}

void
CoeffList::unkeep_coeff_ddd()
{
    for(int i = 0; i < max_order; i++)
	{
		ddd_mgr->UnKeep(coeff[i].coeff_ddd);
	}
}

void
CoeffList::statistic(ostream& out)
{
    double npath, tpath = 0.0 ;
    
	out <<"########## statistics for coefficient list"<<endl;
    
	for( int i = 0; i < max_order; i++)
	{
        out <<endl;
        out <<"coefficient of order: "<<i<<endl;    
        npath = ddd_mgr->PathsInFunction(coeff[i].coeff_ddd);
        tpath += npath;
        ddd_mgr->statistic(coeff[i].coeff_ddd);
        cout << "#paths (DDD): " << npath <<endl;
        //ddd_mgr->PrintSetVal(coeff[i].coeff_ddd,0.0,cout);
    }
    out <<"The total paths is "<<tpath<<endl;
    out <<"########## end of coefficient list"<<endl;
}

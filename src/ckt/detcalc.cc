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
 *    $RCSfile: detcalc.cc,v $
 *    $Revision: 1.10 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/*
 * determ_calc.cpp:
 * 
 *     Calculate the value of determinant based on DDD.  All the
 *     determinant value are implicitly represented The file include
 *     all the extensions of function definition for numerical DDD
 *     manipulations.
 *
 */

/* INCLUDES: */

#include "globals.h"
#include "node.h"
#include "branch.h"
#include "mxexp.h"
#include "ddd_base.h"
#include "rclcompx.h"
#include "coeff.h"
#include "fcoeff.h"
#include "dtstruct.h"

/*----------------------------------------------------
Spec: initialization for DDD numerical operation and 
      complex value memory allocation for each DDD node.
-----------------------------------------------------*/
void
DDDmanager::PreCalculationR
(
	DDDnode * P,
	HasherTree *ht
) 
{
    //ASSERT(P,"Invalid DDD node(=NULL)");

    //ASSERT(P->Else() != one,"Invalid DDD:P->Else() == one");

    if( P == zero || P == one)
	{
        return;
	}


	if(!ht->Put(*P,P))
	{// Already calculated 
		return;
	}
   
    if(!P->val_ptr) // need't to allocate space
	{        
		complex *val = new complex(0.0);
		P->val_ptr = val;
	}
	else
	{
		(*P->val_ptr) = complex(0,0);		
	}

    PreCalculationR( P->Then(), ht );
    PreCalculationR( P->Else(), ht );

    int sign;
    if(P->Then() != one)
	{
        if((sign = CalcSign(P,P->Then())) > 0)
		{
            P->sign = 1;
		}
        else if( sign < 0 )
		{
            P->sign = 0;
		}
        else
		{
            sprintf(_buf,"Invalid sign calculation(=0)in node %d\n", P->index);
            error_mesg(INT_ERROR,_buf);
        }
    }
    else
	{
        P->sign = 1;
	}
        
    //printf("Init Node %d. real:%e, imag:%e\n",P->index, real(V),imag(V));
    return; 
}

/*----------------------------------------------------
Spec: initialization for DDD coefficient numerical 
operation and complex value memory allocation for 
each DDD node.
-----------------------------------------------------*/
void
DDDmanager::PreCoeffCalculationR
(
	DDDnode * P
) 
{

    //ASSERT(P,"Invalid DDD node(=NULL)");

    //ASSERT(P->Else() != one,"Invalid DDD:P->Else() == one");

    if( P == zero || P == one)
	{
        return;
	}
     
    if(P->val_ptr)
	{// need't to allocate space
        return;
	}

    complex *val = new complex(0.0);
    P->val_ptr = val;

    PreCoeffCalculationR( P->Then() );
    PreCoeffCalculationR( P->Else() );

    int sign;
    if(P->Then() != one)
	{
        if((sign = CoeffCalcSign(P,P->Then())) > 0)
		{
            P->sign = 1;
		}
        else if( sign < 0 )
		{
            P->sign = 0;
		}
        else
		{
            sprintf(_buf,"Invalid sign calculation(=0)in node %d\n", P->index);
            error_mesg(INT_ERROR,_buf);
        }
    }
    else
	{
        P->sign = 1;
	}
   
    //printf("Init Node %d. real:%e, imag:%e\n",P->index, real(V),imag(V));
    return; 
}

/*----------------------------------------------------
Spec: Initialization for fully expanded DDD coefficient 
numerical operation and complex value memory allocation 
for each DDD node.
-----------------------------------------------------*/
void
DDDmanager::PreFCoeffCalculationR
(
	DDDnode * P,
	HasherTree *ht
) 
{

    if( P == zero || P == one)
	{
        return;
	}
   
	if(!ht->Put(*P,P))
	{// Already calculated 
		return;
	}

    if(!P->val_ptr) // need't to allocate space
	{        
		complex *val = new complex(0.0);
		P->val_ptr = val;
	}
	else
	{
		(*P->val_ptr) = complex(0,0);		
	}
	
    PreFCoeffCalculationR( P->Then(), ht );
    PreFCoeffCalculationR( P->Else(), ht );

    int sign;
    if(P->Then() != one)
	{
        if((sign = FCoeffCalcSign(P,P->Then())) > 0)
		{
            P->sign = 1;
		}
        else if( sign < 0 )
		{
            P->sign = 0;
		}
        else
		{
            sprintf(_buf,"Invalid sign calculation(=0)in node %d\n", P->index);
            error_mesg(INT_ERROR,_buf);
        }
    }
    else
	{
        P->sign = 1;
	}
        
    return ; 
}

/*----------------------------------------------------
    FuncName:
    Spec: Recursively calculate the determinant value
    represented by DDD given. The procedure take
    advantage of sharing of subexpression and
    all the node only need one visiting during
    all the calculation. 
 -----------------------------------------------------
    Inputs:
    Outputs:

    Global Variblas used:
----------------------------------------------------*/

complex
DDDmanager::GetDetermValueR
(
	DDDnode * P,
	double freq, 
	HasherTree *ht
)
{
	complex Czero(0);

    // the DDD node with 1 edge pointing to one has postive sign
    if( P == zero )
	{
        return complex(0);
	}
    else if( P == one)
	{
        return complex(1);
    }
    
    
	if(!ht->Put(*P,P))
	{// Already calculated 
		return *(P->val_ptr);
	}
   	
    ASSERT(P->val_ptr != NULL,"NULL complex value pointor");

    complex V;
    complex V_sub1,V_sub0;

    // visit DDDs in top-levle circuit
    if( P->sign )
	{
		if(label2elem->get_elem(P->index)->get_clist())
		{
			V = label2elem->get_elem(P->index)->elem_value(freq);
		}
		else
		{
			/* operator ^ is overloaded for rlc_complex and double
			   multiplication 
			*/
			V = (label2elem->get_elem(P->index)->get_cvalue() ^ freq) + Czero;
		}
	}
	
    else
	{
	
		if(label2elem->get_elem(P->index)->get_clist())
		{
			V = 0 - label2elem->get_elem(P->index)->elem_value(freq);
		}
		else
		{
			V = -((label2elem->get_elem(P->index)->get_cvalue() ^ freq) + Czero);
		}
	}
     

    if(V != complex(0.0))
	{
        V_sub1 = GetDetermValueR( P->Then(),freq, ht);
	}

    V_sub0 = GetDetermValueR( P->Else(),freq, ht); 

    *(P->val_ptr) = V = V * V_sub1 + V_sub0;

    return V; 
}    


/*----------------------------------------------------
    FuncName:
    Spec: Recursively calculate the coefficient value
    represented by DDD given. The procedure take
    advantage of sharing of subexpression and
    all the node only need one visit during
    all the calculation. 
 -----------------------------------------------------
    Inputs:DDD 
    Outputs:double value of the DDD coefficient.

    Global Variblas used:
----------------------------------------------------*/
double
DDDmanager::GetCoeffValueR
(
	DDDnode *P, 
	HasherTree *ht
)
{
    int    index;
    double V = 0.0; 
    double V_sub1,V_sub0;
    rcl_complex cval; 
    int sign;

    ASSERT(P,"Invalid DDD node(=NULL)");
    ASSERT(P->Else() != one,"Invalid DDD(P->Else() == one");

    // the DDD node with 1 edge pointing to one has postive sign
    if( P == zero )
	{
        return 0.0;
	}
    else if( P == one)
	{
        return 1.0;
	}

    if(!ht->Put(*P,P))
	{// Already calculated
        return real(*(P->val_ptr));
	}

    // Have not calculated yet

    ASSERT(P->val_ptr != NULL,"NULL complex value pointor");

    if(theLabMap->which_part(P->index) == ResPart)
	{
        index = theLabMap->coeff2ddd(P->index);
        cval = get_label2elem()->get_elem(index)->get_cvalue();
        V = cval.get_r();
	}
	else if(theLabMap->which_part(P->index) == CapPart)
	{
        index = theLabMap->coeff2ddd(P->index);
        cval = get_label2elem()->get_elem(index)->get_cvalue();
        V = cval.get_c();
	}
    else
	{// must be inductance part
        index = theLabMap->coeff2ddd(P->index);
        cval = get_label2elem()->get_elem(index)->get_cvalue();
        /* we negate the inductance part before */
        V = (-1)*cval.get_l();
    }

    V_sub1 = GetCoeffValueR( P->Then(),ht);
	V_sub0 = GetCoeffValueR( P->Else(),ht); 

    V = V * V_sub1;

    if( P->sign )
	{
		sign = 1;
	}
    else
	{
		sign = -1;
	}

    *(P->val_ptr) = V = V * sign + V_sub0;

    return V; 
}    

/*----------------------------------------------------
    FuncName:

    Spec: Recursively calculate the fully expanded coefficient value
    represented by DDD given. The procedure take advantage of sharing
    of subexpression and all the node only need one visit during all
    the calculation.
-----------------------------------------------------
    Inputs:DDD Outputs:double value of the DDD coefficient.

    Global Variblas used:
----------------------------------------------------*/

double
DDDmanager::GetFCoeffValueR
(
	DDDnode * P, 
	HasherTree *ht
)
{


	ASSERT(P,"Invalid DDD node(=NULL)");

	// the DDD node with 1 edge pointing to one has postive sign
	if( P == zero )
	{
		return 0.0;
	}
	else if( P == one)
	{
		return 1.0;
	}
     
	if(!ht->Put(*P,P)) 
	{// Already calculated
		return real(*(P->val_ptr));
	}

    // Have not calculated yet

	double V = 0.0; 
	double V_sub1,V_sub0;
	rcl_complex cval; 
	int sign;
	

	ASSERT(P->val_ptr != NULL,"NULL complex value pointor");
	
	DummBranch *dbch =  theFLabMap->get_dummbranch(P->index);
	assert(dbch);
	V = dbch->get_bvalue();	
	

	V_sub1 = GetFCoeffValueR(P->Then(), ht);
	V_sub0 = GetFCoeffValueR(P->Else(), ht); 

	V = V * V_sub1;

	if( P->sign )
	{
		sign = 1;
	}
	else
	{
		sign = -1;
	}

	*(P->val_ptr) = V = V * sign + V_sub0;

	return V; 
}    

/*----------------------------------------------------
Recursively sign calculation for DDD node
------------------------------------------------------*/
int
DDDmanager::CalcSign
(
	DDDnode * Parent, 
	DDDnode * Child 
)
{
    if(Child == one)
	{
        return 1;
	}

    int sign=1, sign1 = 1;
    int row_delta = label2elem->get_row(Parent->index) - 
		label2elem->get_row(Child->index);
    
	int col_delta = label2elem->get_col(Parent->index) -
		label2elem->get_col(Child->index);

    sign1 = row_delta*col_delta;

    if(!sign1)
	{
        printf("(1)Invalid sign calculation in node %d and node %d\n",
			   Parent->index, Child->index);
    }
    
	sign1 = sign1>0?1:-1;

    if(Child->Then() != one)
	{
        sign = CalcSign(Parent,Child->Then());
	}

    if(!sign)
	{
        printf("(2)Invalid sign calculation in node %d and node %d\n",
			   Parent->index, Child->index);
    }

    sign = sign>0?1:-1;

    return sign*sign1;
}

/*----------------------------------------------------
Recursively sign calculation for DDD coeficient node
------------------------------------------------------*/
int
DDDmanager::CoeffCalcSign
(
	DDDnode * Parent, 
	DDDnode * Child 
)
{
    if(Child == one)
	{
        return 1;
	}

    int pindex = theLabMap->coeff2ddd(Parent->index);
    int cindex = theLabMap->coeff2ddd(Child->index);
    
	int sign=1, sign1 = 1;
    
	int row_delta = label2elem->get_row(pindex) - 
		label2elem->get_row(cindex);
    int col_delta = label2elem->get_col(pindex) -
		label2elem->get_col(cindex);

    sign1 = row_delta*col_delta;

    if(!sign1)
	{
        printf("(1)Invalid sign calculation in ddd node %d and node %d\n",
			   pindex,cindex);
        printf("(1)Invalid sign calculation in coeff node %d and node %d\n",
			   Parent->index, Child->index);
    }

    sign1 = sign1>0?1:-1;

    if(Child->Then() != one)
	{
        sign = CoeffCalcSign(Parent,Child->Then());
	}

    if(!sign)
	{
        printf("(2)Invalid sign calculation in node %d and node %d\n",
			   pindex, cindex);
    }

    sign = sign>0?1:-1;

    return sign*sign1;
}

/*----------------------------------------------------
Recursively sign calculation for DDD coeficient node
------------------------------------------------------*/
int
DDDmanager::FCoeffCalcSign
(
	DDDnode * Parent, 
	DDDnode * Child 
)
{
    if(Child == one)
	{
        return 1;
	}

	int pindex = theFLabMap->get_complex_ddd_label(Parent->index);
	int cindex = theFLabMap->get_complex_ddd_label(Child->index);

	int sign=1, sign1 = 1;
    
	int row_delta = label2elem->get_row(pindex) - 
		label2elem->get_row(cindex);
    int col_delta = label2elem->get_col(pindex) -
		label2elem->get_col(cindex);

    sign1 = row_delta*col_delta;


    if(!sign1)
	{
        printf("(1)Invalid sign calculation in ddd node %d and node %d\n",
			   pindex,cindex);
        printf("(1)Invalid sign calculation in fcoeff node %d and node %d\n",
			   Parent->index, Child->index);

		cout <<"parent node: " << Parent << endl;
		cout <<"child node: " << Child << endl;		
    }
    
	sign1 = sign1>0?1:-1;

    if(Child->Then() != one)
	{
        sign = FCoeffCalcSign(Parent,Child->Then());
	}

    if(!sign)
	{
        printf("(2)Invalid sign calculation in node %d and node %d\n",
			   pindex, cindex);
    }
    
	sign = sign>0?1:-1;

    return sign*sign1;
}

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
 *    $RCSfile: polezero.cc,v $
 *    $Revision: 1.4 $
 *    $Date: 2002/11/21 02:40:15 $
 */


#include <limits.h>
 
#include "globals.h"
#include "ddd_base.h"
#include "control.h"
#include "circuits.h"
#include "node.h"
#include "branch.h"
#include "complex.h"
#include "rclcompx.h"
#include "mna_form.h"
#include "mxexp.h"
#include "dtstruct.h"
#include "approx.h"
#include "acan.h"
  
   
/* 
**    Extraction pole or zero based on multi-root DDDs
**    and pole splitting theory.
*/


#define _DEBUG

static int device_deleted;
static int num_factor_ddd;

void
Cofactor::factor_polezero
(
	MNAMatrix *matrix, 
	int pindex
)
{
    MNAElement *elem;
    int     max_label = matrix->get_num_elem();
    DDDnode *num_ddd, *den_ddd;
    double exa_val;
    double simp_val_num, simp_val_den, simp_val;
    int    i,lab,findex, num_branch;
    double  error;

    print_mesg("Factoring common DDD nodes in polezero extraction.");
    cout <<"pindex:"<<pindex<<endl;
    exa_val = fcoeff_list->get_cratio(pindex);

	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);

    for(lab=max_label; lab >= 1; lab--)
	{
        elem = ddd_mgr->get_label2elem()->get_elem(lab);

		// we only try the diagnonal element
		/*
		  if( elem->get_row()  != elem->get_col() )
		  continue;
        */

		if(elem->get_row() == row ||
		   elem->get_col() == col)
            continue;

        cout <<"index: "<<lab<<endl;

        num_ddd = fcoeff_list->get_fcoeff_ddd(pindex);
        den_ddd = fcoeff_list->get_fcoeff_ddd(pindex + 1);

        ddd_mgr->statistic(num_ddd);
        ddd_mgr->statistic(den_ddd);
        num_branch = elem->get_blist()->get_num();

        cout <<"num_branch:"<<num_branch<<endl;
        for(i=0;i<num_branch;i++)
		{
            //cout<<"try :"<<i<<endl;           
            
			BranchList *blist = ddd_mgr->get_label2elem()->get_elem(lab)->get_blist();
			DummBranch * dbch = blist->get_dummbranch(i);
			assert(dbch);
			findex = theFLabMap->get_flabel(dbch);

			num_ddd = ddd_mgr->Subset1(num_ddd,findex);
            den_ddd = ddd_mgr->Subset1(den_ddd,findex);
            if(num_ddd != ddd_mgr->Empty() &&
               den_ddd != ddd_mgr->Empty())
				break;
        }

        if(num_ddd == ddd_mgr->Empty() ||
           den_ddd == ddd_mgr->Empty() )
		{
            continue;
		}
        
        /* factor out the selected DDD to check
           the errors caused.
        */
        ddd_mgr->PreFCoeffCalculationR(num_ddd, prec_ht);
        ddd_mgr->PreFCoeffCalculationR(den_ddd, prec_ht);
        simp_val_num = ddd_mgr->GetFCoeffValueR(num_ddd,ht);
        simp_val_den = ddd_mgr->GetFCoeffValueR(den_ddd,ht);

        if(simp_val_den == 0.0)
            simp_val = 0.0;
        else
            simp_val = simp_val_num/simp_val_den;
        
        error = fabs(simp_val - exa_val)/fabs(exa_val);

#ifdef _DEBUG
        cout<<"exa_val:"<<exa_val<<" simp_val:"<<simp_val<<endl;
        cout <<"error:"<<error<<endl;
#endif
        if(error > opMagErr){
            continue;
        }

        cout <<"DDD node index: "<<lab<<" deleted"<<endl;
#ifdef _DEBUG
        cout <<"Branch:";
        elem->get_blist()->print_branch(cout,NUM);
        cout <<endl;
#endif

        fcoeff_list->set_fcoeff_ddd(num_ddd,pindex);
        fcoeff_list->set_fcoeff_ddd(den_ddd,pindex + 1);
#ifdef _DEBUG
        cout <<"for numerator"<<endl;
        ddd_mgr->statistic(num_ddd);

        cout <<"for denominator"<<endl;
        ddd_mgr->statistic(den_ddd);
        num_factor_ddd++;
        cout <<"# of ddd nodes factored out: "<<num_factor_ddd<<endl;
#endif
    }

	delete prec_ht;	
	delete ht;

    return;
}


/*    
**    Delete a circuit element with weak contribuation to the
**    both numerator and demonator of value. 
**    The element symbols may appear
**    four positions at most. For the element is the only
**    symbol a DDD node represents, then all the pathes
**    the DDD node  asscoiates are removed.
**    pindex: the pole/zero index;
*/

int
Cofactor::whole_elem_elim_polezero
(
	char *bname, 
	int pindex, 
	int ind1, int ind2, 
	int ind3, int ind4
)
{

    DDDnode *num_ddd,*den_ddd;
    double exa_val, simp_val, simp_val_num, simp_val_den;
    double error;
    MNAElement *elem1, *elem2, *elem3, *elem4;
    int    findex,bindex;

    elem1 = elem2 = elem3 = elem4 = NULL;

    exa_val = fcoeff_list->get_cratio(pindex);

    /* first , we temporaily delete the element in four 
       posoition by change the MNAelment value.
    */
    cout <<"Ind1: "<<ind1<<"Ind2: "<<ind2<<"Ind3: "<<ind3<<"Ind4: "<<ind4<<endl;
    num_ddd = fcoeff_list->get_fcoeff_ddd(pindex);
    den_ddd = fcoeff_list->get_fcoeff_ddd(pindex + 1);

    if(ind1 != -1)
	{
        elem1 = ddd_mgr->get_label2elem()->get_elem(ind1);
        bindex = elem1->get_blist()->get_branch_index(bname);
		
		DummBranch * dbch = elem1->get_blist()->get_dummbranch(bindex);
		assert(dbch);
		findex = theFLabMap->get_flabel(dbch);
 
        num_ddd = ddd_mgr->Subset0(num_ddd,findex);
        den_ddd = ddd_mgr->Subset0(den_ddd,findex);
    }
    if(ind2 != -1)
	{
        elem2 = ddd_mgr->get_label2elem()->get_elem(ind2);
        bindex = elem2->get_blist()->get_branch_index(bname);

		DummBranch * dbch = elem1->get_blist()->get_dummbranch(bindex);
		assert(dbch);
		findex = theFLabMap->get_flabel(dbch);
         
        num_ddd = ddd_mgr->Subset0(num_ddd,findex);
        den_ddd = ddd_mgr->Subset0(den_ddd,findex);
    }
    if(ind3 != -1)
	{
        elem3 = ddd_mgr->get_label2elem()->get_elem(ind3);
        bindex = elem3->get_blist()->get_branch_index(bname);
		
		DummBranch * dbch = elem1->get_blist()->get_dummbranch(bindex);
		assert(dbch);
		findex = theFLabMap->get_flabel(dbch);

		num_ddd = ddd_mgr->Subset0(num_ddd,findex);
        den_ddd = ddd_mgr->Subset0(den_ddd,findex);
    }
    if(ind4 != -1)
	{
        elem4 = ddd_mgr->get_label2elem()->get_elem(ind4);
        bindex = elem4->get_blist()->get_branch_index(bname);

		DummBranch * dbch = elem1->get_blist()->get_dummbranch(bindex);
		assert(dbch);
		findex = theFLabMap->get_flabel(dbch);
        
        num_ddd = ddd_mgr->Subset0(num_ddd,findex);
        den_ddd = ddd_mgr->Subset0(den_ddd,findex);
    }
    

	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);
    ddd_mgr->PreFCoeffCalculationR(num_ddd, prec_ht);
    ddd_mgr->PreFCoeffCalculationR(den_ddd, prec_ht);

    simp_val_num = ddd_mgr->GetFCoeffValueR(num_ddd,ht);
    simp_val_den = ddd_mgr->GetFCoeffValueR(den_ddd,ht);
	
	delete ht;
	delete prec_ht;
	

    if(simp_val_den == 0.0)
        simp_val = 0;
    else
        simp_val = simp_val_num/simp_val_den;
    
    error = fabs(simp_val - exa_val)/fabs(exa_val);

#ifdef _DEBUG
    cout<<"exa_val:"<<exa_val<<" simp_val:"<<simp_val<<endl;
    cout <<"error:"<<error<<endl;
#endif
    
    if(magn(exa_val - simp_val)/magn(exa_val) > opMagErr)
        return 0;

    device_deleted++;
    cout <<"After DDD reduction"<<endl;
    cout <<"#device deleted"<<device_deleted<<endl;

    fcoeff_list->set_fcoeff_ddd(num_ddd,pindex);
    fcoeff_list->set_fcoeff_ddd(den_ddd,pindex + 1);            

#ifdef _DEBUG
    cout <<"for numerator"<<endl;
    ddd_mgr->statistic(num_ddd);

    cout <<"for denominator"<<endl;
    ddd_mgr->statistic(den_ddd);
#endif
    return 1;
}

/*
** find four positions of admittance for each device
*/
void
Circuit::whole_elem_elim_polezero
( 
	int pindex 
)
{
    Branch *blaux;
    int    n1,n2,n3,n4;
    int    ind1, ind2, ind3, ind4;
    if(!rblist)
        return;

    print_mesg("Whole device elimination for both ... ");

    /* we through all the branches */
    for(blaux = rblist; blaux; blaux = blaux->Next())
	{
        if(blaux->get_stat() == B_DELETED)
            continue;
        n1 = n2 = n3 = n4 = -1;
        ind1 = ind2 = ind3 = ind4 = -1;

        if(blaux->get_type() == dRES ||
           blaux->get_type() == dCAP ||
           blaux->get_type() == dIND )
		{

			if(blaux->get_node1()->get_real_num())
                n1 = blaux->get_node1()->get_index();

			if(blaux->get_node2()->get_real_num())
				n2 = blaux->get_node2()->get_index();

            if(n1 == -1 && n2 == -1)
                continue;

#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()<<
				" n1:"<<n1<<" n2:"<<n2<<endl;
#endif
            /*    n1    n2 
				  n1    ind1    ind2
				  n2    ind3    ind4
            */
            if(n1 != -1)
			{
                ind1 = matrix->get_label(n1,n1);
                //cout <<"ind1: "<<ind1<<endl;
            }
            if(n1 != -1 && n2 != -1)
			{
                ind2 = matrix->get_label(n1,n2);
                ind3 = matrix->get_label(n2,n1);
                //cout <<"ind2: "<<ind2<<endl;
                //cout <<"ind3: "<<ind3<<endl;
            }
            if(n2 != -1)
			{
                ind4 = matrix->get_label(n2,n2);
                //cout <<"ind4: "<<ind4<<endl;
            } 
            if(ac_mgr->whole_elem_elim_polezero(blaux->get_name(),
												pindex, ind1, ind2, ind3, ind4))
			{
                blaux->get_stat() = B_DELETED;
			}
        }
        else if( blaux->get_type() == dVCCS ) 
		{
            if(blaux->get_node1()->get_real_num())
                n1 = blaux->get_node1()->get_index();
			if(blaux->get_node2()->get_real_num())
                n2 = blaux->get_node2()->get_index();
			if(blaux->get_node3()->get_real_num())
                n3 = blaux->get_node3()->get_index();
			if(blaux->get_node4()->get_real_num())
                n4 = blaux->get_node4()->get_index();

#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1<<" n1:"
				<<n2<<" n3:"<<n3<<" n4:"
				<<n4<<endl;
#endif

            /*        n3    n4 
					  n1    ind1    ind2
					  n2    ind3    ind4
            */
            if(n1 != -1 && n3 != -1)
			{
                ind1 = matrix->get_label(n1,n3);
                //cout <<"ind1: "<<ind1<<endl;
            }
            if(n1 != -1 && n4 != -1)
			{
                ind2 = matrix->get_label(n1,n4);
                //cout <<"ind2: "<<ind2<<endl;
            }
            if(n2 != -1 && n3 != -1)
			{
                ind3 = matrix->get_label(n2,n3);
                //cout <<"ind3: "<<ind3<<endl;
            }
            if(n2 != -1 && n4 != -1)
			{
                ind4 = matrix->get_label(n2,n4);
                //cout <<"ind4: "<<ind4<<endl;
            }

            if(ac_mgr->whole_elem_elim_polezero(
				blaux->get_name(),
				pindex, ind1, ind2, ind3, ind4
			))
			{
                blaux->get_stat() = B_DELETED;
			}
        }
    }
    print_mesg("End of whole device elimination for both");
}

int
ACAnalysisManager::whole_elem_elim_polezero
(
    char *bname, 
	int pindex, 
	int ind1, int ind2, 
	int ind3, int ind4
)
{
	Cofactor    *num_cof, *den_cof;
	if(!poly)
	{
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return 0;
	}
         
    // we assume the fcoefficient has been obtained a priori 
	den_cof = poly->den_list;
	num_cof = poly->num_list;
    if(!den_cof->get_fcoeff_list())
	{
        error_mesg(INT_ERROR,"No fcoefficient list existing.");
        return 0;
    }

    // first suppress the unused fcoefficient
    den_cof->get_fcoeff_list()->fcoeff_suppress(pindex);
    num_cof->get_fcoeff_list()->fcoeff_supp_init();
    /* now, we only consider pole */
    if(den_cof->get_fcoeff_list()->get_cratio(pindex) == 0.0)
        den_cof->get_fcoeff_list()->calc_cratio(pindex);

    return den_cof->whole_elem_elim_polezero(bname, 
											 pindex,ind1, ind2, ind3, ind4);
}

void
ACAnalysisManager::factor_polezero(int pindex)
{
	Cofactor    *num_cof, *den_cof;
	if(!poly)
	{
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return;
	}

    /* now, we only consider pole */
	den_cof = poly->den_list;
	num_cof = poly->num_list;
	if(!den_cof->get_fcoeff_list())
	{
		error_mesg(INT_ERROR,"No fcoefficient list existing.");
		return;
	}

	// first suppress the unused fcoefficient
	den_cof->get_fcoeff_list()->fcoeff_suppress(pindex);
	num_cof->get_fcoeff_list()->fcoeff_supp_init();
    
    /* now, we only consider pole */
	if(den_cof->get_fcoeff_list()->get_cratio(pindex) == 0.0)
		den_cof->get_fcoeff_list()->calc_cratio(pindex);

	den_cof->factor_polezero(matrix, pindex);
}

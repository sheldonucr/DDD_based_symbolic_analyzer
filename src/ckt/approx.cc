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
 *    $RCSfile: approx.cc,v $
 *    $Revision: 1.13 $
 *    $Date: 2002/11/21 02:40:15 $
 */

#define _DEBUG 0

/*
 * approx.c:
 *  Perform the approximation under the give tolenrance * after the
 *  DDD has been constructed. We assume the * given DDD is the
 *  coefficient DDD of different powers of s.  * Our method is to
 *  generate the most significant terms * from DDD in the
 *  nonincreasing order by simple DFS traversal.  * Post-symbolic
 *  processes are required to allow for the term * cancellation and
 *  matching of circuit elements.
 *
 *  As to the error control, we monitor the magnitude and phase of the
 *  transfer functions for a range of frequency of interest.
 *
 *       X.Tan  Feb. 6, 1998
 *       Electrical and Engineering Dept., University of Iowa
 * */

/* INCLUDES: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


DDDstack *theTopoList;
DDDstack *theCurPath;

//#define MAX_PATH_COUNT 10000
//#define MAX_PATH_COUNT_PER_COEFF 2000

#define MAX_PATH_COUNT 1e20
#define MAX_PATH_COUNT_PER_COEFF 1e20

/* FUNCTIONS */
extern void statistic_info(DDDnode * Z);
extern void print_permutation(int *list, int range);

/* Function definitions */
complex CofactorValue(DDDnode *ddd_aux);

/* Perform the topological sorting on the DDD and initialize the DDD
** for DFS traversal. This function combine the topological sorting
** and Initialize-single-source step.  
*/

void
DDDmanager::DFS_ISS
(
	DDDnode *s
)
{
    if(!s)
	{
        return;
	}

    s->color = dBLACK;
    if(!s->val_ptr)
	{
        s->val_ptr = new complex(0.0);
	}

    s->val_ptr[0] = INF_WEIGHT;
    s->pi = NULL;
	if((s == zero) || (s == one))
	{
        return;
	}
    if(s->Then()->color == dWHITE)
	{
        DFS_ISS(s->Then());
    }
    if(s->Else()->color == dWHITE)
	{
        DFS_ISS(s->Else());
    }
	assert(theTopoList);
    theTopoList->DDDpush(s);
}


/* restore the colors of all the DDD nodes to the 'white' for further
    DFS.  
*/
void
DDDmanager::DFS_Restore
(
	DDDnode *s
)
{
    if(!s)
	{
        return;
	}

    s->color = dWHITE;
    //printf("Res node: %d\t",s->index);
    if(s == zero || s == one)
	{
        return;
	}

    if((int)(s->Then()->color) == dBLACK)
	{
        DFS_Restore(s->Then());
	}
    if((int)(s->Else()->color) == dBLACK)
	{
        DFS_Restore(s->Else());
	}
}

/* Relax the edges origniate from u to v.  In DDDs, the weight of
   1-edge is -log(value) and 0 for 0-edge 
*/

void
DDDmanager::Relax
(
	DDDnode *u, 
	DDDnode *v, 
	double freq
)
{

    complex cval, Czero =0.0;
    double val;
    
	if(!u || !v)
	{
        return;
	}
    
    if(v == zero)// we igore all the edge pointing to 0-terminal
	{
        return;
	}

    cval = (label2elem->get_elem(u->index)->get_cvalue() ^ freq) + Czero;
    val = 0 - log10(magn(cval));

    if(u->Then() == v)
	{ // 1-edge
        if( real(v->val_ptr[0]) > (real(u->val_ptr[0]) + val) )
		{
            v->val_ptr[0] = real(u->val_ptr[0]) + val;
            v->pi = u;
        }	
    }
    else 
	{ // 0-edge
        // 0.0 for 0-edge
        if( real(v->val_ptr[0]) > real(u->val_ptr[0]) + 0.0 )
		{
            v->val_ptr[0] = real(u->val_ptr[0]);
            v->pi = u;
        }
					
    }
}

/* Relax the edges origniate from u to v.  In DDDs, the weight of
    1-edge is -log(value) and 0 for 0-edge The function is used for
    fully expanded coefficient DDD.  
*/

void
DDDmanager::FCRelax
(
	DDDnode *u, 
	DDDnode *v
)
{

    double val;
	double V = 0.0;
	rcl_complex cval;

    if(!u || !v)
	{
        return;
	}
    
    if( v == zero)// we igore all the edge pointing to 0-terminal
	{
		return;
	}

	
	if(u->Then() == v)
	{ // 1-edge
		
		DummBranch *dbch =  theFLabMap->get_dummbranch(u->index);
		assert(dbch);
		V = dbch->get_bvalue();	
		val = 0 - log10(fabs(V));
    
#if _DEBUG
		cout <<"try to relax v: " 
			 << v->index << " " << (void *)v 
			 << "-> u: " << u->index 
			 << " " << (void *)u << endl;
		cout <<"v.val: " << real(v->val_ptr[0]) << " u+val: " 
			 << real(u->val_ptr[0]) + val << endl;
#endif
        if( real(v->val_ptr[0]) > (real(u->val_ptr[0]) + val) )
		{
            v->val_ptr[0] = real(u->val_ptr[0]) + val;
            v->pi = u;
#if _DEBUG
			cout <<"v: " << v->index << "-> u: " << u->index << endl;
#endif
        }		

    }
    else 
	{ // 0-edge
        // 0.0 for 0-edge
#if _DEBUG
		cout <<"try to relax v: " << v->index 
			 << (void *)v << "-> u: " 
			 << u->index << " " << (void *)u << endl;
		cout <<"v.val: " << real(v->val_ptr[0]) << " u: " 
			 << real(u->val_ptr[0]) << endl;
#endif

        if( real(v->val_ptr[0]) > real(u->val_ptr[0]) + 0.0 )
		{
            v->val_ptr[0] = real(u->val_ptr[0]);
            v->pi = u;
#if _DEBUG
			cout <<"v: " << v->index << "-> u: " << u->index << endl;
#endif
        }
    }
}
        
/* Find the shortest path from root s to the 1-terminal in the given
    DDD.  It returns the found path in DDD form.  
*/

DDDnode *
DDDmanager::DDDShortestPath
(
	DDDnode *s, 
	double freq
)
{
    DDDnode *ddd_aux;
    if(s == zero)
	{
        return zero;
	}	

	DFS_ISS(s); // topological sorting and source initialization.
    s->val_ptr[0] = 0.0;
    if(!theTopoList->isEmpty())
	{ // we have result
        //printf("stack size: %d\n", theTopoList->StackSize());
        while((ddd_aux = theTopoList->DDDpop()) != NULL)
		{
            //printf("node:%d\t",ddd_aux->index);
            Relax(ddd_aux, ddd_aux->Then(), freq);
            Relax(ddd_aux, ddd_aux->Else(), freq);
        }
    }

    DFS_Restore(s); // color all the DDDnodes to `white`.
    
	return ExtractPath();
}

/* Find the shortest path from root s to the 1-terminal in the given
    DDD.  It returns the found path in DDD form.  It is used for fully
    expanded coefficient DDD.  
*/

DDDnode *
DDDmanager::FCDDDShortestPath
(
	DDDnode *s
)
{
    DDDnode *ddd_aux;
    if(s == zero)
	{
        return zero;
	}

#ifdef __USE_THIRD_TERM_GENERATION 
	cout <<"Initlize for 2nd shortest path term generation method ..." 
		 << endl;
	DFS_ISS_2(s);
#else
    DFS_ISS(s); // topological sorting and source initialization.
#endif
  
    s->val_ptr[0] = 0.0;
    if(!theTopoList->isEmpty())
	{ 
        while((ddd_aux = theTopoList->DDDpop()) != NULL)
		{
            //printf("node:%d\t",ddd_aux->index);
            FCRelax(ddd_aux, ddd_aux->Then());
            FCRelax(ddd_aux, ddd_aux->Else());
        }
    }
    DFS_Restore(s); // color all the DDDnodes to `white`.
    
	return ExtractPath();
}
        
        
/*  we traverse from 1-terminal to the root node which is indicated by
    NULL pi field.  
*/
DDDnode *
DDDmanager::ExtractPath()
{
    DDDnode *path = Base();
    DDDnode *ddd_aux = one;
    if(!ddd_aux)
	{
        return NULL;
	}

    for(;ddd_aux->pi;ddd_aux = ddd_aux->pi)
	{
        if(ddd_aux->pi->Then() == ddd_aux) //check it is 1-edge
		{
            path = Change(path,ddd_aux->pi->index);
		}
    }

    return path;
}

/* We assume only one path exists in the DDD We also only compute the
   magnitude of the product term 
*/
complex 
DDDmanager::PathValue
(
	DDDnode *path, 
	double freq
)
{
    complex Czero = 0.0;
    DDDnode *aux = path;
    complex eval, pval = 1.0;
    double    sign = 1.0;

    if(!aux)
	{
        return 0;
	}

    if(aux == zero)
	{
        return 0;
	}

    printf("path:[");

    /* first calculate sign of the path */

    for(; aux != one; aux = aux->Then())
	{
        sign *= CalcSign(aux, aux->Then());
        eval = (label2elem->get_elem(aux->index)->get_cvalue() ^ freq) + Czero;
        pval = pval * eval;
        printf(" %d",aux->index);
    }
    pval = pval*sign;
    printf(" ] val: %g, logval: %g\n",magn(pval),real(one->val_ptr[0]));
    
	return pval;
}

/* We assume only one path exists in the DDD We also only compute the
   magnitude of the product term.  It is used for fully expanded
   coeficient DDDs.  
*/
double 
DDDmanager::FCPathValue
(
	DDDnode *path,
	bool compute_sign // if we need to compute sign or not.
)
{
    DDDnode *aux = path;
    double eval, pval = 1.0;
    double    sign=1.0;

    if(!aux)
	{
        return 0;
	}
    if(aux == zero)
	{
        return 0;
	}

    //printf("path:[");
    for(; aux != one; aux = aux->Then())
	{
		if(compute_sign)
		{
			sign *= FCoeffCalcSign(aux, aux->Then());
		}

		DummBranch *dbch =  theFLabMap->get_dummbranch(aux->index);
		assert(dbch);
		eval = dbch->get_bvalue();	
        pval = pval * eval;
        // printf("(%d %g) ",aux->index, eval);
    }
    pval = pval*sign;
    //printf(" ] val: %g, logval: %g\n",pval,real(one->val_ptr[0]));
    //printf(". ");
    
	return pval;
}


/* Generate all the product terms with magnitude greate than the
   reference magnitude given at given at given frequency point.  
*/
void
Cofactor::simp_cofactor
(
	double _freq, 
	double mag_ref
)
{
    DDDnode *msp_ddd;
    double path_val;

    /* go find all the product terms with magnitude  
       greater than  ref_mag.
    */
    //print_mesg("Before simplification");
    printf("ref_mag: %g\n",mag_ref);
    //ddd_mgr->statistic(sddd);

    if(sddd == ddd_mgr->Empty())
	{
        //print_mesg("No product terms found.");
        return;
    }
    while(1)
	{
        msp_ddd = ddd_mgr->DDDShortestPath(sddd,_freq);
        path_val = fabs(real(ddd_mgr->PathValue(msp_ddd, _freq)));
        if(path_val < mag_ref)
		{
            break;
		}

        // build the new DDD */
        sddd = ddd_mgr->Union(sddd,msp_ddd);

        // remove the shortest path from old DDD 
        sddd = ddd_mgr->Diff(sddd,msp_ddd);

        if(sddd == ddd_mgr->Base() || sddd == ddd_mgr->Empty())
		{
            break;
		}
    }
    //print_mesg("End of cofactor simplification\n");
    //ddd_mgr->statistic(sddd);
}

/* Generate all the product terms with magnitude greate than the
    reference magnitude.  We assume the sfcoefficient has been
    initialized.  It is used for fully expanded coefficient DDDs.  
*/
void
Cofactor::simp_fcofactor_method1
(
	double _freq, 
	double mag_ref
)
{

    int i, max_order;
    FCoeff * fcoeff, *sfcoeff;
    DDDnode *fcddd, *sfcddd, *msp_ddd,*ddd_aux, *prev_ddd;
    double path_val;
    register double nfreq = 1.0;
	static int new_path_count = 0;
   
    /* go find all the product terms with magnitude  
       greater than  rel_ref*value for each fcoefficient DDD.
    */

	if(_DEBUG)
	{
		cout <<"mag_ref: "<<mag_ref<<endl;
	}

    if(!fcoeff_list)
	{
        error_mesg(INT_ERROR,"Not fcoefficient list found.");
        return;
    }

	int offset = fcoeff_list->get_offset();
	
    /*	 
	  if(!sfcoeff_list)
	      sfcoeff_list = new FCoeffList(ddd_mgr, offset);
    */
    if(!sfcoeff_list)
	{
        sfcoeff_list = new FCoeffList(ddd_mgr, offset);
	}

    sfcoeff_list->reset_fcoeff_calced();

    max_order = fcoeff_list->get_max_order();
	for(i = 0; i < max_order; nfreq = nfreq * _freq,i++)
	{

		if(_DEBUG)
		{
			cout <<"order: "<<i
				 <<", nfreq: "<<nfreq<<endl;
		}

        fcoeff = fcoeff_list->get_fcoeff(i);
        fcddd = fcoeff_list->get_fcoeff_ddd(i);
        sfcoeff = sfcoeff_list->get_fcoeff(i);
        sfcddd = sfcoeff_list->get_fcoeff_ddd(i);


        if(fcddd == ddd_mgr->Empty())
		{
            //print_mesg("No product terms found.");
            //ddd_mgr->statistic(fcddd);
            continue;
        }
        else if(!fcoeff->keep_val())
		{
            cout<<"suppressed, ignored"<<endl;
            //ddd_mgr->statistic(fcddd);
            continue;
        }

        if(fcoeff->Prev_path() && 
		   (fabs(fcoeff->Prev_pval())*nfreq < mag_ref))
		{
            //ddd_mgr->statistic(fcddd);

			if(_DEBUG)
			{
				cout<<" order: "<<i<<endl;
				cout<<" mag_ref:"<<mag_ref
					<<" path_val:"<<fabs(fcoeff->Prev_pval())
					<<" total path_val:"<<fabs(fcoeff->Prev_pval())*nfreq 
					<< endl;
				cout<<"nfreq:" <<nfreq<<"--->skip!!!"<<endl;
			}

            continue;
        }
            
        theCurPath->ClearStack();
        double tpath_val;
        prev_ddd = NULL;
		int path_count_coeff = 0;

        while(1)
		{
            msp_ddd = ddd_mgr->FCDDDShortestPath(fcddd);

			if(msp_ddd)
			{
				//ddd_mgr->fcoeff_print_symbolic_path(msp_ddd, cout);	

				new_path_count++;
				path_count_coeff++;
				if((path_count_coeff  >= MAX_PATH_COUNT_PER_COEFF)
				   || (new_path_count > MAX_PATH_COUNT) )
				{
					break;
				}
			}

			bool compute_sign = false;
            path_val = ddd_mgr->FCPathValue(msp_ddd, compute_sign);
            tpath_val = path_val*nfreq;

            if(fabs(tpath_val) < mag_ref)
			{
                fcoeff->Prev_path() = msp_ddd;
                fcoeff->Prev_pval() = path_val;

				if(_DEBUG)
				{
					cout<<" order: "<<i<<endl;
					cout<<" mag_ref:"<<mag_ref
						<<" path_val:"<<path_val
						<<" total path_val:"<<tpath_val
						<<endl;
					cout<<"break"<<endl;
				}

                while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}

                break;
            }          

			// first remove it from old DDD
            fcddd = ddd_mgr->Diff(fcddd,msp_ddd);
		

//#define __ENABLE_DYNAMICAL_DECANCELLATION
#ifdef __ENABLE_DYNAMICAL_DECANCELLATION

			double sum, rdiff;
			
            if(!fcoeff->Prev_path()) 
			{
                // first time

				if(_DEBUG)
				{
					cout<<"new path DDDpush"<<endl;
				}

                fcoeff->Prev_path() = msp_ddd;
                fcoeff->Prev_pval() = path_val;
                theCurPath->DDDpush(msp_ddd);
                continue;
            }

            sum = path_val + fcoeff->Prev_pval();
            rdiff = sum/path_val;

			if(_DEBUG)
			{
				cout <<"rdiff:"<<rdiff<<endl;
			}

            if( fabs(rdiff) < 1e-6 && (ddd_aux = theCurPath->DDDpop()))
			{

				if(_DEBUG)
				{
					cout<<"Path cancelled\n"<<endl;
				}

                if(!prev_ddd)
				{
                    prev_ddd = msp_ddd;
				}

				if(_DEBUG)
				{
					ddd_mgr->fcoeff_print_symbolic_path(prev_ddd,cout);
					ddd_mgr->fcoeff_print_symbolic_path(ddd_aux,cout);
				}

                if(theCurPath->isEmpty())
				{
                    fcoeff->Prev_path() = NULL;
                    fcoeff->Prev_pval() = 0.0;
                    fcddd = fcoeff_list->fcoeff_dyn_decancel
						(
							fcddd,
							prev_ddd, ddd_aux
						);
                    prev_ddd = NULL;
                }
                continue;
            }
            else if(fabs(rdiff - 2.0) < 1e-6)
			{
                theCurPath->DDDpush(msp_ddd);

				if(_DEBUG)
				{
					cout<<"Path DDDpushed.\n"<<endl;
				}

                continue;
            }

#endif  //__ENABLE_DYNAMICAL_DECANCELLATION

            while((ddd_aux = theCurPath->DDDpop()))
			{
                sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
			}
			
            prev_ddd = NULL;
            fcoeff->Prev_path() = msp_ddd;
            fcoeff->Prev_pval() = path_val;
            theCurPath->DDDpush(msp_ddd);

            if(fcddd == ddd_mgr->Base() || 
			   fcddd == ddd_mgr->Empty())
			{
                while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}
                break;
            }
		   
        }

        fcoeff->set_ddd(fcddd);
		if(sfcddd != sfcoeff->get_ddd())
		{			
			sfcoeff->set_ddd(sfcddd);

			// do the GC to reduce memory (for shortest path method) 		
			
			if(theCkt)
			{
				cout <<"number of terms generated: " << new_path_count << endl;
				theCkt->ac_mgr->forceGCForAll();
			}			
		}

		if(new_path_count >= MAX_PATH_COUNT)
		{			
			cout <<"Max number of paths (" << new_path_count 
				 << ") is reached" << endl;

			break;
		}
    }

	cout <<"number of terms generated: " << new_path_count << endl;
}

#ifdef __USE_SECOND_TERM_GENERATION

/* Use dynamic programming method to generate dominant terms.  
** 
**/
void
Cofactor::simp_fcofactor_method2
(
	double _freq, 
	double mag_ref
)
{

    int i, max_order;
    FCoeff * fcoeff, *sfcoeff;
    DDDnode *fcddd, *sfcddd, *msp_ddd,*ddd_aux, *prev_ddd;
    double path_val;
    register double nfreq = 1.0;
	static int new_path_count = 0;
   
    /* go find all the product terms with magnitude  
       greater than  rel_ref*value for each fcoefficient DDD.
    */



	if(_DEBUG)
	{
		cout <<"mag_ref: "<<mag_ref<<endl;
	}

    if(!fcoeff_list)
	{
        error_mesg(INT_ERROR,"Not fcoefficient list found.");
        return;
    }

    
	int offset = fcoeff_list->get_offset();
    /*	 
	  if(!sfcoeff_list)
	      sfcoeff_list = new FCoeffList(ddd_mgr,offset);
    */
    if(!sfcoeff_list)
	{
        sfcoeff_list = new FCoeffList(ddd_mgr,offset);
	}

    sfcoeff_list->reset_fcoeff_calced();
	
	max_order = fcoeff_list->get_max_order();
	for(i = 0; i < max_order; nfreq = nfreq * _freq,i++)
	{		

		if(_DEBUG)
		{
			cout <<"order: "<<i
				 <<", nfreq: "<<nfreq<<endl;
		}

        fcoeff = fcoeff_list->get_fcoeff(i);
        fcddd = fcoeff_list->get_fcoeff_ddd(i);
        sfcoeff = sfcoeff_list->get_fcoeff(i);
        sfcddd = sfcoeff_list->get_fcoeff_ddd(i);


        if(fcddd == ddd_mgr->Empty())
		{
            //print_mesg("No product terms found.");
            //ddd_mgr->statistic(fcddd);
            continue;
        }
        else if(!fcoeff->keep_val())
		{
            cout<<"suppressed, ignored"<<endl;
            //ddd_mgr->statistic(fcddd);
            continue;
        }

		if(!fcddd->get_terms_stack())
		{
			ddd_mgr->FCSecondInitialize(fcddd);
		}

        if(fcoeff->Prev_path() && 
		   (fabs(fcoeff->Prev_pval())*nfreq < mag_ref))
		{
            //ddd_mgr->statistic(fcddd);

			if(_DEBUG)
			{
				cout<<" order: "<<i<<endl;
				cout<<" mag_ref:"<<mag_ref
					<<" path_val:"<<fabs(fcoeff->Prev_pval())
					<<" total path_val:"<<fabs(fcoeff->Prev_pval())*nfreq 
					<< endl;
				cout<<"nfreq:" <<nfreq<<"--->skip!!!"<<endl;
			}

            continue;
        }
            
        theCurPath->ClearStack();
        double tpath_val;
        prev_ddd = NULL;
		int path_count_coeff = 0;

        while(1)
		{

			msp_ddd = ddd_mgr->FCSecondGetOneDominantTerm(fcddd);


			if(msp_ddd)
			{
				//ddd_mgr->fcoeff_print_symbolic_path(msp_ddd,cout);	

				new_path_count++;
				path_count_coeff++;
				if((path_count_coeff  >= MAX_PATH_COUNT_PER_COEFF)
				   || (new_path_count > MAX_PATH_COUNT) )
				{
					break;
				}
			}

			bool compute_sign = false;
            path_val = ddd_mgr->FCPathValue(msp_ddd, compute_sign);
            tpath_val = path_val*nfreq;

            if(fabs(tpath_val) < mag_ref)
			{
                fcoeff->Prev_path() = msp_ddd;
                fcoeff->Prev_pval() = path_val;

				if(_DEBUG)
				{
					cout<<" order: "<<i<<endl;
					cout<<" mag_ref:"<<mag_ref
						<<" path_val:"<<path_val
						<<" total path_val:"<<tpath_val
						<<endl;
					cout<<"break"<<endl;
				}

                while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}

                break;
            }         		

            while((ddd_aux = theCurPath->DDDpop()))
			{
                sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
			}
			
            prev_ddd = NULL;
            fcoeff->Prev_path() = msp_ddd;
            fcoeff->Prev_pval() = path_val;
            theCurPath->DDDpush(msp_ddd);

			if(!msp_ddd)
			{
				 while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}
                break;
			}
		   
        }

        fcoeff->set_ddd(fcddd);
		if(sfcddd != sfcoeff->get_ddd())
		{			
			sfcoeff->set_ddd(sfcddd);
			cout <<"number of terms generated: " << new_path_count << endl;
		}

		if(new_path_count >= MAX_PATH_COUNT)
		{			
			cout <<"Max number of paths (" << new_path_count 
				 << ") is reached" << endl;

			break;
		}
    }

	cout <<"number of terms generated: " << new_path_count << endl;
}
#endif  //__USE_SECOND_TERM_GENERATION 

#ifdef  __USE_THIRD_TERM_GENERATION 

/* The second shortest path generation algorithm (third path
** generation method) We use dynamic shortest path method.
** i.e. Instead of relaxing all the vertices each time, we only relax
** partial portiion of vertices to speedup the the term generation.
**/
void
Cofactor::simp_fcofactor_method3
(
	double _freq, 
	double mag_ref
)
{

    int i, max_order;
    FCoeff * fcoeff, *sfcoeff;
    DDDnode *fcddd, *sfcddd, *msp_ddd,*ddd_aux, *prev_ddd;
    double path_val;
    register double nfreq = 1.0;
	static int new_path_count = 0;
   
    /* go find all the product terms with magnitude  
       greater than  rel_ref*value for each fcoefficient DDD.
    */

	if(_DEBUG)
	{
		cout <<"mag_ref: "<<mag_ref<<endl;
	}

    if(!fcoeff_list)
	{
        error_mesg(INT_ERROR,"Not fcoefficient list found.");
        return;
    }

	int offset = fcoeff_list->get_offset();
    max_order = fcoeff_list->get_max_order();
    
    if(!sfcoeff_list)
	{
        sfcoeff_list = new FCoeffList(ddd_mgr,offset);
	}

    sfcoeff_list->reset_fcoeff_calced();

	for(i = 0; i < max_order; nfreq = nfreq * _freq,i++)
	{		

		//if(_DEBUG)		
		if(1)
		{
			cout <<"order: "<<i
				 <<", nfreq: "<<nfreq<<endl;
		}

        fcoeff = fcoeff_list->get_fcoeff(i);
        fcddd = fcoeff_list->get_fcoeff_ddd(i);
        sfcoeff = sfcoeff_list->get_fcoeff(i);
        sfcddd = sfcoeff_list->get_fcoeff_ddd(i);


        if(fcddd == ddd_mgr->Empty())
		{
            //print_mesg("No product terms found.");
            //ddd_mgr->statistic(fcddd);
            continue;
        }
        else if(!fcoeff->keep_val())
		{
            cout<<"suppressed, ignored"<<endl;
            //ddd_mgr->statistic(fcddd);
            continue;
        }

        if(fcoeff->Prev_path() && 
		   (fabs(fcoeff->Prev_pval())*nfreq < mag_ref))
		{
            //ddd_mgr->statistic(fcddd);

			if(_DEBUG)
			{
				cout<<" order: "<<i<<endl;
				cout<<" mag_ref:"<<mag_ref
					<<" path_val:"<<fabs(fcoeff->Prev_pval())
					<<" total path_val:"<<fabs(fcoeff->Prev_pval())*nfreq 
					<< endl;
				cout<<"nfreq:" <<nfreq<<"--->skip!!!"<<endl;
			}

            continue;
        }
            
        theCurPath->ClearStack();
        double tpath_val;
        prev_ddd = NULL;
		int path_count_coeff = 0;

        while(1)
		{		
			//if it is the first term
			if(path_count_coeff == 0)
			{
				// for the first term, we call tranditional shortest
				// path method to find it.
				ddd_mgr->FCDDDShortestPath(fcddd);

				// get the path and remove it from fcddd
				msp_ddd = ddd_mgr->FCDDDShortestPath_2(&fcddd);
			}
			else
			{
				msp_ddd = ddd_mgr->FCDDDShortestPath_2(&fcddd);
			}

			if(msp_ddd)
			{
				// ddd_mgr->fcoeff_print_symbolic_path(msp_ddd,cout);	
				
				new_path_count++;				
				path_count_coeff++;
				if((path_count_coeff  >= MAX_PATH_COUNT_PER_COEFF)
				   || (new_path_count > MAX_PATH_COUNT) )
				{
					break;
				}
			}

			bool compute_sign = false;
            path_val = ddd_mgr->FCPathValue(msp_ddd, compute_sign);
            tpath_val = path_val*nfreq;

            if(fabs(tpath_val) < mag_ref)
			{
                fcoeff->Prev_path() = msp_ddd;
                fcoeff->Prev_pval() = path_val;

				if(_DEBUG)
				{
					cout<<" order: "<<i<<endl;
					cout<<" mag_ref:"<<mag_ref
						<<" path_val:"<<path_val
						<<" total path_val:"<<tpath_val
						<<endl;
					cout<<"break"<<endl;
				}

                while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}

                break;
            }
           		
            while((ddd_aux = theCurPath->DDDpop()))
			{
                sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
			}
			
            prev_ddd = NULL;
            fcoeff->Prev_path() = msp_ddd;
            fcoeff->Prev_pval() = path_val;
            theCurPath->DDDpush(msp_ddd);


            if(fcddd == ddd_mgr->Base() || 
			   fcddd == ddd_mgr->Empty())
			{
                while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}
                break;
            }
		   
        }

        fcoeff->set_ddd(fcddd);
		if(sfcddd != sfcoeff->get_ddd())
		{			
			sfcoeff->set_ddd(sfcddd);			
			
			if(theCkt)
			{
				cout <<"number of terms generated: " << new_path_count << endl;
				theCkt->ac_mgr->forceGCForAll();
			}
			
			
		}

		if(new_path_count >= MAX_PATH_COUNT)
		{			
			cout <<"Max number of paths (" << new_path_count 
				 << ") is reached" << endl;

			break;
		}
    }

	cout <<"number of terms generated: " << new_path_count << endl;
}

#endif  //__USE_THIRD_TERM_GENERATION 


#ifdef  __USE_FOURTH_TERM_GENERATION 


/* The third shortest path generation algorithm (fourth path
** generation method) We use dynamic shortest path method.
** i.e. Instead of relaxing all the vertices each time, we only relax
** newly added vertices (guarantteed) to speedup the the term
** generation.
**/
void
Cofactor::simp_fcofactor_method4
(
	double _freq, 
	double mag_ref
)
{

	int i, max_order;
    FCoeff * fcoeff, *sfcoeff;
    DDDnode *fcddd, *sfcddd, *msp_ddd,*ddd_aux, *prev_ddd;
    double path_val;
    register double nfreq = 1.0;
	static int new_path_count = 0;
   
    /* go find all the product terms with magnitude  
       greater than  rel_ref*value for each fcoefficient DDD.
    */

	if(_DEBUG)
	{
		cout <<"mag_ref: "<<mag_ref<<endl;
	}

    if(!fcoeff_list)
	{
        error_mesg(INT_ERROR,"Not fcoefficient list found.");
        return;
    }

	int offset = fcoeff_list->get_offset();
    max_order = fcoeff_list->get_max_order();
    
    if(!sfcoeff_list)
	{
        sfcoeff_list = new FCoeffList(ddd_mgr,offset);
	}

    sfcoeff_list->reset_fcoeff_calced();

	for(i = 0; i < max_order; nfreq = nfreq * _freq,i++)
	{		

		if(_DEBUG)		
		//if(1)
		{
			cout <<"order: "<<i
				 <<", nfreq: "<<nfreq<<endl;
		}

        fcoeff = fcoeff_list->get_fcoeff(i);
        fcddd = fcoeff_list->get_fcoeff_ddd(i);
        sfcoeff = sfcoeff_list->get_fcoeff(i);
        sfcddd = sfcoeff_list->get_fcoeff_ddd(i);


        if(fcddd == ddd_mgr->Empty())
		{
            //print_mesg("No product terms found.");
            //ddd_mgr->statistic(fcddd);
            continue;
        }
        else if(!fcoeff->keep_val())
		{
            cout<<"suppressed, ignored"<<endl;
            //ddd_mgr->statistic(fcddd);
            continue;
        }

        if(fcoeff->Prev_path() && 
		   (fabs(fcoeff->Prev_pval())*nfreq < mag_ref))
		{
            //ddd_mgr->statistic(fcddd);

			if(_DEBUG)
			{
				cout<<" order: "<<i<<endl;
				cout<<" mag_ref:"<<mag_ref
					<<" path_val:"<<fabs(fcoeff->Prev_pval())
					<<" total path_val:"<<fabs(fcoeff->Prev_pval())*nfreq 
					<< endl;
				cout<<"nfreq:" <<nfreq<<"--->skip!!!"<<endl;
			}

            continue;
        }
            
        theCurPath->ClearStack();
        double tpath_val;
        prev_ddd = NULL;
		int path_count_coeff = 0;

        while(1)
		{		
			//if it is the first term
			if(path_count_coeff == 0)
			{
				// for the first term, we call tranditional shortest
				// path method to find it.
				ddd_mgr->FCDDDShortestPath_4(fcddd);

				// get the path and remove it from fcddd
				msp_ddd = ddd_mgr->FCDDDNextShortestPath_4(&fcddd);
			}
			else
			{
				//msp_ddd = ddd_mgr->FCDDDShortestPath_4(fcddd);
				msp_ddd = ddd_mgr->FCDDDNextShortestPath_4(&fcddd);
			}

			if(msp_ddd)
			{
				//ddd_mgr->fcoeff_print_symbolic_path(msp_ddd,cout);	
				
				new_path_count++;				
				path_count_coeff++;
				if((path_count_coeff  >= MAX_PATH_COUNT_PER_COEFF)
				   || (new_path_count > MAX_PATH_COUNT) )
				{
					break;
				}
			}

			bool compute_sign = false;
            path_val = ddd_mgr->FCPathValue(msp_ddd, compute_sign);
            tpath_val = path_val*nfreq;

            if(fabs(tpath_val) < mag_ref)
			{
                fcoeff->Prev_path() = msp_ddd;
                fcoeff->Prev_pval() = path_val;

				if(_DEBUG)
				{
					cout<<" order: "<<i<<endl;
					cout<<" mag_ref:"<<mag_ref
						<<" path_val:"<<path_val
						<<" total path_val:"<<tpath_val
						<<endl;
					cout<<"break"<<endl;
				}

                while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}

                break;
            }		
           	
			// add the new path into the sfcddd
            while((ddd_aux = theCurPath->DDDpop()))
			{
                sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
			}
			
            prev_ddd = NULL;
            fcoeff->Prev_path() = msp_ddd;
            fcoeff->Prev_pval() = path_val;
            theCurPath->DDDpush(msp_ddd);


            if(fcddd == ddd_mgr->Base() || 
			   fcddd == ddd_mgr->Empty())
			{
                while((ddd_aux = theCurPath->DDDpop()))
				{
                    sfcddd = ddd_mgr->Union(sfcddd,ddd_aux);
				}
                break;
            }
		   
        }

        fcoeff->set_ddd(fcddd);
		if(sfcddd != sfcoeff->get_ddd())
		{			
			sfcoeff->set_ddd(sfcddd);			
			
			if(theCkt)
			{
				cout <<"number of terms generated: " << new_path_count << endl;
				//theCkt->ac_mgr->forceGCForAll();
			}
			
			
		}

		if(new_path_count >= MAX_PATH_COUNT)
		{			
			cout <<"Max number of paths (" << new_path_count 
				 << ") is reached" << endl;

			break;
		}
    }

	cout <<"number of terms generated: " << new_path_count << endl;

}

#endif // __USE_FOURTH_TERM_GENERATION 


/* Delete some DDD nodes with weak contribuation to the final numeric
** value.  
*/
boolean
Cofactor::elem_elim
(
	double *_freq, 
	complex *val, 
	int _fnum, 
	int index, 
	PolyType _type
)
{
    DDDnode *new_cof;
    complex exa_val, simp_val;
    int    i;

    //* we first obtained the cofactor w.r.t. the DDD index given
    new_cof = ddd_mgr->Subset0(sddd,index);
    if(new_cof == sddd)
	{
        error_mesg(INT_ERROR,"No cofactor found for given index");
        return 0;
    }
    // calculation intialization
	HasherTree *prec_ht = new HasherTree(FALSE);	
    ddd_mgr->PreCalculationR(sddd, prec_ht);
    ddd_mgr->PreCalculationR(new_cof, prec_ht);
	delete prec_ht;	
    
    for(i=0; i < _fnum; i++)
	{

		HasherTree *ht    = new HasherTree(FALSE);
        // first we calculate the exact DDD value
        exa_val = val[i]*sign;
        
        // then we calculate the simplifief DDD value
        simp_val = ddd_mgr->GetDetermValueR(new_cof,_freq[i],ht);

        //cout<<"exa_val:"<<exa_val<<" simp_val:"<<simp_val<<endl;
        if(magn(exa_val - simp_val)/magn(exa_val) > opMagErr)
		{
			delete ht;
			return 0;        
		}

		delete ht;
    }
    // we deduct all the pathes having the DDD node with index given
    sddd = new_cof;

    return 1;
}

void
Cofactor::fcoeff_supp
(
	double _freq, 
	double mag_ref
)
{   
    if(!cddd || !fcoeff_list)
	{
        return;
	}

    fcoeff_list->fcoeff_suppress(_freq, mag_ref);
}   
    
void 
Cofactor::fcoeff_decancel
(
	int ind1, 
	int ind2, 
	int ind3, 
	int ind4
)
{   
    fcoeff_list->fcoeff_decancel(ind1, ind2, ind3, ind4);
}

/* Delete a circuit element with weak contribuation to the both
** numerator and denominator. The element symbols may appear four
** positions at most. If the element is the only symbol a DDD node
** represents, then all the pathes the DDD node asscoiates are
** removed.  
*/

static int num_deleted;
static int den_deleted;
int
ExpList::whole_elem_elim_both
(
	DDDmanager *ddd_mgr, 
	MNAMatrix *matrix,
	double *_freq, 
	int _fnum, 
	char *bname, 
	int ind1, 
	int ind2, 
	int ind3, 
	int ind4
)
{
    DDDnode *ddd_aux;
    complex exa_val_num, exa_val_den, exa_val;
    complex simp_val_num, simp_val_den, simp_val;
    MNAElement *elem1, *elem2, *elem3, *elem4;
    int    i;

    elem1 = elem2 = elem3 = elem4 = NULL;

    /* first , we temporaily delete the element in 4 
       posoitions by change the MNAelment value.
    */
    //cout <<"Ind1: "<<ind1<<"Ind2: "<<ind2<<"Ind3: "<<ind3<<"Ind4: "<<ind4<<endl;
    //cout <<"Simplify both nume and deno"<<endl;
    if(ind1 != -1)
	{
        elem1 = ddd_mgr->get_label2elem()->get_elem(ind1);
        //cout <<"num of branch left:"<<elem1->num_branch(NUM)<<endl;
        // It does not matter NUM or DEN here
        elem1->update_cvalue(bname,NUM);
    }
    if(ind2 != -1)
	{
        elem2 = ddd_mgr->get_label2elem()->get_elem(ind2);
        //cout <<"num of branch left:"<<elem2->num_branch(NUM)<<endl;
        elem2->update_cvalue(bname,NUM);
    }
    if(ind3 != -1)
	{
        elem3 = ddd_mgr->get_label2elem()->get_elem(ind3);
        //cout <<"num of branch left:"<<elem3->num_branch(NUM)<<endl;
        elem3->update_cvalue(bname,NUM);
    }
    if(ind4 != -1)
	{
        elem4 = ddd_mgr->get_label2elem()->get_elem(ind4);
        //cout <<"num of branch left:"<<elem4->num_branch(NUM)<<endl;
        elem4->update_cvalue(bname,NUM);
    }
    
    for(i=0; i < _fnum; i++)
	{

		HasherTree *prec_ht    = new HasherTree(FALSE);
		HasherTree *ht    = new HasherTree(FALSE);

        // first we calculate the exact DDD value
        bool useSimpExp = 1;
        simp_val_num = num_list->evaluate(_freq[i], prec_ht, ht, useSimpExp);
        simp_val_den = den_list->evaluate(_freq[i], prec_ht, ht, useSimpExp);

		/*
		  cout<<"sim_num:"<<simp_val_num<<
		  " simp_den:"<<simp_val_den<<endl;
		*/

        if(magn(simp_val_den) == 0.0)
		{
            simp_val = 0.0;
		}
        else
		{
            simp_val = simp_val_num/simp_val_den;
		}

        exa_val_num = num_val[i];
        exa_val_den = den_val[i];
        exa_val = exa_val_num/exa_val_den;
        
        // cout<<"exa_val:"<<exa_val<<" simp_val:"<<simp_val<<endl;
        if(magn(exa_val - simp_val)/magn(exa_val) > opMagErr)
		{
            // restore the original cvalue for all the MNA elements
            if(elem1)
			{
                elem1->restore_cvalue(bname);
			}
            if(elem2)
			{
                elem2->restore_cvalue(bname);
			}
            if(elem3)
			{
                elem3->restore_cvalue(bname);
			}
            if(elem4)
			{
                elem4->restore_cvalue(bname);
			}

			delete ht;

            return 0;
        }

		delete prec_ht;		
		delete ht;
    }

    //The whole element can be removed

    cout <<"Element: "<<bname<<" deleted"<<endl;
    num_deleted++;
    den_deleted++;
    cout <<"After DDD reduction"<<endl;
    cout <<"num_deleted: "<<num_deleted<<endl;
    cout <<"den_deleted: "<<den_deleted<<endl;

    int flag = 0;
    if(elem1)
	{
        if(!elem1->delete_branch(bname,NUM))
		{
            ddd_aux = num_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind1);
            num_list->set_sddd(ddd_aux);
            matrix->delete_element(ind1);
            flag = 1;
        }
        if(!elem1->delete_branch(bname,DEN))
		{
            ddd_aux = den_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind1);
            den_list->set_sddd(ddd_aux);
            matrix->delete_element(ind1);
            flag = 1;
        }
    }

    if(elem2)
	{
        if(!elem2->delete_branch(bname,NUM))
		{
            ddd_aux = num_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind2);
            num_list->set_sddd(ddd_aux);
            matrix->delete_element(ind2);
            flag = 1;
        }
        if(!elem2->delete_branch(bname,DEN))
		{
            ddd_aux = den_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind2);
            den_list->set_sddd(ddd_aux);
            matrix->delete_element(ind2);
            flag = 1;
        }
    }
    if(elem3)
	{
        if(!elem3->delete_branch(bname,NUM))
		{
            ddd_aux = num_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind3);
            num_list->set_sddd(ddd_aux);
            matrix->delete_element(ind3);
            flag = 1;
        }
        if(!elem3->delete_branch(bname,DEN))
		{
            ddd_aux = den_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind3);
            den_list->set_sddd(ddd_aux);
            matrix->delete_element(ind3);
            flag = 1;
        }
    }

    if(elem4)
	{
        if(!elem4->delete_branch(bname,NUM))
		{
            ddd_aux = num_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind4);
            num_list->set_sddd(ddd_aux);
            matrix->delete_element(ind4);
            flag = 1;
        }
        if(!elem4->delete_branch(bname,DEN))
		{
            ddd_aux = den_list->get_sddd();
            ddd_aux = ddd_mgr->Subset0(ddd_aux,ind4);
            den_list->set_sddd(ddd_aux);
            matrix->delete_element(ind4);
            flag = 1;
        }
    }

	if(_DEBUG)
	{
   
		cout <<"for numerator"<<endl;
		ddd_aux = num_list->get_sddd();
		ddd_mgr->statistic(ddd_aux);
		
		cout <<"for denominator"<<endl;
		ddd_aux = den_list->get_sddd();
		ddd_mgr->statistic(ddd_aux);
	}
   
    
	return 1;
}

/*     Factoring the common DDD node from both numerator and
**    denominator, such that reduce matrix size.  
*/

#ifdef _DEBUG
static int num_factor_ddd;
#endif

//#define _CANCEL_REMOVE

void
ExpList::factor_ddd_node
(
	DDDmanager *ddd_mgr, 
	MNAMatrix *matrix,
	double *_freq, 
	int _fnum
)
{
    MNAElement      *elem;
    int             max_label = matrix->get_num_elem();
    DDDnode *sddd_num, *sddd_den;
    complex exa_val_num, exa_val_den, exa_val;
    complex simp_val_num, simp_val_den, simp_val;
    int    i,lab,sign1,sign2;
    double  error;

    print_mesg("Factoring common DDD node ...");
    for(lab=max_label; lab >= 1; lab--)
	{
        elem = ddd_mgr->get_label2elem()->get_elem(lab);

		// we only try the diagnonal element
		if( elem->get_row()  != elem->get_col() )
		{
            continue;
		}

		if(elem->get_row() == num_list->get_row() ||
		   elem->get_col() == num_list->get_col() ||
		   elem->get_row() == den_list->get_row() ||
		   elem->get_col() == den_list->get_col() )
		{
            continue;
		}

        sddd_num = num_list->get_sddd();                        
		sddd_den = den_list->get_sddd();            

        sddd_num = ddd_mgr->Subset1(sddd_num,lab);
        sddd_den = ddd_mgr->Subset1(sddd_den,lab);

		//ddd_mgr->statistic(sddd_num);
		//ddd_mgr->statistic(sddd_den);
        if(sddd_num == ddd_mgr->Empty() ||
           sddd_den == ddd_mgr->Empty() )
		{
            continue;
		}
        
        /* we then numerically eliminate the its remained diagonal
		   appearance of the devices in the DDD nodes factored out.  
		*/
#ifdef _CANCEL_REMOVE
        cout <<"Index: "<<lab <<endl;
        for(dbaux = elem->get_blist()->get_blist();dbaux;
			dbaux = dbaux->Next())
		{

            if(dbaux->get_branch()->get_stat() == B_DELETED)
			{
                continue;
			}

            /*
			  cout<<"Branch: "<<dbaux->get_branch()->get_name()
			  <<" tried"<<endl;
            */
            lab2 = find_diag_index(dbaux->get_branch(), 
								   lab, matrix->get_imatrix());
            if(lab2 == -1)
			{
                continue;
			}

            //cout <<"lab2: "<<lab2<<endl;
            elem2 = ddd_mgr->get_label2elem()->get_elem(lab2);
            if(elem2)
			{
                elem2->update_cvalue(dbaux->get_branch()->get_name(),NUM);
			}   
        }
#endif

        /* our sign adjustment assumes the row of
		   num_list and den_list are same */
        if((elem->get_col() > num_list->get_col() &&
			elem->get_col() < den_list->get_col()) ||
		   (elem->get_col() < num_list->get_col() &&
			elem->get_col() > den_list->get_col()))
		{
            sign1 = -1;
		}
        else
		{
            sign1 = 1;
		}
        //cout <<"col:"<<elem->get_col()<<"sign1:"<<sign1<<endl;
        sign2 = sign1*num_list->get_sign()*den_list->get_sign();

		HasherTree *prec_ht = new HasherTree(FALSE);
        ddd_mgr->PreCalculationR(sddd_num, prec_ht);
        ddd_mgr->PreCalculationR(sddd_den, prec_ht);
		delete prec_ht;
		
        // in the following sign caculation, we
        // assume num_list->row == den_list_row.

        int deleted = 1;
        for(i=0; i < _fnum; i++)
		{

			HasherTree *ht    = new HasherTree(FALSE);
			
            simp_val_num = 
				ddd_mgr->GetDetermValueR(sddd_num,_freq[i],ht);

            simp_val_den = 
				ddd_mgr->GetDetermValueR(sddd_den,_freq[i],ht);
			

            if(magn(simp_val_den) == 0.0)
			{
                simp_val = 0.0;
			}
            else
			{
                simp_val = sign2*(simp_val_num/simp_val_den);
			}

            //cout<<"sign2:"<<sign2<<endl;

            exa_val_num = num_val[i];
            exa_val_den = den_val[i];
            exa_val = exa_val_num/exa_val_den;

            error = magn(exa_val-simp_val)/magn(exa_val);

			if(_DEBUG)
			{
				cout<<"exa_val:"<<magn(exa_val)
					<<" simp_val:"<<magn(simp_val)
					<<" freq: " << _freq[i]
	                <<" error:"<< error<<endl;
				cout <<"num_val: " << magn(simp_val_num) 
					 <<" den_val: " << magn(simp_val_den) << endl;
			}

            if(error > opMagErr)
			{
                deleted = 0;
				delete ht;
                break;
            }

			delete ht;

        }


        if(!deleted)
		{ // can not be deleted
#ifdef _CANCEL_REMOVE
            for(dbaux = elem->get_blist()->get_blist();dbaux;
				dbaux = dbaux->Next())
			{

                if(dbaux->get_branch()->get_stat() == B_DELETED)
				{
                    continue;
				}

                lab2 = find_diag_index(dbaux->get_branch(), 
									   lab, matrix->get_imatrix());
                if(lab2 == -1)
				{
                    continue;
				}

                elem2 = ddd_mgr->get_label2elem()->get_elem(lab2);
                if(elem2)
				{
                    elem2->restore_cvalue(dbaux->get_branch()->get_name());
				}
            }
#endif
            continue;
        }

        cout <<"DDD node index: "<<lab<<" deleted"<<endl;

		if(_DEBUG)
		{
			cout <<"Branch:";
			elem->get_blist()->print_branch(cout,NUM);
			cout <<endl;
		}

        // we need adjust sign
        sign1 = sign1*num_list->get_sign();
        num_list->set_sign(sign1);

        // we mark the deleted branch in MNAelement
#ifdef _CANCEL_REMOVE
        for(dbaux = elem->get_blist()->get_blist();dbaux;
			dbaux = dbaux->Next())
		{
            if(dbaux->get_branch()->get_stat() == B_DELETED)
			{
                continue;
			}

            lab2 = find_diag_index(dbaux->get_branch(), 
								   lab, matrix->get_imatrix());
            if(lab2 = -1)
			{
                continue;
			}

            elem2 = ddd_mgr->get_label2elem()->get_elem(lab2);
            if(elem2)
			{
                elem2->delete_branch(
					dbaux->get_branch()->get_name(),NUM);
                elem2->delete_branch(
					dbaux->get_branch()->get_name(),DEN);
            }

            dbaux->get_branch()->get_stat() = B_DELETED;
        }
#endif

		num_list->set_sddd(sddd_num);
		den_list->set_sddd(sddd_den);

        cout <<"for numerator"<<endl;
        ddd_mgr->statistic(sddd_num);

        cout <<"for denominator"<<endl;
        ddd_mgr->statistic(sddd_den);
        num_factor_ddd++;
        cout <<"total # of ddd nodes factored out: "<<num_factor_ddd<<endl;

    }

    return;
}

/* simplify the expression list at given frequency point.  We find out
** all the product terms with mag greater than the specified magnitude
** given in the DDD cofactors.  If no such product term is found, no
** simpilication is performed.  
*/

void
ExpList::simp_expr
( 
	double _freq, 
	double rel_th, 
	int findex 
)
{
    Cofactor *cofact_aux = num_list;
    double mag_ref;

    if(input->type == dVOL)
	{// voltage type excitation
        if(!num_list || !den_list)
		{
            error_mesg(INT_ERROR,"Invalid num or den cofactor list");
            return;
        }
        // first, numerator
        mag_ref = rel_th*magn(num_val[findex]);

        cout<<"_freq:"<<_freq
			<<" num_val[findex]"
			<<num_val[findex]<<endl;

        for(; cofact_aux ; cofact_aux = cofact_aux->Next())
		{
            cofact_aux->simp_cofactor( _freq, mag_ref );
		}

        // second, denominator
        mag_ref = rel_th*magn(den_val[findex]);
        cout<<"_freq:"<<_freq<<" den_val[findex]"<<
            den_val[findex]<<endl;
        cofact_aux = den_list;
        for(; cofact_aux ; cofact_aux = cofact_aux->Next())
		{
            cofact_aux->simp_cofactor( _freq, mag_ref );
		}
    }
    else if(input->type == dCUR)
	{ // current type excitation
        if(!num_list)
		{
            error_mesg(INT_ERROR,"Invalid num cofactor list");
            return; 
        }
        mag_ref = rel_th*magn(num_val[findex]);
        for(; cofact_aux ; cofact_aux = cofact_aux->Next())
		{
            cofact_aux->simp_cofactor( _freq, mag_ref );
		}
    }
}

/* simplify the expression list for fully expanded coefficient DDD.
** We find out all the product terms with mag greater than the
** specified magnitude given in the DDD cofactors.  If not such
** product term is found, no simpilication is performed.  
*/

void
ExpList::simp_fcexpr
(
	double _freq, 
	int findex
)
{
    Cofactor *cofact_aux = num_list;
    double     mag_ref;
    
	if(!num_list || !den_list)
	{
		error_mesg(INT_ERROR,"Invalid num or den cofactor list");
		return;
	}
	// first, numerator
	mag_ref = opFCNRelThd*magn(num_val[findex]);

	//cout <<endl<<"Numerator"<<endl;
	cofact_aux = num_list;
	for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
#ifdef  __USE_FOURTH_TERM_GENERATION 
		cofact_aux->simp_fcofactor_method4( _freq, mag_ref );
#else
 #ifdef  __USE_THIRD_TERM_GENERATION 
		cofact_aux->simp_fcofactor_method3( _freq, mag_ref );
 #else
   #ifdef  __USE_SECOND_TERM_GENERATION 
		cofact_aux->simp_fcofactor_method2( _freq, mag_ref );
   #else
		cofact_aux->simp_fcofactor_method1( _freq, mag_ref );
   #endif
 #endif
#endif
	}

	// second, denominator
	//cout <<endl<<"Denominator"<<endl;
	mag_ref = opFCDRelThd*magn(den_val[findex]);
	cofact_aux = den_list;
	for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
#ifdef  __USE_FOURTH_TERM_GENERATION 
		cofact_aux->simp_fcofactor_method4( _freq, mag_ref );
#else
 #ifdef  __USE_THIRD_TERM_GENERATION 
		cofact_aux->simp_fcofactor_method3( _freq, mag_ref );
 #else
   #ifdef  __USE_SECOND_TERM_GENERATION 
		cofact_aux->simp_fcofactor_method2( _freq, mag_ref );
   #else
		cofact_aux->simp_fcofactor_method1( _freq, mag_ref );
   #endif
 #endif
#endif
	}

}


/* Suppress the fcoefficients expression by expressions 
*/

void
ExpList::fcoeff_supp_expr
(
	double _freq, 
	double rel_th, 
	int findex
)
{
    Cofactor *cofact_aux = num_list;
    double    mag_ref;
    
    if(input->type == dVOL)
	{// voltage type excitation
        if(!num_list || !den_list)
		{
            error_mesg(INT_ERROR,"Invalid num or den cofactor list");
            return;
        }
        // first, numerator
        mag_ref = rel_th*magn(num_val[findex]);
        cout <<"numerator"<<endl;
        for(; cofact_aux ; cofact_aux = cofact_aux->Next())
		{
            cofact_aux->fcoeff_supp(_freq, mag_ref );
		}

        // second, denominator
        mag_ref = rel_th*magn(den_val[findex]);
        cofact_aux = den_list;
        cout <<"denominator"<<endl;
        for(; cofact_aux ; cofact_aux = cofact_aux->Next())
		{
            cofact_aux->fcoeff_supp(_freq, mag_ref );
		}
    }
    else if(input->type == dCUR)
	{ // current type excitation
        if(!num_list)
		{
            error_mesg(INT_ERROR,"Invalid num cofactor list");
            return; 
        }
        mag_ref = rel_th*magn(num_val[findex]);
        for(; cofact_aux ; cofact_aux = cofact_aux->Next())
		{
            cofact_aux->fcoeff_supp(_freq, mag_ref );
		}
    }
}

/*  Delete a circuit element with weak contribuation to the final
**  numeric value. The element symbols may appear four positions at
**  most. For the element is the only symbol a DDD node represents,
**  then all the pathes the DDD node asscoiates are removed.  
*/

void
Cofactor::whole_elem_elim
(
	double *_freq, 
	complex *val, int _fnum, 
	PolyType ptype, 
	char *bname, 
	int ind1, 
	int ind2, 
	int ind3, 
	int ind4
)
{
    complex exa_val, simp_val;
    MNAElement *elem1, *elem2, *elem3, *elem4;
    int    i;

    elem1 = elem2 = elem3 = elem4 = NULL;

    /* first , we temporaily delete the element in four 
       posoition by change the MNAelment value.
    */
    //cout <<"Ind1: "<<ind1<<"Ind2: "<<ind2<<"Ind3: "<<ind3<<"Ind4: "<<ind4<<endl;
    if(ind1 != -1)
	{
        elem1 = ddd_mgr->get_label2elem()->get_elem(ind1);
        //cout <<"num of branch left:"<<elem1->num_branch(ptype)<<endl;
        elem1->update_cvalue(bname,ptype);
    }
    if(ind2 != -1)
	{
        elem2 = ddd_mgr->get_label2elem()->get_elem(ind2);
        //cout <<"num of branch left:"<<elem2->num_branch(ptype)<<endl;
        elem2->update_cvalue(bname,ptype);
    }
    if(ind3 != -1)
	{
        elem3 = ddd_mgr->get_label2elem()->get_elem(ind3);
        //cout <<"num of branch left:"<<elem3->num_branch(ptype)<<endl;
        elem3->update_cvalue(bname,ptype);
    }
    if(ind4 != -1)
	{
        elem4 = ddd_mgr->get_label2elem()->get_elem(ind4);
        //cout <<"num of branch left:"<<elem4->num_branch(ptype)<<endl;
        elem4->update_cvalue(bname,ptype);
    }
    
    // we then go through all the frequency to see the effect
	HasherTree *prec_ht = new HasherTree(FALSE);
    ddd_mgr->PreCalculationR(sddd, prec_ht);
	delete prec_ht;	
    
    for(i=0; i < _fnum; i++)
	{
		HasherTree *ht    = new HasherTree(FALSE);
        
        // first we calculate the exact DDD value
        exa_val = val[i]*sign;
        
        simp_val = ddd_mgr->GetDetermValueR(sddd,_freq[i],ht);
        
		//cout<<"exa_val:"<<exa_val<<" simp_val:"<<simp_val<<endl;
        
		if(magn(exa_val - simp_val)/magn(exa_val) > opMagErr)
		{
            // restore the original cvalue for all the MNA elements
            if(elem1)
			{
                elem1->restore_cvalue(bname);
			}
            if(elem2)
			{
                elem2->restore_cvalue(bname);
			}
            if(elem3)
			{
                elem3->restore_cvalue(bname);
			}
            if(elem4)
			{
                elem4->restore_cvalue(bname);
			}
            /*
			  exa_val = ddd_mgr->GetDetermValue(sddd,_freq[i]);
			  cout <<"calced exa_val:"<<exa_val<<endl;
            */

			delete ht;
            
			return;
        }

		delete ht;
    }
    //The whole element can be removed
    if(ptype == NUM)
	{
        cout <<"Element: "<<bname<<" deleted(num)"<<endl;
        num_deleted++;
    }
    else
	{
        cout <<"Element: "<<bname<<" deleted(den)"<<endl;
        den_deleted++;
    }

    //ddd_mgr->statistic(sddd);
    cout <<"After DDD reduction"<<endl;
    cout <<"num_deleted: "<<num_deleted<<endl;
    cout <<"den_deleted: "<<den_deleted<<endl;
    int flag = 0;

    if(elem1)
	{
        if(!elem1->delete_branch(bname,ptype))
		{
            sddd = ddd_mgr->Subset0(sddd,ind1);
            flag = 1;
        }
    }
    if(elem2)
	{
        if(!elem2->delete_branch(bname,ptype))
		{
            sddd = ddd_mgr->Subset0(sddd,ind2);
            flag = 1;
        }
    }

    if(elem3)
	{
        if(!elem3->delete_branch(bname,ptype))
		{
            sddd = ddd_mgr->Subset0(sddd,ind3);
            flag = 1;
        }
    }

    if(elem4)
	{
        if(!elem4->delete_branch(bname,ptype))
		{
            sddd = ddd_mgr->Subset0(sddd,ind4);
            flag = 1;
        }
    }

    if(flag)
	{
        ddd_mgr->statistic(sddd);
	}

    return;
}

/*     Delete some fcoefficient DDD nodes with weaky contribuation to
**    the final numeric value of the the fully expanded coefficient.  
*/

void
Cofactor::fcoeff_elem_elim
(
	int index, 
	PolyType _type
)
{
    int    i;
    int    num_branch;
    BranchList *blist;
    
    if(!theFLabMap)
	{
        error_mesg(INT_ERROR,"No fcoefficient list");
        return;
    }

    blist = ddd_mgr->get_label2elem()->get_elem(index)->get_blist();
	num_branch = blist->get_num();
    
    for(i=0;i < num_branch; i++)
	{
        fcoeff_list->fcoeff_elem_elim(index, i);
	}
}

/* find the complex DDD index of the branch which is in diagonal
   position of the index given.  if -1 is return, no index is found.  
*/

int
ExpList::find_diag_index
(
	Branch *branch, 
	int _index, 
	IndexMatrix *imatrix
)
{
    Branch *blaux = branch;
    int   n1,n2,n3,n4;
    int   ind1, ind2, ind3, ind4;

    if(!branch)
	{
        error_mesg(INT_ERROR,"No branch is given");
        return -1;
    }

    /* we through all the branches */
    if(blaux->get_stat() == B_DELETED)
	{
        sprintf(_buf,"%s is already eliminated",blaux->get_name());
        error_mesg(INT_ERROR,_buf);
        return -1;
    }

    n1 = n2 = n3 = n4 = -1;
    ind1 = ind2 = ind3 = ind4 = -1;

    if(blaux->get_type() == dRES ||
       blaux->get_type() == dCAP ||
       blaux->get_type() == dIND )
	{

        if(blaux->get_node1()->get_real_num())
		{
            n1 = blaux->get_node1()->get_index();
		}

        if(blaux->get_node2()->get_real_num())
		{
            n2 = blaux->get_node2()->get_index();
		}

		if(n1 == -1 || n2 == -1)
		{
			return -1;
		}

		/*
		  cout<<"Bname:"<<blaux->get_name()<<
		  " n1:"<<n1<<" n2:"<<n2<<endl;
		*/
		/*    n1    n2 
			  n1    ind1    ind2
			  n2    ind3    ind4
		*/
		if(n1 != -1)
		{
			ind1 = imatrix->get_label(n1,n1);
		}
		if(n1 != -1 && n2 != -1)
		{
			ind2 = imatrix->get_label(n1,n2);
			ind3 = imatrix->get_label(n2,n1);
		}
		if(n2 != -1)
		{
			ind4 = imatrix->get_label(n2,n2);
		} 
		if( _index == ind1 )
		{
			return ind4;
		}
		else if( _index == ind4 )
		{
			return ind1;
		}
		else if( _index == ind2 )
		{
			return ind3;
		}
		else if( _index == ind3 )
		{
			return ind2;
		}
		else
		{
			return -1;
		}
	}
	else if( blaux->get_type() == dVCCS ) 
	{
		if(blaux->get_node1()->get_real_num())
		{
			n1 = blaux->get_node1()->get_index();
		}
		if(blaux->get_node2()->get_real_num())
		{
			n2 = blaux->get_node2()->get_index();
		}
		if(blaux->get_node3()->get_real_num())
		{
			n3 = blaux->get_node3()->get_index();
		}
		if(blaux->get_node4()->get_real_num())
		{
			n4 = blaux->get_node4()->get_index();
		}

		/*
		  cout<<"Bname:"<<blaux->get_name()<<
		  " n1:"<<n1<<" n2:"<<n2<<" n3:"<<n3<<" n4:"<<n4<<endl;
		*/

		/*        n3    n4 
				  n1    ind1    ind2
				  n2    ind3    ind4
		*/
		if(n1 != -1 && n3 != -1)
		{
			ind1 = imatrix->get_label(n1,n3);
		}
		if(n1 != -1 && n4 != -1)
		{
			ind2 = imatrix->get_label(n1,n4);
		}
		if(n2 != -1 && n3 != -1)
		{
			ind3 = imatrix->get_label(n2,n3);
		}
		if(n2 != -1 && n4 != -1)
		{
			ind4 = imatrix->get_label(n2,n4);
		}

		if( _index == ind1 )
		{
			return ind4;
		}
		else if( _index == ind4 )
		{
			return ind1;
		}
		else if( _index == ind2 )
		{
			return ind3;
		}
		else if( _index == ind3 )
		{
			return ind2;
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

/* simply the expressions represented by complex DDD up to the error
   criteria given by user for both magnitude and phase. 
 */
void
ACAnalysisManager::simplify_ddd()
{
	ExpList * poly_list = poly;
	double *freq = command->freq_pts;
 
	/* we first obtain the exact values for all the frequency points
	   of interest */
	/* evaluate(freq[0], 0); */
 
	print_mesg("Simplify complex ddd ... ");

	/* Then, we perform the approximation, freq by freq and cofactor
	   by cofactor.  */
	if(!theTopoList)
	{
		theTopoList = new DDDstack;
	}

	for (int i = 0; i < num_pts; i++)
	{
		printf("freq[%d]: %f\n",i,freq[i]);
		for(poly_list = poly; poly_list ;
			poly_list = poly_list->next)
		{
			poly_list->simp_expr(freq[i], opRelThd , i);
		}
	}

	print_mesg("End of simplification ");
 
#ifdef _DEBUG
	bool useSimpExp = 1;
	bool enableBchElim = 0;
	freq_sweep(useSimpExp, enableBchElim);
#endif
	delete theTopoList;
	theTopoList = NULL;
    ddd_simplied = 1;

}

/* simplify the expressions represented by coefficient list up to the
   error criteria for both magnitude and phase. 
 */

void
ACAnalysisManager::simplify_coefflist()
{
	ExpList * poly_list = poly;
	double *freq = command->freq_pts;
 
	/* we first obtain the exact values for all the frequency points
	   of interest */
         
	print_mesg("Simplify fcoefficient ddd list ... ");

#ifdef  __USE_FOURTH_TERM_GENERATION 
		cout <<"Use the fourth term generation method (method4, reverse dynamic shortest path)..." << endl;
#else
 #ifdef  __USE_THIRD_TERM_GENERATION 
		cout <<"Use the third term generation method (method3, dynamic shortest path)..." << endl;
 #else
   #ifdef  __USE_SECOND_TERM_GENERATION 
		cout <<"Use the second term generation algorithm (method2, dynamic)..." <<endl;
   #else
		cout <<"Use the first term generation method (method1, shortest path)..." << endl;
   #endif
 #endif
#endif	

	/* Then, we perform the approximation, freq by freq and cofactor
	   by cofactor.  */
	if(!theTopoList)
	{
		theTopoList = new DDDstack;
	}
	
	if(!theCurPath)
	{
        theCurPath = new DDDstack;
	}
	for (int i = 0; i < num_pts; i++)
	{
		printf("freq[%d]: %f\n",i,freq[i]);

		for(poly_list = poly; poly_list; poly_list = poly_list->next)
		{
			poly_list->simp_fcexpr(freq[i], i);
		}
	}
 
	print_mesg("End of simplification ");

	/* caluculate all numerical value of simplied coefficient */	
	calc_sfcoeff_list_value();

#ifdef _DEBUG
	bool useSimpExp = 1;
	fcoeff_freq_sweep(useSimpExp);
#endif 

	fcoeff_simped = 1;
	delete theTopoList;
	delete theCurPath;
	theTopoList = NULL;
	theCurPath = NULL;
}


/* simplify the expressions represented by coefficient list up to the
   error criteria for both magnitude and phase.  
*/

void
ACAnalysisManager::elem_elimination()
{
	double     *freq = command->freq_pts;
	Cofactor   *num_cof, *den_cof;
	int        max_label = matrix->get_num_elem();
	MNAElement *elem;
        
	if(!poly)
	{
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return;
	}
         
	print_mesg("Complex DDD node elimination");
 
	/* Then, we perform the element elimination, element by element,
	   cofactor by cofactor. We assume only one polynormial exist.
	   (single output) and only one cofactor exist for the numerator
	   and denominator.  */
	num_cof = poly->num_list;
	den_cof = poly->den_list;
	for(int i=1; i <= max_label; i++)
	{
		elem = ddd_mgr->get_label2elem()->get_elem(i);
        cout<<"Index:"<<i <<" elem:("
			<<elem->get_row()
			<<","<<elem->get_col()
			<<") "<<endl;

		if(elem->get_row() != num_cof->get_row() &&
		   elem->get_col() != num_cof->get_col() )
		{
			if(num_cof->elem_elim(freq,poly->num_val,num_pts,i,NUM))
			{
				cout<<"elem:("<<elem->get_row()
					<<","<<elem->get_col()
					<<") deleted(num)."<<endl;
			}
		}

		if(elem->get_row() != den_cof->get_row() &&
		   elem->get_col() != den_cof->get_col())
		{
			if(den_cof->elem_elim(freq,poly->den_val,num_pts,i,DEN))
			{
				cout<<"elem:("<<elem->get_row()
					<<","<<elem->get_col()
					<<") deleted(den)."<<endl;
			}
		}
	}

    cout <<endl<<"Numerator"<<endl;
    ddd_mgr->statistic(num_cof->get_sddd());
    cout <<endl<<"Denominator"<<endl;
    ddd_mgr->statistic(den_cof->get_sddd());
	print_mesg("End of node elimnination ");

	bool enBranchElim = 0;
	bool useSimpExp = 1;
	freq_sweep(useSimpExp, enBranchElim);
} 

/* simplify the expressions represented by coefficient list up to the
   error criteria for both magnitude and phase.  
*/

void
ACAnalysisManager::fcoeff_elem_elimination()
{
	Cofactor  *num_cof, *den_cof;
	int        max_label = matrix->get_num_elem();
	MNAElement    *elem;
        
	if(!poly){
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return;
	}
         
	print_mesg("DDD fcoeficient node elimination");
 
	/* Then, we perform the element elimination, element by element,
	   cofactor by cofactor. We assume only one polynormial exist.
	   (single output) and only one cofactor exist for the numerator
	   and denominator.  */

	num_cof = poly->num_list;
	den_cof = poly->den_list;
	for(int i=1; i <= max_label; i++)
	{
		elem = ddd_mgr->get_label2elem()->get_elem(i);

        cout<<"Index:"<<i<<" elem:("
			<<elem->get_row()<<","
			<<elem->get_col()
			<<") "<<endl;

		if(elem->get_row() != num_cof->get_row() &&
		   elem->get_col() != num_cof->get_col() )
		{
			num_cof->fcoeff_elem_elim(i,NUM);
		}

		if(elem->get_row() != den_cof->get_row() &&
		   elem->get_col() != den_cof->get_col())
		{
			den_cof->fcoeff_elem_elim(i,DEN);
		}

        fcoeff_GC();
	}

	print_mesg("End of DDD fcoeficient node elimination");

	bool use_simp = 0;
	fcoeff_freq_sweep(use_simp);
}

/* Perform the elimination of cancellation terms The four index is the
    DDD index for fcofficients.  There are position is as follows:
    
	ind1    ind2
    ind3    ind4
    So, the valid combination is ind1*ind4 and ind2*ind3

*/
void
ACAnalysisManager::fcoeff_decancel
(
	int ind1, 
	int ind2, 
	int ind3, 
	int ind4
)
{
	Cofactor    *num_cof, *den_cof;
        
	if(!poly)
	{
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return;
	}
    
	if(!theFLabMap)
	{
		error_mesg(INT_ERROR,"Not fcoefficient exist, abort!");
		return;
    }
         
	/* Then, we perform the decancellation, element by element,
	   cofactor by cofactor. We assume only one polynormial exist.
	   (single output) and only one cofactor exist for the numerator
	   and denominator.  */
	num_cof = poly->num_list;
	den_cof = poly->den_list;
    fcoeff_GC();
    num_cof->fcoeff_decancel(ind1, ind2, ind3, ind4);
    fcoeff_GC();
    den_cof->fcoeff_decancel(ind1, ind2, ind3, ind4);
    fcoeff_GC();
}


/* Perform the eliminations of circuit elements in the complex
   DDDs. We will remove all the appearances of the element admittances
   with error limits.

    ind1    ind2
    ind3    ind4

*/
void
ACAnalysisManager::whole_elem_elim
(
    char *bname, 
	int ind1, 
	int ind2, 
	int ind3, 
	int ind4
)
{
	Cofactor    *num_cof, *den_cof;
	double         *freq = command->freq_pts;
	complex        *num_val = poly->num_val; 
	complex        *den_val = poly->den_val; 
	if(!poly)
	{
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return;
	}
         
	/* Then, we perform the elimnation, element by element, cofactor
	   by cofactor. We assume only one polynormial exist.  (single
	   output) and only one cofactor exist for the numerator and
	   denominator.  */
	num_cof = poly->num_list;
	den_cof = poly->den_list;

    matrix->calc_value(NUM);
    num_cof->whole_elem_elim
		(
			freq, num_val, num_pts, NUM, bname,
			ind1, ind2, ind3, ind4
		);

    matrix->calc_value(DEN);  
	den_cof->whole_elem_elim
		(
			freq, den_val, num_pts, DEN, bname,
			ind1, ind2, ind3, ind4
		);
}

/* Perform the eliminations of circuit elements in the complex DDDs
    for both numerator and denominator.  We will remove all the
    appearances of the element admittances with error limits.

    ind1     ind2
    ind3    ind4

*/
int
ACAnalysisManager::whole_elem_elim_both
(
    char *bname, 
	int ind1, 
	int ind2, 
	int ind3, 
	int ind4
)
{
    
	double         *freq = command->freq_pts;
    if(!poly)
	{
    	error_mesg(INT_ERROR,"Not expression exist, abort!");
       	return 0;
    }
         
	return poly->whole_elem_elim_both
		(
			ddd_mgr, matrix, 
			freq, num_pts, 
			bname, 
			ind1, ind2, ind3, ind4
		);
}

void
ACAnalysisManager::factor_ddd_node()
{
    double         *freq = command->freq_pts;
 
	if(!poly)
	{
    	error_mesg(INT_ERROR,"Not expression exist, abort!");
        return;
    }

	bool enBranchElim = 1;
	bool useSimpExp = 1;
	freq_sweep(useSimpExp, enBranchElim);
    
	poly->factor_ddd_node(ddd_mgr, matrix,freq, num_pts);

#ifdef _DEBUG
    enBranchElim = 1;
	useSimpExp = 1;
    freq_sweep(useSimpExp, enBranchElim);
#endif
}
    
/*  we suppress all the coefficients whose numeric size is smaller
    than a fraction of numerator or denominators for all the frequency
    points.  The function assumes the completion of frequency
    sweepping and coeffcients construction. 
 */
void
ACAnalysisManager::fcoeff_suppress()
{
     
    ExpList * poly_list;
    double *freq = command->freq_pts;
    
    print_mesg("Beginning of coefficient elimination");
    
	if(!poly)
	{
        error_mesg(INT_ERROR,"Now symbolic polynomial expressions derived");
        return;
    }
    for (int i = 0; i < num_pts; i++)
	{
		poly_list = poly;
		for(; poly_list ; poly_list = poly_list->next)
		{
			poly_list->fcoeff_supp_expr( freq[i], opAlpha,i);
		}
    }

    print_mesg("End of coefficient elimination");
}

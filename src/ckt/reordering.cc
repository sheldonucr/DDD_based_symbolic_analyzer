/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 1998-2003 (c) Copyright

   Department of Electrical Engineering, 
   University of California at Riverside.

   email: stan@ee.ucr.edu

*******************************************************
*/


/*
 *    $RCSfile$
 *    $Revision$
 *    $Date$
 */

/*
 * reordering.cc:
 * 
 *    Functions for perform the dynamic variable reordering to
 *    reduce the complex DDD sizes. The main routing for adjacent 
 *    variable swap are in semi_symb.cc
 *
 */

/* INCLUDES: */

#include "globals.h"
#include "circuits.h"
#include "control.h"
#include "complex.h"
#include "node.h"
#include "branch.h"
#include "mxexp.h"
#include "ddd_base.h"
#include "rclcompx.h"
#include "acan.h"
#include "fcoeff.h"

//#define _DEBUG

////////////////////////////////////////////////////////////////////////
//
//  Semi-Symbolic analysis via DDD varaible reordering
//
////////////////////////////////////////////////////////////////////////



/* Algorithm: Select one variable at a time and move it to the best
 * position assuming other variable's positions unchnaged. This
 * process is repeated for all the variables whose size are
 * significant (>10%) of the total DDD size.  */

typedef  struct _LabSize
{
	int lab;
	int size;
} LabSize;

bool theLabeSizeSortFunc
(
	LabSize *A,
	LabSize *B
)
{
	if(A->size > B->size)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	return 0;
}
void
DDDmanager::ComplexDDDMinimizationByReordering()
{			
	
	// make sure the complex DDD has been constructed
	assert(label2elem);	

	vector<LabSize *> labSizeVec;

	// first, find the set of all the variables whose DDD size are
	// significant.
	float total_num = 0;
	for(integer i = label2elem->get_size(); i>=0; i--)
	{
		total_num += unique[i]->Count();
	}

	for(integer i = label2elem->get_size(); i>=0; i--)
	{
		float ratio = unique[i]->Count()/total_num; 
		
		// if the DDD size of the variable > 1%, we will work on.
		if(ratio > 0.01)
		{
			LabSize *newItem = new LabSize();
			newItem->lab = i;
			newItem->size = unique[i]->Count();

			labSizeVec.push_back(newItem);
		}
	}

	// sort the labSizeVec in a decreasing order
	sort(labSizeVec.begin(), labSizeVec.end(), theLabeSizeSortFunc);

	for(integer j = 0; j < (int)labSizeVec.size(); j++ )
	{
		
		int lab = labSizeVec[j]->lab;
		cout <<"idx: " << lab << " size: " 
			 << labSizeVec[j]->size << endl;
		// ignore indices which are not used.
		if(unique[lab]->Count() == 0)
		{
			continue;			
		}
						
		MNAElement *elem =  label2elem->get_elem(lab);
		assert(elem);
		cout <<"process elem: (label= " << lab <<" ) (" << elem->get_row() 
			 << "," << elem->get_col() <<") ";
		elem->get_blist()->print_branch(cout);
		cout <<endl;
					
		// we do the swap for variables in -/+ 10 variable away
		int low_limit = lab - 20 >= 1? lab - 20:1;
		int high_limit = lab + 20 > label2elem->get_size()?
			label2elem->get_size():lab+20;

		cout <<"low_limit: " << low_limit << endl;
		cout <<"high_limit: " << high_limit << endl;
		
		// first, move the variable to the top
		int new_lab = high_limit;
		if(high_limit > lab)
		{
			new_lab = SwapTwoVariableOrder(lab, high_limit);
		}

		//perform GC();
		this->GC();

		// find the best label for the variable from it new label
		// down to low_limit.
		int new_index = new_lab;
		int net_change = 0;
		int max_change = SHRT_MAX;
		int best_label = -1;
		for(int k = new_lab - 1; k >=  low_limit; k--)
		{
			if(unique[k]->Count() == 0)
			{
				continue;
			}

			int count_b = unique[k]->Count() 
				+ unique[new_index]->Count();

			SwapAdjacentVariableOrder(k, new_index);
			
			// swap the two index in the FLabmap			
			label2elem->swap_two_labels(k, new_index);			   

			this->GC();

			int count_a = unique[k]->Count() 
				+ unique[new_index]->Count();

			net_change = net_change + count_a - count_b;

			cout <<"net change: " << net_change << endl;
			if(net_change < max_change)
			{
				max_change = net_change;
				best_label = k;
			}			
			
			new_index = k;
		}

		// move the variable to the best index found.
		if(best_label > new_index)
		{
			cout <<" move the element from " << new_index 
				 <<" to "<< best_label << endl;
			SwapTwoVariableOrder(new_index, best_label);
		}

		cout <<"The final index for the element is: " 
			  << best_label << endl;
		this->GC();
	}

	//. release memory
	for(int i = 0; i < (int)labSizeVec.size(); i++)
	{
		LabSize *aux = labSizeVec[i];
		delete aux;
	}
}


/*
 * 
 * Perform a number of adjacent variable swaps.
 * Return the new index for index1 */
int
DDDmanager::SwapTwoVariableOrder
(
	int index1,
	int index2
) 
{	
	// first make sure we don't get corrupted data.	
	assert(index1 < num_inputs);
	assert(index2 < num_inputs);

	// index1 is i-1 and index2 is i.
	assert(index1 < index2);

	int new_index = index1;
	for(integer i = index1+1; i <= index2; i++)	
	{
		if(unique[i]->Count() == 0)
		{
			continue;
		}
		

		SwapAdjacentVariableOrder(new_index, i);

		// swap the two index in the FLabmap
		label2elem->swap_two_labels(new_index, i);

		new_index = i;
	}

	return new_index;
}


/* 
 * Minimize the sizes of complex DDDs by dynamic ordering. 
 */

void
ACAnalysisManager::complex_ddd_minimization_via_reordering()
{	

	print_mesg("perform complex DDD minimization ... ");

	// first, do the GC
	//ddd_mgr->GC();

	// reorder all the symbolic variables such that they will appear
	// on the top of all s-expanded DDD trees.
	double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

	ddd_mgr->ComplexDDDMinimizationByReordering();

	time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"complex DDD reordering time: " << c_time <<endl;

	print_mesg("End of complex DDD minimization. ");

	freq_sweep(FALSE, FALSE);

}




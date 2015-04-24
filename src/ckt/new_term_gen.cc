/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 2002 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 5/14/2001 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: new_term_gen.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/06/08 14:24:35 $
 */


/* new_term_gen.cc 
 *
 * Use a new dominant term generation algorithm proposed by
 * W. Verhaegen and G. Gielen in DAC01 tittled " Efficient DDD-based
 * symbolic analysis of large linear analog circuits"

 * The main idea is to record the all dominant terms at each DDD node
 * in a decreasing order to remember all the dominant terms of a DDD
 * tree.  The new algorithm trades the space for speed and it should be
 * faster than the shortest path algorithm (O(n) vs O(|DDD|), n is the
 * size of the term in terms of symbols and |DDD| is the size of DDD
 * tree in terms of DDD vertices.)
 *
 *
 *       X.-D. Sheldon Tan  May. 14, 2002
 * */

/* INCLUDES: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "globals.h"
#include "ddd_base.h"
#include "control.h"
#include "node.h"
#include "branch.h"
#include "complex.h"
#include "rclcompx.h"
#include "mna_form.h"
#include "mxexp.h"
#include "dtstruct.h"
#include "approx.h"
#include "acan.h"

//#define _DEBUG

#ifdef __USE_SECOND_TERM_GENERATION


/*
** set tm_index to 0 for all the DDD vertices downstream
*/
void
DDDmanager::FCSecondInitialize
(
	DDDnode *s
)
{

#ifdef _DEBUG
	cout << "Initialize DDD " 
		 << s->index << " ..." 
		 << endl;
	//statistic(s);
#endif

	HasherTree *ht = new HasherTree(FALSE);
	assert(ht);

	 FCSecondInitializeR(s, ht);

	 delete ht;	
}

void
DDDmanager::FCSecondInitializeR
(
	DDDnode *s,
	HasherTree *ht	
)

{	

	if(!s)
	{
        return;
	}

	if((s == zero) || (s == one))
	{
        return;
	}

	if(!ht->Put(*s, s))
	{
		return;
	}

#ifdef _DEBUG
	cout << "intialize D index: " << s->index 
		 << " add: << " << s << endl;
#endif

	// initialize status
    s->tm_index = 0;

	if(!s->terms_stack)
	{
		s->terms_stack = new DDDstack();
	}
	else
	{
		s->terms_stack->ClearStack();		
	}
	
    if(s->Then() != one)
	{
        FCSecondInitializeR(s->Then(), ht);
    }
    if(s->Else() != zero)
	{
        FCSecondInitializeR(s->Else(), ht);
    }
	
}

/* Find the one new dominant term. We assume that the given DDD tree
** is inialized.  The return path is represented as DDD tree.  */

DDDnode *
DDDmanager::FCSecondGetOneDominantTerm
(
	DDDnode *s
)
{
	assert(s);

	if(s == one)
	{
		return one;
	}
	int index = s->terms_stack->StackSize();
	
	DDDnode *new_term = FCSecondGetDominantTermByIndex(s, index);

	return new_term;
}

/* Find the one dominant term according to the given index. We assume
** that the given DDD tree is inialized.  The return path is
** represented as DDD tree.  */

DDDnode * 
DDDmanager::FCSecondGetDominantTermByIndex
(
	DDDnode *s,
	int index
)
{
	assert(s);
	assert(index >= 0);

	if(s == one)
	{
		return one;
	}
	else if(s == zero)
	{
		return NULL;
	}
	else if(s->terms_stack->GetTermByIndex(index))
	{
		return s->terms_stack->GetTermByIndex(index);
	}
	else
	{
		FCSecondGenerateNextTerm(s, index);		
	}

	if(!s->terms_stack->GetTermByIndex(index))
	{							

#ifdef _DEBUG	
		int num_path = (int)PathsInFunction(s);		
		cout <<"Node: " << s->index 
			 <<" num_paths: " << num_path
			 <<" stack size: " <<  s->terms_stack->StackSize()
			 << endl;
#endif
		
	}
	
	return s->terms_stack->GetTermByIndex(index);

}

void
DDDmanager::FCSecondGenerateNextTerm
(
	DDDnode *s,
	int  index
)
{

	assert(s);
	assert(index >= 0);
	

	if(s == one)
	{
		return;
	}

	while(!s->terms_stack->GetTermByIndex(index))
	{

		DDDnode *node = s;
		DDDnode *max_term = NULL;
		DDDnode *node_for_update = NULL;

		double  max_value = -1;

		// go through all the DDD vertices linked by 0-edges to find
		// out the available terms.
		for(; node != zero; node = node->Else())
		{

#ifdef _DEBUG			
			cout << "D index: " << node->index 
				 << " given index: " << index 
				 << " tm_index: " << node->tm_index << endl;
#endif
			DDDnode *child = node->Then();
			DDDnode *term = NULL;

			if((child == one) && (node->tm_index >= 1))
			{
				// the node does not have any path downstream				
				continue;
			}
					
			if(child == one)
			{
				term = one;
			}
			else
			{
				term = FCSecondGetDominantTermByIndex
					(
						child, 
						node->tm_index
					);
			}
						
			if(term)
			{
				DDDnode *new_term = Change(term, node->index);
				bool compute_sign = false;
				double path_val =  fabs(FCPathValue(new_term, compute_sign));			
				if(path_val >= max_value)
				{
					max_term = new_term;
					max_value = path_val;
					node_for_update = node;
				}
			} 
			else
			{			
				// the node does not have any path downstream
#ifdef _DEBUG
				int num_path = (int)PathsInFunction(node);
				
				cout <<"Node: " << node->index 
					 <<" num_paths: " << num_path
					 <<" stack size: " <<  node->terms_stack->StackSize()
					 << endl;							
#endif			   
			}
		}

		if(max_term && node_for_update)
		{
			s->terms_stack->push(max_term);
			// update the term index of the corresponding DDD node.
			node_for_update->tm_index++;		
		}
		else
		{
			// means no term can be generated, we have to bail out
			break;
		}
	}

}


#endif

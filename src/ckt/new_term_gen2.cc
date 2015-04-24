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
 *    $RCSfile: new_term_gen2.cc,v $
 *    $Revision: 1.4 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/* new_term_gen2.cc 
 *
 * Use a new dominant term generation algorithm based on 
 * shortest path algorithm. 

 * The main idea is to only update newly generated DDD vertices
 * and associated old vertices instead of finding the new 
 * dominant terms from scratch. In this way, we will be much
 * fast. The time complexity should be O(n), n is the size of 
 * a term.
 *
 *
 *       X.-D. Sheldon Tan  June. 5, 2002
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

#ifdef __USE_THIRD_TERM_GENERATION

/*
** New Diff operation which build a new partial topology list
** for the operation of removing the given 
**/
DDDnode*
DDDmanager::DiffAndTopoSort
(
	DDDnode* P, 
	DDDnode* Q, 
	DDDUniqueStack & topoList
) 
{

	//DDDcomputed1 *c;
	DDDnode* f = NULL;
	//Object* o;


	if( P == zero )
	{
#ifdef _DEBUG
		cout << "P is zero" << endl;
#endif
		return zero;
	}
	if( Q == zero )
	{
#ifdef _DEBUG
		cout << "Q is zero" << endl;
#endif
		return P;
	}
	if( P == Q )
	{
		
#ifdef _DEBUG
		if(P != one)
		{
			cout <<"P: " << P->index <<" == Q: " << Q->index << endl;
			cout <<"P: " << P << endl;
			cout <<"Q: " << Q << endl;
		}
		else
		{
			cout <<"P == Q == one" << endl;
		}
#endif
		
		return zero;
	}

#ifdef _DEBUG	
	cout <<"P: " << P << endl;
	cout <<"Q: " << Q << endl;
#endif	

	if( P->index > Q->index )
	{
		DDDnode *Dthen = P->Then();
		DDDnode *Delse = DiffAndTopoSort(P->Else(), Q, topoList);

		f = GetNode
			(
				P->Index(), 
				Dthen,
				Delse
			);
		
		
		if(P->Else() == Q)
		{
			// update parents_stack from P->Else() down
			DDDnode *prev;
			DDDnode *node = prev = P->Else();
			
			// we can only remove the node when its 
			// # of parent is just one.
			if(node->parents_stack->StackSize() == 1)
			{
				while(node != one)
				{			
					
					prev = node;
					node = node->Then();				
					if(prev->Then()->parents_stack->remove(prev))
					{
#ifdef _DEBUG
						cout << "2D:" << prev->index 
							 <<" " << (void *)prev << " is removed from " 
							 << prev->Then()->index 
							 <<" " << (void *)(prev->Then()) << endl;
#endif
					}
					if(prev->Else()->parents_stack->remove(prev))
					{
#ifdef _DEBUG
						cout << "2D:" << prev->index 
							 <<" " << (void *)prev << " is removed from " 
							 << prev->Else()->index 
							 <<" " << (void *)(prev->Else()) << endl;
#endif
					}
				
					if(node->parents_stack->StackSize() > 0)
					{
						break;
					}
			
				}
			}
			
			// We need to update all the nodes down for their distance
			// value only.
			HasherTree *ht    = new HasherTree(FALSE);
			DFS_TRACE_2(node, topoList, ht);			
			delete ht;			

		}

		// must be done after if(P->Else() == Q) clause
		if(P != f)
		{
			// f is a new DDD, so need to update old
			// DDD's children parent stack.
			if(P->Then()->parents_stack->remove(P))
			{
#ifdef _DEBUG
				cout << "1D:" << P->index 
					 <<" " << (void *)P << " is removed from " 
					 << P->Then()->index 
					 <<" " << (void *)(P->Then()) << endl;
#endif
			}
			if(P->Else()->parents_stack->remove(P))
			{
#ifdef _DEBUG
				cout << "1D:" << P->index 
					 <<" " << (void *)P  << " is removed from " 
					 << P->Else()->index 
					 <<" " << (void *)(P->Else()) << endl;
#endif
			}
			if(P->parents_stack->StackSize() > 1)
			{
					HasherTree *ht    = new HasherTree(FALSE);
					DFS_TRACE_2(P, topoList, ht);			
					delete ht;
			}

			// update parent stack in f's two children
			f->Then()->parents_stack->push(f);
#ifdef _DEBUG		
			cout <<"new D: " << f->index << " become parent of " 
				 << f->Then()->index << endl;
#endif
	
			f->Else()->parents_stack->push(f);
#ifdef _DEBUG		
			cout <<"new D: " << f->index << " become parent of " 
				 << f->Else()->index << endl;
#endif
	
		}

		
	}
	else if( P->index < Q->index )
	{
		f =  DiffAndTopoSort( P, Q->Else(), topoList);
		if(P != f)
		{
			// f is a new DDD, so need to update old
			// DDD's children parent stack.
			if(P->Then()->parents_stack->remove(P))
			{
#ifdef _DEBUG
				cout << "3D:" << P->index 
					 <<" " << (void *)P << " is removed from " 
					 << P->Then()->index 
					 <<" " << (void *)(P->Then())<< endl;
#endif
			}
			if(P->Else()->parents_stack->remove(P))
			{
#ifdef _DEBUG				
				cout << "3D:" << P->index 
					 <<" " << (void *)P << " is removed from " 
					 << P->Else()->index 
					 <<" " << (void *)(P->Else()) << endl;
#endif
			}
			if(P->parents_stack->StackSize() > 1)
			{
					HasherTree *ht    = new HasherTree(FALSE);
					DFS_TRACE_2(P, topoList, ht);			
					delete ht;
			}

			// update parent stack in f's two children
			f->Then()->parents_stack->push(f);
#ifdef _DEBUG
			cout <<"new D: " << f->index << " become parent of " 
				 << f->Then()->index << endl;
#endif
	
			f->Else()->parents_stack->push(f);
#ifdef _DEBUG
			cout <<"new D: " << f->index << " become parent of " 
				 << f->Else()->index << endl;	
#endif			
		}
	}
	else
	{
		DDDnode *Dthen = DiffAndTopoSort(P->Then(), Q->Then(), topoList);
		DDDnode *Delse = DiffAndTopoSort(P->Else(), Q->Else(), topoList);

		f = GetNode
			(
				P->Index(),
				Dthen,
				Delse
			);
		
		if(P->Then() == Q->Then())
		{
			// in this case, P will be gone.
			/*
			cout <<"D: " << P->Then()->index << " stack size: " 
				 << P->Then()->parents_stack->StackSize() 
				 << endl;
			*/
			if(P->Then()->parents_stack->remove(P))
			{
#ifdef _DEBUG
				cout << "5D:" << P->index 
					 <<" " << (void *)P << " is removed from " 
					 << P->Then()->index 
					 <<" " << (void *)P->Then() << endl;
#endif
			}
			if(P->Else()->parents_stack->remove(P))
			{
#ifdef _DEBUG
				cout << "5D:" << P->index 
					 <<" " << (void *)P << " is removed from " 
					 << P->Else()->index 
					 <<" " << (void *)(P->Else()) << endl;
#endif
			}

			HasherTree *ht    = new HasherTree(FALSE);
			DFS_TRACE_2(P->Then(), topoList, ht);			
			delete ht;
		
		}

		// must be done after if(P->Then() == Q->Then())
		if(P != f)
		{
			// f is a new DDD, so need to update old
			// DDD's children parent stack.
			if(P->Then()->parents_stack->remove(P))
			{
#ifdef _DEBUG
				cout << "4D:" << P->index 
					 <<" " << (void *)P << " is removed from " 
					 << P->Then()->index 
					 <<" " << (void *)(P->Then()) << endl;
#endif
			}		
			if(P->Else()->parents_stack->remove(P))
			{
#ifdef _DEBUG
				cout << "4D:" << P->index 
					 <<" " << (void *)P << " is removed from " 
					 << P->Else()->index 
					 <<" " << (void *)(P->Else())<< endl;
#endif
			}
			// P has more than two incoming 1-edge, so it will be
			// duplicated and f is the duplicate. The old P downstream
			// should be updated for thier distance value.
			if(P->parents_stack->StackSize() > 1)
			{
					HasherTree *ht    = new HasherTree(FALSE);
					DFS_TRACE_2(P, topoList, ht);			
					delete ht;
			}
	
			// update parent stack in f's two children
			f->Then()->parents_stack->push(f);
#ifdef _DEBUG
			cout <<"new D: " << f->index << " become parent of " 
				 << f->Then()->index << endl;
#endif
			
			f->Else()->parents_stack->push(f);
#ifdef _DEBUG
			cout <<"new D: " << f->index << " become parent of " 
				 << f->Else()->index << endl;
#endif	
		}		

		
	
	}

	topoList.DDDpush(f);
#ifdef _DEBUG
	cout << "f: pushed:" << f << endl;
#endif

	// do some initialization.	
	if(!f->parents_stack)
	{
		f->parents_stack = new DDDUniqueStack();
		f->parents_stack->ClearStack();	
	}
	
	// update its children parent stacks
	f->Then()->parents_stack->push(f);
	f->Else()->parents_stack->push(f);

	if(!f->val_ptr)
	{
        f->val_ptr = new complex(0.0);	
	}
	f->val_ptr[0] = INF_WEIGHT;
	f->pi = NULL;

	return f;
}

/* Initialization function for second shortest path term generation
** algorithm.
**/     
void
DDDmanager::DFS_ISS_2
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

	if(!s->parents_stack)
	{
		s->parents_stack = new DDDUniqueStack();
	}
	else
	{
		s->parents_stack->ClearStack();		
	}

    s->val_ptr[0] = INF_WEIGHT;
    s->pi = NULL;
	

	if((s == zero) || (s == one))
	{
        return;
	}
    if(s->Then()->color == dWHITE)
	{
        DFS_ISS_2(s->Then());		
    }
    if(s->Else()->color == dWHITE)
	{
        DFS_ISS_2(s->Else());
    }

   
	s->Then()->parents_stack->push(s);	
#ifdef _DEBUG	
	cout <<"D: " << s->Then()->index << " parent(1): " << s->index 
		 << " stack size:" << s->Then()->parents_stack->StackSize() << endl;
#endif

	s->Else()->parents_stack->push(s);
#ifdef _DEBUG
	cout <<"D: " << s->Else()->index << " parent(0): " << s->index 
		 << " stack size:" << s->Else()->parents_stack->StackSize() << endl;
	
	if(s->Then() == one)
	{
		cout << "D: " << s->index << " point to one" << endl;
		cout << "one stack size: " << one->parents_stack->StackSize() 
			 << endl;
	}
#endif	

	assert(theTopoList);
    theTopoList->DDDpush(s);
}

void
DDDmanager::DFS_TRACE_2
(
	DDDnode *s,
	DDDUniqueStack & stack,
	HasherTree *ht
)
{
    if(!s)
	{
        return;
	}   
	
	if(s == zero)
	{		
        return;
	}
	
	if(s == one)
	{		
		stack.push(s);
#ifdef _DEBUG
		cout << "f: down pushed: ONE" <<  endl;
#endif
        return;
	}

	if(!ht->Put(*s,s))
	{// Already  seen
		return;
	}

    s->val_ptr[0] = INF_WEIGHT;

	DFS_TRACE_2(s->Then(), stack, ht);		
    
	DFS_TRACE_2(s->Else(), stack, ht);

	s->Then()->parents_stack->push(s);

#ifdef _DEBUG
	cout <<"D: " << s->index << " become parent of " << s->Then()->index << endl;
#endif	

	s->Else()->parents_stack->push(s);

#ifdef _DEBUG
	cout <<"D: " << s->index << " become parent of " << s->Else()->index << endl;
#endif

    stack.push(s);

#ifdef _DEBUG
	cout << "f: down pushed: " << s->index <<  endl;
#endif

}
   
/*  Second shortest path term generation algortithm. It assume that
**  the first dominant term has been found prior. Find the shortest
**  path from root s to the 1-terminal in the given DDD.  It returns
**  the found path in DDD form.  It is used for fully expanded
**  coefficient DDD. Also note that once the path is found, we remove
**  it from s, so s will be changed.
**/

DDDnode *
DDDmanager::FCDDDShortestPath_2
(
	DDDnode **s
)
{
    DDDnode *ddd_aux;
    if(*s == zero)
	{
        return zero;
	}
#ifdef _DEBUG    
    cout <<"Extract a new path ... " << endl;
#endif
	DDDnode * path = ExtractPath();

#ifdef _DEBUG	
	fcoeff_print_symbolic_path(path, cout);	
#endif
	
	if(path)
	{		
		static DDDUniqueStack * topoList = new DDDUniqueStack;
		topoList->ClearStack();

#ifdef _DEBUG
		cout <<"Remove the new path from DDD tree ... " << endl;
#endif
		
		*s = DiffAndTopoSort(*s, path, *topoList);
		(*s)->val_ptr[0] = 0;

#ifdef _DEBUG
		cout <<"\n Relaxing DDD tree ... " << endl;
#endif

		if(!topoList->isEmpty())
		{ 
	   
			while((ddd_aux = topoList->DDDpop()) != NULL)
			{			   
				
				// if this is the last one, we do forward relaxing
				// first.

#ifdef _DEBUG							
				cout << "\n relaxing node: " << ddd_aux << endl;
				cout << "backforward ..." << endl;
#endif
				int size = ddd_aux->parents_stack->StackSize();
				
				// the value of the first DDD should be zero
				if(size > 0)
				{
					if(ddd_aux != *s)
					{
						ddd_aux->val_ptr[0] = INF_WEIGHT;
					}
					
					for(int i = 0; i < size; i++)
					{
						DDDnode *parent =
							ddd_aux->parents_stack->GetTermByIndex(i);
						assert(parent);
						
						FCRelax(parent, ddd_aux);
					}
				}
				
				
				if(ddd_aux != one)
				{
#ifdef _DEBUG
					cout << "forward ..." << endl;
#endif

					FCRelax(ddd_aux, ddd_aux->Then());
					FCRelax(ddd_aux, ddd_aux->Else());
				}
			}
		}
		
    }
	
	return path;
}

#endif //__USE_THIRD_TERM_GENERATION 

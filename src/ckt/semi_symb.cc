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
 *    $RCSfile: semi_symb.cc,v $
 *    $Revision: 1.1 $
 *    $Date: 2002/11/21 02:40:15 $
 */

/*
 * semic_symb.cc:
 * 
 *    Functions for semi-symbolic analysis of analog circuits
 *    via DDD graph. We implement two methods:
 *   (1)  MTDDD method by Tao Pi and Richard Shi (DAC00).
 *   (2)  Variable reordering based method.
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



/*
 * Move all symbolic variables to the top of the DDD grpahs via
 * dynamic DDD variable reordering. For efficient operations, we first
 * perform the CG to remove all the unused DDD vertices.
 */
void
DDDmanager::ComplexDDDVariableReorder()
{			
	// Then do the adjacent variable order swap for each symbolic
	// variable(index) which is below any numerical variable(index).
	
	int sym_index = num_inputs-1; // first symbolic index
	int num_index = 0; // first numerical index

	// make sure the complex DDD has been constructed
	assert(label2elem);	

	// we only take care of the complex DDD port of the unique
	for(integer i = label2elem->get_size(); i>=0; i--)
	{
		
		// ignore indices which are not used.
		if(unique[i]->Count() == 0)
		{
			continue;			
		}
								
		MNAElement *elem =  label2elem->get_elem(i);
		assert(elem);
		
		if(elem->is_symbolic())
		{
			sym_index = i;
			//cout <<"symbolic index: " << i << endl;
		}
		else
		{
			//cout <<"numerical index: " << i << endl;
			// we only remember the highest numerical index
			if(i > num_index)
			{
				num_index = i;
			}
		}

		//cout <<"before symbolic_idx: " << sym_index << endl;
		//cout <<"before num_idx:" << num_index << endl;
		
		if(sym_index < num_index)
		{
			ComplexDDDSwapVariableOrder(sym_index, num_index);
			
			// then find the new sym and num indices after variable swap.
			int prev_sym_index = sym_index;			
			sym_index = num_index;

			// the next numerical variable index
			int j = 0;			
			for(j = num_index-1; j >= prev_sym_index; j--)
			{
				if((unique[j]->Count() != 0))
				{
					//cout <<"index: " << j << "is not empty" << endl;
					
					MNAElement *elem =  label2elem->get_elem(i);
					assert(elem);
					if(!elem->is_symbolic())
					{
						break;
					}					
				}
			}
						
			num_index = j;
				   			
			//cout <<"after symbolic_idx: " << sym_index << endl;
			//cout <<"after num_idx:" << num_index << endl;
		
		}
		// do the GC
		//GC();		
	}
}


/*
 * 
 * Perform a number of adjacent variable swaps
 */
void
DDDmanager::ComplexDDDSwapVariableOrder
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

	int sym_index = index1;
	for(integer i = index1+1; i <= index2; i++)
	{
		if(unique[i]->Count() == 0)
		{
			continue;
		}
		
		SwapAdjacentVariableOrder(sym_index, i);

		// swap the two index in the FLabmap
		label2elem->swap_two_labels(sym_index, i);

		sym_index = i;
	}
}


/* simplify the expressions represented by coefficient list up to the
   error criteria for both magnitude and phase. 
 */

void
ACAnalysisManager::complex_ddd_semi_variable_reordering()
{	

	print_mesg("perform  complex DDD reordering ... ");

	// first, do the GC
	//ddd_mgr->GC();

	// reorder all the symbolic variables such that they will appear
	// on the top of all s-expanded DDD trees.
	double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

	ddd_mgr->ComplexDDDVariableReorder();

	time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"complex DDD reordering time: " << c_time <<endl;

	print_mesg("End of complex DDD reordering ");

	freq_sweep(FALSE, FALSE);

}



////////////////////////////////////////////////////////////////////////
//
//  Reordering functions for fcoeff DDDs (s-expanded DDDs)
//
////////////////////////////////////////////////////////////////////////



/*
 * Move all symbolic variables to the top of the DDD grpahs via
 * dynamic DDD variable reordering. For efficient operations, we first
 * perform the CG to remove all the unused DDD vertices.
 */
void
DDDmanager::FcoeffVariableReorder
(
	ACAnalysisManager    *ac_mgr
)
{
		
	
	// Then do the adjacent variable order swap for each symbolic
	// variable(index) which is below any numerical variable(index).
	
	int sym_index = num_inputs-1; // first symbolic index
	int num_index = 0; // first numerical index

	// assume the fcoeff ddds are constructed.
	assert(theFLabMap);

	for(integer i=(num_inputs-1); i>=0; i--)
	{
		
		// ignore indices which are not used.
		if(unique[i]->Count() == 0)
		{
			continue;			
		}

		// ignore index correspdonding to complex DDD indices
		if(i <= theFLabMap->get_offset())
		{
			continue;			
		}
						
		
		DummBranch *dbch =  theFLabMap->get_dummbranch(i);
		assert(dbch);
		
		if(dbch->is_symbolic())
		{
			sym_index = i;
			//cout <<"symbolic index: " << i << endl;
		}
		else
		{
			//cout <<"numerical index: " << i << endl;
			// we only remember the highest numerical index
			if(i > num_index)
			{
				num_index = i;
			}
		}

		//cout <<"before symbolic_idx: " << sym_index << endl;
		//cout <<"before num_idx:" << num_index << endl;
		
		if(sym_index < num_index)
		{
			FcoeffSwapVariableOrder(sym_index, num_index);
			
			// then find the new sym and num indices after variable swap.
			int prev_sym_index = sym_index;			
			sym_index = num_index;

			// the next numerical variable index
			int j = 0;			
			for(j = num_index-1; j >= prev_sym_index; j--)
			{
				if((unique[j]->Count() != 0))
				{
					//cout <<"index: " << j << "is not empty" << endl;
					
					DummBranch *dbch =  theFLabMap->get_dummbranch(j);
					assert(dbch);
					if(!dbch->is_symbolic())
					{
						break;
					}					
				}
			}
						
			num_index = j;
				   			
			//cout <<"after symbolic_idx: " << sym_index << endl;
			//cout <<"after num_idx:" << num_index << endl;
		
		}
		//ac_mgr->fcoeff_GC();		
	}
}


/*
 * 
 * Perform a number of adjacent variable swaps
 */
void
DDDmanager::FcoeffSwapVariableOrder
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

	int sym_index = index1;
	for(integer i = index1+1; i <= index2; i++)
	{
		if(unique[i]->Count() == 0)
		{
			continue;
		}
		
		SwapAdjacentVariableOrder(sym_index, i);

		// swap the two index in the FLabmap
		theFLabMap->swap_two_labels(sym_index, i);

		sym_index = i;
	}
}


/*
**
**  Swap the variable indices index1 and index2 in the 
**  Uniqu table of DDDmanager to change the variable order.
**  Upon success, true is return, false otherwise.

**  The idea for DDD variable reordering is based on the R. Rudell's
**  DAC03's paper -- "Dynamic Variable Ordering" fopr Ordered Binary
**  Decision Diagrams". Let F(x(i), F1, F0) be a node at level i (ith
**  component in unique[]). Let F11 and F10 be the cofacor and the
**  remainder of F1 with respect to x(i-1). F01 and F00 are the
**  cofactor and remainder of F0 with respect to x(i+1). Then we can
**  overwrite x(i) at unique[i] with a new tuple (x(i-1), (x(i),F11,
**  F01), (x(i), F10, F00)). Since we swap the variable index of x(i)
**  and x(i-1), so x(i+1) has the same index of x(i) before the
**  swap. Same is true for x(i). As a result, we only need to update
**  "left" and "right" pointers in the x(i). For the new two tuples
**  (x(i),F11, F01), (x(i), F10, F00), we have to add them into the
**  unique[i-1]. Also, we should remove all the ddd vertex nodes that
**  are not referenced by other nodes.

**  Assumption. (1) Index2 > Index1.
**              (2) Index1 and Index2 are adjacent variables
*/

void
DDDmanager::SwapAdjacentVariableOrder
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

	// if the number of DDD vertices for one of index is zero, do
	// nothing.
	if(!unique[index1]->Count() || !unique[index2]->Count())
	{
		return;		
	}

	HasherTree *unique_save1 = new HasherTree;
	HasherTree *unique_save2 = new HasherTree;

	// First, we overwrite all the index2 vertices which will be reduced to index1 vertices
	// due to ZBDD reduction rule. 
	Hasher_ForEach_Object(*unique[index2], key, object) 
		{
			DDDnode *node = (DDDnode *) key;
#ifdef _DEBUG
			cout <<"process node: " << node << endl;
#endif

			DDDnode *F1 = node->Then();
			DDDnode *F0 = node->Else();
		
			DDDnode *F11 = Subset1(F1, index1);
			DDDnode *F10 = Subset0(F1, index1);
		
			DDDnode *F01 = Subset1(F0, index1);
			DDDnode *F00 = Subset0(F0, index1);

			// create two new nodes and put them into the unique
			// table.
			DDDnode *new_then = GetNode( index1, F11, F01);			
			DDDnode *new_else = GetNode( index1, F10, F00);
			
			//cout <<"new_then: " << new_then << endl;
			//cout <<"new_else: " << new_else << endl;
				
			
			//if((new_then == zero) || (new_then == new_else))
			if(new_then == zero)
			{
				// this means that the node will be suppressed due to
				// zero-suppressed BDD rules. In this case, we have to
				// use new_then or new_else (one of them must hold
				// that new_x->index == index1) to replece the "node".
				
				assert(new_then != new_else);
				
				if(new_then->index == index1)
				{
					node->SetIndex(index1);
					UnKeep(node->Then());
					UnKeep(node->Else());
					
					node->SetLeft(new_then->Then());
					node->SetRight(new_then->Else());
					
					Keep(new_then->Then());
					Keep(new_then->Else());					
				}
				else if(new_else->index == index1)
				{
					node->SetIndex(index1);
					UnKeep(node->Then());
					UnKeep(node->Else());
					
					node->SetLeft(new_else->Then());
					node->SetRight(new_else->Else());
					
					Keep(new_else->Then());
					Keep(new_else->Else());					
				}
				else
				{
					// something must be wrong.
					assert(0);
				}
				
				// then put the node into unique[index1]
				if(unique_save1->Put(*node, node))
					//unique_save1->Put(*node, node);					
				{
#ifdef _DEBUG
					cout <<"node: " << node << " is put into unique[" << index1 << "]" << endl;
#endif 					
				}
				else
				{
#ifdef _DEBUG
					cout <<"node: " << node <<" can't be saved into unique table(1)" << endl;	
#endif
				}				
					
#ifdef _DEBUG
				cout << "node elim: " << node << endl;
#endif
			}
		}
	Hasher_End_ForEach;


	// Then we overwrite all the vertices which can not be reduced in unique[index2]
	Hasher_ForEach_Object(*unique[index2], key, object) 
		{
			DDDnode *node = (DDDnode *) key;
#ifdef _DEBUG
			cout <<"process node: " << node << endl;
#endif

			DDDnode *F1 = node->Then();
			DDDnode *F0 = node->Else();
		
			DDDnode *F11 = Subset1(F1, index1);
			DDDnode *F10 = Subset0(F1, index1);
		
			DDDnode *F01 = Subset1(F0, index1);
			DDDnode *F00 = Subset0(F0, index1);

			// create two new nodes and put them into the unique
			// table.
			DDDnode *new_then = GetNode( index1, F11, F01);			
			DDDnode *new_else = GetNode( index1, F10, F00);
			
			//cout <<"new_then: " << new_then << endl;
			//cout <<"new_else: " << new_else << endl;
				
			
			//if((new_then != zero) && (new_then != new_else))
			if((new_then != zero))
			{

				// then update the node
				if(new_then != zero && new_else != one )
				{

					// we need to create new index1 vertices as
					// the old ones will be come index2 vertices.
					if(new_then->index == index1)
					{
						// we need to create a new DDD vertex
						boolean   dc = ( F11 == F01 && F01->isDC() && F01->Index()+1 == index1);
						DDDnode *new_then_tmp = new DDDnode(index1, F11, F01, dc);

						//cout <<"new_then_tmp: " << new_then_tmp << endl;
							
						DDDnode *p;						
						if(  (p = (DDDnode*) unique_save1->Get( new_then_tmp)) ) 
						{  //If already in the unique save table
							delete new_then_tmp;
							new_then = p;								
						} 
						else
						{						
							new_then = new_then_tmp;
								
							unique_save1->Put(*new_then, new_then);
							//unique_save1->ForcePut(*new_then, new_then);								
							num_nodes++;                           //# of nodes increases
							Keep(F11);
							Keep(F01);
#ifdef _DEBUG
							cout <<"a new new_then: " << new_then << " is put into unique[" << index1 << "]" << endl;								
#endif
						}
					}
					if(new_else->index == index1)
					{							
						boolean   dc = ( F10 == F00 && F00->isDC() && F00->Index()+1 == index1);
						DDDnode *new_else_tmp = new DDDnode(index1, F10, F00, dc);

						//cout <<"new_else_tmp: " << new_else_tmp << endl;																			
						
						DDDnode *p;						
						if(  (p = (DDDnode*) unique_save1->Get( new_else_tmp)) ) 
						{  //If already in the save unique table
							delete new_else_tmp;
							new_else = p;								
						} 
						else
						{						
							new_else = new_else_tmp;								
							unique_save1->Put(*new_else, new_else);
							//unique_save1->ForcePut(*new_then, new_then);								
							num_nodes++;                           //# of nodes increases
							Keep(F10);
							Keep(F00);
#ifdef _DEBUG
							cout <<"a new new_else: " << new_else << " is put into unique[" << index1 << "]" << endl;								
#endif
						}							
					}

					UnKeep(node->Then());
					UnKeep(node->Else());
					node->SetLeft(new_then);
					node->SetRight(new_else);
					Keep(new_then);
					Keep(new_else);					
		
					// then overwrite the node on unique[index2]
					if(unique_save2->Put(*node, node))
						//unique_save2->Put(*node, node);						
					{
#ifdef _DEBUG							
						cout <<"node: " << node << " is put into unique[" << index2 << "]" << endl;
#endif
					}
					else
					{
#ifdef _DEBUG
						cout <<"node: " << node <<" can't be saved into unique table(2)" << endl;	
#endif
					}					
						
				}
#ifdef _DEBUG
				cout << "overwrite: " << node << endl;
#endif
			} 
		}
	Hasher_End_ForEach;

	// change all vertices in unique[index1] to index2 vertices.
	Hasher_ForEach_Object(*unique[index1], key, object) 
		{
			DDDnode *node = (DDDnode *) key;
									
			//cout <<"process node (correlated): " << node << endl;
			node->SetIndex(index2);
			//cout <<"after: " << node << endl;
			if(unique_save2->Put(*node, node))
			{
#ifdef _DEBUG
				cout <<"node: " << node << " is put into unique[" << index2 << "]" << endl;
#endif 					
			}
			else
			{
#ifdef _DEBUG
				cout <<"node: " << node <<" can't be saved into unique table(2)" << endl;				
#endif
			}
									
		}
	Hasher_End_ForEach;


	delete unique[index1];
	unique[index1] = unique_save1;

	delete unique[index2];
	unique[index2] = unique_save2;
	
}


/* simplify the expressions represented by coefficient list up to the
   error criteria for both magnitude and phase. 
 */

void
ACAnalysisManager::fcoeff_semi_symbolic_analysis()
{
	ExpList * poly_list = poly;
	//double *freq = command->freq_pts;
 
	print_mesg("perform semi symbolic analysis ... ");

	// first, do the GC
	this->fcoeff_GC();

	// reorder all the symbolic variables such that they will appear
	// on the top of all s-expanded DDD trees.
	double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    double c_time = 0.0, e_time = 0.0;	

	ddd_mgr->FcoeffVariableReorder(this);

	time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    

	HasherTree *prec_ht    = new HasherTree(FALSE);
	HasherTree *ht    = new HasherTree(FALSE);

	// for statisitic collection
	HasherTree *node_ht    = new HasherTree(FALSE);
	HasherTree *term_ht    = new HasherTree(FALSE);
	// go through all the polynomails
	for(poly_list = poly; poly_list; poly_list = poly_list->next)
	{
        /* build the coefficient for numerator */
		cout <<"\nRecomputing the coefficient for numerator"<<endl;

		Cofactor *cof_aux;
        for(cof_aux = poly_list->num_list; cof_aux; cof_aux = cof_aux->next)
		{

			time1 = 1.0 * clock() / CLOCKS_PER_SEC;
			cof_aux->fcoeff_list->calc_fcoeff_list(prec_ht, ht);
			time2 = 1.0 * clock() / CLOCKS_PER_SEC;
			e_time = e_time + time2 - time1;
#ifdef COLLECT_COEFF_DATA
			cout <<"\nStatisitic for numerator coefficient list"<<endl;
			cof_aux->fcoeff_list->semi_symbolic_statistic(cout, node_ht, term_ht);			
			//cof_aux->fcoeff_list->statistic(cout);
#endif
		}
	
	
		/* build the coefficient for denominator */
		cout <<"\nRecomputing the coefficient for denominator"<<endl;

		for(cof_aux = poly_list->den_list; cof_aux; cof_aux = cof_aux->next)
		{
			//time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    
			if(cof_aux->fcoeff_list)
			{
				time1 = 1.0 * clock() / CLOCKS_PER_SEC;
				cof_aux->fcoeff_list->calc_fcoeff_list(prec_ht, ht);
				time2 = 1.0 * clock() / CLOCKS_PER_SEC;
				e_time = e_time + time2 - time1;

#ifdef COLLECT_COEFF_DATA
				cout <<"\nStatisitic for denominator coefficient list"<<endl;
				cof_aux->fcoeff_list->semi_symbolic_statistic(cout, node_ht, term_ht);
				//cof_aux->fcoeff_list->statistic(cout);
#endif
			}
		}
	}

    delete node_ht;
	delete term_ht;
	
	delete prec_ht;
	delete ht;
	cout <<"total fcoeff DDD reordering time: " << c_time <<endl;
	cout <<"total coeff evaluation time: "<<e_time<<endl;
	print_mesg("End of semi symbolic analysis ");

	theFLabMap->printFlabel2BchMap(cout);
	
	bool usesimp = false;
	fcoeff_freq_sweep(usesimp);

/* caluculate all numerical value of semi symbolic coefficient */	
//calc_semi_fcoeff_list_value();

}


/*
 * SymbNodesInFunction, SymbPathsInFunction (i.e. Count)...  Compute
 * the number of symbolic DDD vertices and number of paths in the
 * symbolic DDD graph. Those function is only applied to fcoeff DDDs.
 */

integer
DDDmanager::FSymbNodesInFunction
(
	DDDnode* f
)
{

	integer n;
	HasherTree *ht    = new HasherTree(FALSE);
	FSymbNodesInFunctionR(f, ht);
	n = ht->Count();
	delete ht;
	return n;
}

void
DDDmanager::FSymbNodesInFunctionR
(
	DDDnode *f, 
	HasherTree *ht
) 
{
  
	if (f == one || f == zero)
	{		
		return;
	}
	
	DummBranch *dbch =  theFLabMap->get_dummbranch(f->index);
	assert(dbch);
	if(dbch->is_symbolic())
	{
				
		if (ht->Put(*f,f)) 
		{
			if (f->Then())
			{			
				FSymbNodesInFunctionR(f->Then(), ht);
			}
		
			if (f->Else())
			{			
				FSymbNodesInFunctionR(f->Else(), ht);
			}
		
		} 
		else
		{		
			return;
		}
	}	
	
}

integer
DDDmanager::FSymbTerminalsInFunction
(
	DDDnode* f
)
{

	integer n;
	HasherTree *ht    = new HasherTree(FALSE);
	FSymbTerminalsInFunctionR(f, ht);
	n = ht->Count();
	delete ht;
	return n;
}

void
DDDmanager::FSymbTerminalsInFunctionR
(
	DDDnode *f, 
	HasherTree *ht
) 
{
  
	if (f == one || f == zero)
	{		
		return;
	}
	
	DummBranch *dbch =  theFLabMap->get_dummbranch(f->index);
	assert(dbch);
	if(dbch->is_symbolic() == false)
	{
				
		ht->Put(*f,f); 
	}	
	else
	{		
		if (f->Then())
		{			
			FSymbTerminalsInFunctionR(f->Then(), ht);
		}
		
		if (f->Else())
		{			
			FSymbTerminalsInFunctionR(f->Else(), ht);
		}
	}		
}


float
DDDmanager::FSymbPathsInFunction
(
	DDDnode* P 
) 
{
	HasherTree *ht    = new HasherTree(TRUE);
	float       count = FSymbPathsInFunctionR( P, ht );

	delete ht;
	return count;
}

float
DDDmanager::FSymbPathsInFunctionR
(
	DDDnode* P, 
	HasherTree* ht
) 
{
	Object *o;

	if (P == zero )
	{		
		return 0;
	}
	
	if (P == one )
	{		
		return 1;
	}
	
	DummBranch *dbch =  theFLabMap->get_dummbranch(P->index);
	assert(dbch);
	if(dbch->is_symbolic())
	{
		
		if (ht->Get(&o, P))
		{		
			return ((FloatObj*)o)->val;
		}
	

		float n;
		n = FSymbPathsInFunctionR(P->Then(), ht) +
			FSymbPathsInFunctionR(P->Else(), ht);
		
		ht->Put(new FloatObj(n), P);
		return n;
	}
	else // it is a numericla terminal
	{
		return 1;
	}	
	
}

/*
*******************************************************


   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: label.cc,v $
 *    $Revision: 1.4 $
 *    $Date: 2002/06/08 14:24:35 $
 */


#define _DEBUG


// two row or column updating direction
#define INCREASE  1
#define DECREASE -1

/* INCLUDES: */
#include <math.h>
#include <limits.h>
#include <unistd.h>

#include "globals.h"
#include "circuits.h"
#include "dtstruct.h"
#include "mna_form.h"
#include "ddd_base.h"
#include "ckt_call.h"
#include "acan.h"
#include "mxexp.h"

/*----------------------------------------------------
    FuncName: create_index
    Spec: create the index matrix of MNA matrix
 -----------------------------------------------------
    Inputs: MNA matrix
    Outputs: the index matrix and index to element 
    mapping. The procedure call the element-
    assigning heuristics to label a unique index 
    for each element

    Global Variblas used:
----------------------------------------------------*/
void
MNAMatrix::create_index()
{
    MNAElement    *    elem_aux;
    MNARow        *    row_aux;
    int                index = 0;

    if(!rowlist)
	{
        print_error(INT_ERROR,"Invalid rowlist");
        return;
    }

    // set the initial label
    //init_label = IndexMatrix::cur_label - 1;
    init_label = 0;

    // initialize the index matrix
    index_mat = new IndexMatrix(size, ddd_mgr);

    // first assign a new index to every row in MNA 
    if(ckt->which_level() == cktTop)
	{ // in main circuit
        for(row_aux = rowlist; row_aux; row_aux = row_aux->Next())
		{
            row_aux->get_pivot()->get_index() = ++index;
        }
    } 
	else if(ckt->which_level() == cktSub)
	{ // in sub circuit
        for(row_aux = rowlist; row_aux; row_aux = row_aux->Next())
		{
            row_aux->get_pivot()->get_index() = ++index;
            if(row_aux->get_pivot()->get_type() == nBound)
			{
                index_mat->row_m->delete_rc( index );
                index_mat->col_m->delete_rc( index );
            }
        }
    }
    
    /* create the boolean index matrix which does not includes
       boundary elements of sub_circuits */

    for(row_aux = rowlist; row_aux; row_aux = row_aux->Next())
	{

        if(ckt->which_level() == cktSub && 
           row_aux->get_pivot()->get_type() == nBound )
		{
			continue;
		}
        // check all elements
        for(elem_aux = row_aux->elem_list; elem_aux; 
			elem_aux = elem_aux->Next())
		{
            if(ckt->which_level() == cktSub &&
               elem_aux->node->get_type() == nBound )
			{
                continue;
			}
            else
			{
                num_elem++;
                index_mat->add_element(
                    row_aux->pivot->get_index(),
                    elem_aux->node->get_index());
            }
        }
    }
    cout <<"number of element is: "<<num_elem<<endl;
    index_mat->print_bool();

    // assign a unique label for no boundary each elements in MNA
    index_mat->initial_rowcol();
    IndexMatrix::reset_label();
	
	// for semi-symbolic analysis.
	index_mat->assign_symbolic_elements(this);
	
    index_mat->min_label_assignR();
    //index_mat->random_assign();
    index_mat->force_label_assign();

    index_mat->print_index();

    // create the indices to elements mapping
    create_mapping();
}    

/*----------------------------------------------------
    FuncName:
    Spec: This is a heuristic routine used to assign
          indices to the nonzero elements in the MNA 
          matrix in order to reduce the size of resulting 
          DDD.
          The algorithm used here is a greedy scheme which
          always assigns the elements in the denser
          rows or columns with smaller indices first.
          (see our paper ICCAD 1997)
 -----------------------------------------------------
    Inputs: Bool and other data in the class
    Outputs: Index[i][j]

    Global Variblas used: none
----------------------------------------------------*/

void
IndexMatrix::min_label_assignR()
{

    int min_elem_row = -1;
	int min_elem_column = -1;
    int min_row_value;
    int min_col_value;
    int row,col;
    short *row_visited = new short[size+1];
    short *col_visited = new short[size+1];
    int min_value;
    iStack *theiStack = new iStack;

//#define MARKOWITZ
#ifdef MARKOWITZ 
	FindMinMarkowitzElement(min_elem_row, min_elem_column);
	cout <<"min_row: " << min_elem_row << endl;
	cout <<"min_col: " << min_elem_column << endl;
#else
    min_elem_row = row_m->get_min();    
    min_elem_column = col_m->get_min();    
#endif
    // if all the rows and columns have been expanded
    if( (min_elem_row == -1) || (min_elem_column == -1 ))
	{
        return; 
	}

    min_row_value = row_m->get_count(min_elem_row);
    min_col_value = col_m->get_count(min_elem_column);

    //expand the matrix according to the min column if ...
    if( (min_row_value >= min_col_value) && (min_col_value > 0) )
	{

        //first we expand the columns
        col_m->expand_rc(min_elem_column);

        // update the affected rows
        row_m->update(min_elem_column, DECREASE);
        
        memset(row_visited,0,(size+1)*sizeof(short));
        theiStack->Free();
        
        /* first get the updated min_col_value value */
        //min_col_value = col_m->get_count(min_elem_column);

        // then find a row among the left with minimum number of rows
        for(int irow = 0 ; irow < min_col_value; irow++ )
		{
            min_value = SHRT_MAX;
            row = -1;
            for(int ir = 1; ir <= size; ir++)
			{
                if((row_m->get_state(ir) == NORMAL 
					&& Bool[ir][min_elem_column])
				   && !row_visited[ir]) 
				{ // have not been assigned yet
                    if(row_m->get_count(ir) < min_value)
					{    
                        row = ir;
                        min_value = row_m->get_count(ir);
                    }
                }
            }
#ifdef _DEBUG
            if(row == -1)
			{
                print_error(FAT_ERROR,"row == -1, something wrong here");
                exit(-1);
            }
#endif
            theiStack->Push(row);
            row_visited[row] = 1;
            
            // expand the row found
            row_m->expand_rc(row);

            // update affected columns
            col_m->update(row,  DECREASE);

            if(check_unassigned_elem())
                min_label_assignR();
                
            // labelling those not labelled in previous step
            force_label_assign();

            // restore the affacted columns
            col_m->update(row,  INCREASE);

            // then unexpand the expanded row 
            row_m->restore_rc(row);
        }

        // restore the affacted rows
        row_m->update(min_elem_column,  INCREASE);

        // unexpand the expanded column
        col_m->restore_rc(min_elem_column);

            
        /* now assign indices to the unlabled elements 
           in the columns in reverse order of their expansion
        */
        while((row = theiStack->Pop()) != -1)
		{
#ifdef _DEBUG
            if((row_m->get_state(row) != NORMAL) ||
			   !Bool[row][min_elem_column])
                print_error(INT_ERROR,"unexpected row pushed");
#endif
            if(!Index[row][min_elem_column])
			{
                Index[row][min_elem_column] = get_min_index();
            }
        }
    }

    else if( min_row_value > 0) 
	{ 
		// expand the matrix according to the min row

        //first we expand the row
        row_m->expand_rc(min_elem_row);

        // update the affected columns
        col_m->update(min_elem_row, DECREASE);
        
        memset(col_visited,0,(size+1)*sizeof(short));
        theiStack->Free();
        
        // first get the updated min_row_value value
        //min_row_value = row_m->get_count(min_elem_row);

        // then find a col among the left with minimum number of columns
        for(int icol = 0 ; icol < min_row_value; icol++ )
		{
            min_value = SHRT_MAX;
            col = -1;
            for(int ic = 1; ic <= size; ic++)
			{
                if((col_m->get_state(ic) == NORMAL 
					&& Bool[min_elem_row][ic])
				   && !col_visited[ic]) { // have not been assigned yet
                    if(col_m->get_count(ic) < min_value)
					{    
                        col = ic;
                        min_value = col_m->get_count(ic);
                    }
                }
            }
#ifdef _DEBUG
            if(col == -1)
			{
                print_error(FAT_ERROR,"col == -1, something wrong here");
                exit(-1);
            }
#endif
            col_visited[col] = 1;
            theiStack->Push(col);

            // expand the column found
            col_m->expand_rc(col);

            // update the affected rows
            row_m->update(col,  DECREASE);

            if(check_unassigned_elem())
                min_label_assignR();
                
            // labelling those not labeled in previous step
            force_label_assign();

            // restore the affacted rows
            row_m->update(col,  INCREASE);

            // then unexpand the expanded col 
            col_m->restore_rc(col);
        }

        // restore the affacted cols
        col_m->update(min_elem_row,  INCREASE);

        // unexpand the expanded column
        row_m->restore_rc(min_elem_row);

            
        /* now assign indices to the unlabled elements 
           in the columns in reverse order of their expansion
        */
        while((col = theiStack->Pop()) != -1)
		{
#ifdef _DEBUG
            if((col_m->get_state(col) != NORMAL) ||
			   !Bool[min_elem_row][col])
			{
                print_error(INT_ERROR,"unexpected col pushed");
			}
#endif
            if(!Index[min_elem_row][col])
			{
                Index[min_elem_row][col] = get_min_index();
            }
        }
    }

    delete [] row_visited;
    delete [] col_visited;
    delete theiStack;
}


/* Find the row and column element with minimum Markowitz number */
void
IndexMatrix::FindMinMarkowitzElement
(
	int & row,
	int & col
)
{
	// first initialize return row anc column
	row = -1;
	col = -1;

	int int_value = SHRT_MAX;

	for(int icol = 1 ; icol <= size; icol++ )
	{
		if(col_m->get_state(icol) != NORMAL)
		{
			continue;
		}

		for(int irow = 1; irow <= size; irow++)
		{
			if((row_m->get_state(irow) == NORMAL 
                && Bool[irow][icol]) &&
			   !Index[irow][icol]) 
			{ // have not been assigned yet
				int val = (col_m->get_count(icol)-1)*
					(row_m->get_count(irow)-1);
				if(val < int_value)
				{
					int_value = val;
					row = irow;
					col = icol;
				}				
			}
		}
	}
}

/*----------------------------------------------------
    FuncName:random_assign
    Spec: instead of min-DDD size assignment, we 
    sometimes need randomly assign the elements.
    So, we have this procedure.
 -----------------------------------------------------
    Inputs:
    Outputs: same as the min_label_assignR

    Global Variblas used:
----------------------------------------------------*/
void
IndexMatrix::random_assign()
{
    char *Mark = new char[num_elem+1];
    memset(Mark,0,(num_elem + 1)*sizeof(char));
    int index=1;
    int row, col;
	void InitRandom();

    // put the seed in the random generator
    InitRandom();

    for( row = 1; row <= size; row++)
	{
        for( col = 1; col <= size; col++)
		{
            if(Bool[row][col])
			{
                while(1)
				{
                    index = (rand() % (num_elem + 1));
                    if(index == 0)
                        continue;
                    if(Mark[index])
                        continue;
                    
                    Index[row][col] = index;
                    Mark[index] = 1;
                    break;
                }
                /*
				  Index[row][col] = index;
				  Index2mna[index].index = index;
				  Index2mna[index].row = row;
				  Index2mna[index].col = col;
				  index++;
				  printf("Bool[%d][%d]=%d\n",row,col,index);
                */
            }
        }
    }
}

/*----------------------------------------------------
    FuncName:force_label_assign
    Spec: 
    This is because if entries will not appear 
    in the final expression, it will not be assigned an index. e.g.
    | a b |
    | 0 c |
    In this case, b will not be assigned.
    So, we need explicitly assign an index to it after
    the normal expansion-and-indexing procedure.
 -----------------------------------------------------
    Inputs:
    Outputs: same as the min_label_assignR

    Global Variblas used:
-----------------------------------------------------*/
void
IndexMatrix::force_label_assign()
{
	int i,j,index;
 
    for(i = 1; i <= size; i++ )
	{
        if(row_m->get_state(i) == NORMAL)
		{
            for( j = 1; j <= size; j++)
			{
                if(col_m->get_state(j) == NORMAL)
				{
                    if(Bool[i][j] && !Index[i][j])
					{
                        index = Index[i][j] = get_min_index();
                        printf("[%d,%d] has been forcely assigned \n",i,j);
                    }
                }
            }
        }
    }
}

/*----------------------------------------------------
Spec: Check whether the unexpanded matrix has the unlabeled elements
-----------------------------------------------------*/
int
IndexMatrix::check_unassigned_elem()
{
    int i, j;

    for(i = 1; i <= size; i++ )
	{
        if(row_m->get_state(i) == NORMAL)
		{
            for(j = 1; j <= size; j++)
			{
                if(col_m->get_state(j) == NORMAL)
				{
                    if(Bool[i][j] && !Index[i][j])
					{ 
                        return 1;
                    }
                }
            }
        }
    }            
    return 0;
}



/*
 * Assign all the symbolic elements (in semi-symbolic analysis) with
 * largest labels first.
 */ 
void
IndexMatrix::assign_symbolic_elements
(
	MNAMatrix *mna_matrix
)
{
	assert(mna_matrix);

	int index = num_elem;
	
	for(int i = 1; i <= size; i++)
	{
		for(int j = 1; j <= size; j++)
		{
			if(Bool[i][j])
			{
				MNAElement *elem = mna_matrix->get_mna_element(i,j);
				assert(elem);
				//cout <<"processing elem (" << i << " " << j <<")" << endl;
				
				if(elem->is_symbolic())
				{								
				
					Index[i][j] = index;
					index--;
					cout <<"assign elem (" << i << " " << j <<")" <<" to " << Index[i][j] << endl;
				}				
			}
			
		}				
	}	
	
}

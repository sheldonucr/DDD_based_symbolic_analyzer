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
 *    $RCSfile: mxexp.cc,v $
 *    $Revision: 1.4 $
 *    $Date: 2002/05/20 04:22:01 $
 */


#define _DEBUG


// two row or column updating direction
#define INCREASE  1
#define DECREASE -1

/* INCLUDES: */
#include "globals.h"
#include <math.h>
#include <limits.h>

#include "circuits.h"
#include "mxexp.h"
#include "dtstruct.h"

long Max_ddd_size = 4000000;
long Delta_ddd = 10000;

int IndexMatrix::cur_label;

void InitRandom();
/* 
**    Member function defintion of RowColManager 
*/
/*----------------------------------------------------
Spec: we need the size info and the really allocated
      size is one more element larger than needed for
      indexing convenience
-----------------------------------------------------*/
RowColManager::RowColManager( int _size, int **_bool )
{
    size = _size;
    list = new ROWCOL[size+1];
    memset(list, 0, (size+1)*sizeof(ROWCOL));
    ibool = _bool;
}
    
RowColManager::~RowColManager()
{
    delete [] list;
}

/*----------------------------------------------------
Spec: reset both count value and state 
-----------------------------------------------------*/
void 
RowColManager::reset()
{
    memset(list, 0, (size+1)*sizeof(ROWCOL));
}

/*----------------------------------------------------
Spec: reset all the count and set all rows/cols state 
    to NORMAL except the DELETED one.
-----------------------------------------------------*/
void
RowColManager::initial()
{
    int i;
    for(i = 1; i <= size; i++){
        if(list[i].state != DELETED)
            list[i].state = NORMAL;
        list[i].count = 0;
    }
}

/*----------------------------------------------------
Spec: get or set the count value of specific row or col
-----------------------------------------------------*/
int &
RowColManager::get_count( int num)
{
#ifdef _DEBUG
    //ASSERT(num <= size,"Invalid row or col index");
    //ASSERT(num >= 1,"Invalid row or col index");
    if(num > size || num < 1 ){
        sprintf(_buf,"Invalid row or col index: %d", num);
        error_mesg(INT_ERROR,_buf);
    }
#endif
    return list[num].count;
}

/*----------------------------------------------------
Spec: return the state of specific row or col 
-----------------------------------------------------*/
RowColState 
RowColManager::get_state(int num)
{
#ifdef _DEBUG
    ASSERT(num <= size,"Invalid row or col index");
    ASSERT(num >= 1,"Invalid row or col index");
#endif
    return list[num].state;
}

/*----------------------------------------------------
Spec: set the state of specific row or col 
-----------------------------------------------------*/
void
RowColManager::set_state(RowColState _state, int num )
{
#ifdef _DEBUG
    ASSERT(num <= size,"Invalid row or col index");
    ASSERT(num >= 1,"Invalid row or col index");
#endif
    list[num].state = _state;
}
    

/*----------------------------------------------------
    FuncName: get_min()
    Spec: find the row with minimum nonzero element
 -----------------------------------------------------
    Inputs: the row_info
    Outputs: the value of minimum nonzero elements in 
            the found row. if no row is in NORMAL state,
            -1 will return;

    Global Variblas used: none
----------------------------------------------------*/
int
RowColManager::get_min()
{
    int min = -1, min_value = INT_MAX;

    for( int i = 1; i <= size; i++ ){
        if(  list[i].state == NORMAL ){
            if( list[i].count < min_value ){
                min_value = list[i].count;
                min = i;
            }
        }
    }
    if(min_value == INT_MAX)
        return -1;
    return min;
}

void
RowColManager::print_state()
{
    cout <<"State: " << endl;
    for( int i = 1; i<=size; i++){
        if(list[i].state == NORMAL)
            cout <<"N("<<i<<", "<<list[i].count<<") ";
        else if(list[i].state == DELETED)
            cout <<"D("<<i<<", "<<list[i].count<<") ";
        else if(list[i].state == EXPANDED)
            cout <<"E("<<i<<", "<<list[i].count<<") ";
        else
            cout <<"U("<<i<<", "<<list[i].count<<") ";
    }
    cout <<endl;
}
        
///////////////////////////////////////////////
//    member function defintion for RowManager
///////////////////////////////////////////////

RowManager::RowManager(int _size , int **_bool):RowColManager(_size,_bool)
{ ;}

/*----------------------------------------------------
Spec: update all the row counts wrt specific col 
-----------------------------------------------------*/
void
RowManager::update( int col, int direction )
{
    int i;
    for(i = 1; i <= size; i++)
	{
        if(list[i].state == NORMAL && ibool[i][col])
		{
            if(direction > 0)
                list[i].count++;
            else
                list[i].count--;
		}
    }
}

/*----------------------------------------------------
Spec: print the row states wrt specific col
-----------------------------------------------------*/
void
RowManager::print_state( int col )
{
    cout <<"Row state wrt col: "<< col << endl;
    for( int i = 1; i<=size; i++){
        if(list[i].state == NORMAL && ibool[i][col])
            cout <<"N("<<i<<") ";
        else if(list[i].state == DELETED && ibool[i][col])
            cout <<"D("<<i<<") ";
        else if(list[i].state == EXPANDED && ibool[i][col])
            cout <<"E("<<i<<") ";
        else if(!ibool[i][col])
            cout <<"0("<<i<<") ";
        else
            cout <<"U("<<i<<") ";
    }
}


///////////////////////////////////////////////
//  member function defintion for RowManager
///////////////////////////////////////////////

ColManager::ColManager(int _size , int **_bool):RowColManager(_size,_bool)
{ ;}

/*----------------------------------------------------
Spec: update all the col counts wrt specific row 
-----------------------------------------------------*/
void
ColManager::update( int row, int direction )
{
    int i;
    for(i = 1; i <= size; i++)
	{
        if(list[i].state == NORMAL && ibool[row][i])
		{
            if(direction > 0)
                list[i].count++;
            else
                list[i].count--;
		}
    }
}

/*----------------------------------------------------
Spec: print the column states wrt specific row
-----------------------------------------------------*/
void
ColManager::print_stat(int row)
{
    cout <<"Col state wrt row: "<< row << endl;
    for( int i = 1; i<=size; i++){
        if(list[i].state == NORMAL && ibool[row][i])
            cout <<"N("<<i<<") ";
        else if(list[i].state == DELETED && ibool[row][i])
            cout <<"D("<<i<<") ";
        else if(list[i].state == EXPANDED && ibool[row][i])
            cout <<"E("<<i<<") ";
        else if(!ibool[row][i])
            cout <<"0("<<i<<") ";
        else
            cout <<"U("<<i<<") ";
    }
}

///////////////////////////////////////////////
//    member function defintion for IndexMatrix
///////////////////////////////////////////////

/*----------------------------------------------------
Spec: The really allocated size also one more 
      bigger than needed for indexing convenience
-----------------------------------------------------*/
IndexMatrix::IndexMatrix( int _size , DDDmanager *_ddd )
{
    size = _size;
    num_elem = 0;
    Index     = new int*[_size+1];
    Bool     = new int*[_size+1];
    ddd     = _ddd;

    // allocate space for Index and Bool 
    for(int i = 0; i <= _size; i++){
        Index[i] = new int[_size+1];
        Bool[i] = new int[_size+1];
        memset(Index[i], 0, (_size+1)*sizeof(int));
        memset(Bool[i], 0, (_size+1)*sizeof(int));
    }

    // allocate space for row_info and col_info
    row_m = new RowManager(_size, Bool );
    col_m = new ColManager(_size, Bool );
}
    
/*----------------------------------------------------
Spec:
-----------------------------------------------------*/
IndexMatrix::~IndexMatrix()
{
    int i;

    if(Index){
        for(i = 0; i <= size; i++)
            delete [] Index[i];
    }
    delete [] Index;

    if(Bool){
        for(i = 0; i <= size; i++)
            delete [] Bool[i];
    }
    delete [] Bool;

    if(row_m)
        delete row_m;
    
    if(col_m)
        delete col_m;

}


/*----------------------------------------------------
Spec: add a boolean 1 into specified postion 
      determined by row and column
-----------------------------------------------------*/
void
IndexMatrix::add_element( int row, int col)
{
#ifdef _DEBUG
    ASSERT(row <= size,"Invalid row index");
    ASSERT(col <= size,"Invalid row index");
#endif
    Bool[row][col] = 1;
    num_elem++;
}

/*----------------------------------------------------
Spec: add a label to a specific position determined
    by row and column.
-----------------------------------------------------*/
void
IndexMatrix::add_label( int row, int col, int label)
{
#ifdef _DEBUG
    ASSERT(row <= size,"Invalid row index");
    ASSERT(col <= size,"Invalid row index");
#endif
    Index[row][col] = label;
}

/*----------------------------------------------------
Spec: delete a element from specified postion 
      determined by row and column
-----------------------------------------------------*/
void
IndexMatrix::delete_element( int row, int col)
{
#ifdef _DEBUG
    ASSERT(row <= size,"Invalid row index");
    ASSERT(col <= size,"Invalid row index");
#endif
    Bool[row][col] = 0;
}

/*----------------------------------------------------
Spec: count the initial nonzero elements for each row and column 
-----------------------------------------------------*/
void 
IndexMatrix::initial_rowcol()
{
    int i,j;
    // first reset counts of row and col
    row_m->initial();
    col_m->initial();

    // recalculate the num for all the row and column 
    for(i = 1; i <= size; i++){
        for(j = 1; j <= size; j++){
            if(Bool[i][j]){
                if(row_m->get_state(i) == NORMAL)
                    row_m->get_count(i)++;
                if(col_m->get_state(j) == NORMAL)
                    col_m->get_count(j)++;
            }
        }
    }
}


/*----------------------------------------------------
Spec: set the count and state for both of row and column
-----------------------------------------------------*/
void 
IndexMatrix::reset_rowcol()
{
    row_m->reset();
    col_m->reset();
}

/*----------------------------------------------------
Spec: return the minimum available index
-----------------------------------------------------*/
int IndexMatrix::get_min_index()
{
    return cur_label++;
}

/*----------------------------------------------------
Spec: output the indices information in matrix form
-----------------------------------------------------*/
void
IndexMatrix::print_index()
{
    int i,j;

    printf("\n\nTHE INDEX MATRIX \n");
    for(i = 1; i <= size ; i++){
        printf("(%d) [",i);
        for( j = 1; j <= size; j++)
            printf("%3d ",Index[i][j]);
        printf("]\n");
    }
    printf("\n\n");
}

/*----------------------------------------------------
Spec: output the boolean representation of the MNA matrix 
-----------------------------------------------------*/
void
IndexMatrix::print_bool()
{
    int i,j;

    printf("\n\nTHE MNA BOOLEAN MATRIX \n");
    for(i = 1; i <= size ; i++){
        printf("(%d) [",i);
        for( j = 1; j <= size ; j++)
            printf("%3d ",Bool[i][j]);
        printf("]\n");
    }
    printf("\n\n");
}

void
IndexMatrix::dump_index
(
	ofstream & out
)
{
	int i,j;
	//first the size of the matrix:	
	out << size << endl;

	// then the element in the matrix
	out << num_elem << endl;

	// the format is : row column label	
    for(i = 1; i <= size ; i++){
		
		for( j = 1; j <= size ; j++)
		{
			if(Index[i][j])
			{
				out << i << " " << j << " " << Index[i][j] << endl;
			}
		}
    }

    out << endl;
}

/*----------------------------------------------------
    FuncName: matrix_recursive_exp
    Spec: 

        Recursively expand the matrix and store all
        the product terms using DDD. The procedure
        also takes advantage of garbage collection
        techniques and is capable of dynamically
        releasing all the DDD no long used except for
        caching propose in order take great use of
        the memory.

        The procudure actually is the same procedure 
        for MNA element labelling. I believe it is 
        the efficient way of expansion in terms of 
        probability of recurrance of intermediate 
        minors(i.e minor caching effect).
 -----------------------------------------------------
    Inputs: the deleted row and column 
        if de_row == -1 and de_col == -1, no row
        and column will be deleted.

    Outputs: DDD representation of the remianed matrix

    Global Variblas used: cur_ddd(static)
----------------------------------------------------*/

//static ZBDDnode * cur_ddd;

DDDnode *
IndexMatrix::matrix_recursive_expR( int de_row, int de_col)
{
    int min_elem_row; 
    int min_elem_column;
    int min_row_value;
    int min_col_value;
    int row,col;
    short *row_visited = new short[size+1];
    short *col_visited = new short[size+1];
    int min_value;

    if(de_row != -1 && de_col != -1){
#ifdef _DEBUG
        if(row_m->get_state(de_row) == DELETED){
            error_mesg(INT_ERROR,"cann't expand a deleted row.");
	    delete [] row_visited;
	    delete [] col_visited;
            return NULL;
        }
        if(col_m->get_state(de_col) == DELETED){
            error_mesg(INT_ERROR,"cann't expand a deleted column.");
	    delete [] row_visited;
	    delete [] col_visited;
            return NULL;
        }
#endif
        row_m->expand_rc(de_row);
        col_m->update(de_row, DECREASE);
        col_m->expand_rc(de_col);
        row_m->update(de_col, DECREASE);
    }

    /* DDD */
    DDDnode *ddd_aux = ddd->Base();
    DDDnode *cur_level_ddd = ddd->Empty();
    
    min_elem_row = row_m->get_min();    
    min_elem_column = col_m->get_min();    

    // if all the rows and columns has been expanded
    if( (min_elem_row == -1) || (min_elem_column == -1 )){
	delete [] row_visited;
	delete [] col_visited;
        return ddd->Empty();
    }

    min_row_value = row_m->get_count(min_elem_row);
    min_col_value = col_m->get_count(min_elem_column);

    //expand the matrix according to the min column if ...
    if(( min_row_value  >= min_col_value) && (min_col_value > 0)){

        //first we delete the columns
        col_m->expand_rc(min_elem_column);
        // update the affected rows
        row_m->update(min_elem_column, DECREASE);
        
        memset(row_visited,0,(size+1)*sizeof(short));
        
        // find a row among the left with minimum number of rows
        for(int irow = 0 ; irow < min_col_value; irow++ ){
            row = -1;
            min_value = SHRT_MAX;
            for(int ir = 1; ir <= size; ir++){
                if((row_m->get_state(ir) == NORMAL 
                && Bool[ir][min_elem_column])
                && !row_visited[ir]) { // have not been assigned yet
                    if(row_m->get_count(ir) < min_value){    
                        row = ir;
                        min_value = row_m->get_count(ir);
                    }
                }
            }
#ifdef _DEBUG
            if(row == -1){
                row_m->print_state(min_elem_column);
                print_error(FAT_ERROR,"row == -1, something wrong here");
                //exit(-1);
            }
#endif
            row_visited[row] = 1;
            
            // expand the row found
            row_m->expand_rc(row);

            // update the affacted columns
            col_m->update(row, DECREASE);

            if((ddd_aux = query_minor()))
                add_minor(&cur_level_ddd,ddd_aux,Index[row][min_elem_column]);
            else{
                // for garbage collection
                ddd->Keep(cur_level_ddd); 

                ddd_aux = matrix_recursive_expR();

                ddd->UnKeep(cur_level_ddd);

                add_minor(&cur_level_ddd, ddd_aux,Index[row][min_elem_column]);

            }

            // restore the affacted columns
            col_m->update(row,  INCREASE);

            // then unexpand the expanded row 
            row_m->restore_rc(row);
        }

        // restore the affacted rows
        row_m->update(min_elem_column,  INCREASE);

        // unexpand the expanded column
        col_m->restore_rc(min_elem_column);
            
    }
    // expand the matrix according to the min row
    else if(min_row_value > 0 && min_col_value >0) { 
                
        //first we delete the row
        row_m->expand_rc(min_elem_row);

        // update the affected columns
        col_m->update(min_elem_row, DECREASE);
        
        memset(col_visited,0,(size+1)*sizeof(short));

        // find a col among the left with minimum number of columns
        for(int icol = 0 ; icol < min_row_value; icol++ ){
            min_value = SHRT_MAX;
            col = -1;
            for(int ic = 1; ic <= size; ic++){
                if((col_m->get_state(ic) == NORMAL 
                && Bool[min_elem_row][ic])
                && !col_visited[ic]) { // have not been assigned yet
                    if(col_m->get_count(ic) < min_value){    
                        col = ic;
                        min_value = col_m->get_count(ic);
                    }
                }
            }
#ifdef _DEBUG
            if(col == -1){
                col_m->print_state(min_elem_row);
                print_error(FAT_ERROR,"col == -1, something wrong here");
            }
#endif
            col_visited[col] = 1;

            // delete the column found
            col_m->expand_rc(col);

            // update the affected rows
            row_m->update(col,  DECREASE);

            if((ddd_aux = query_minor()))
                add_minor(&cur_level_ddd,ddd_aux, Index[min_elem_row][col]);
            else{
                // for garbage collection
                ddd->Keep(cur_level_ddd); 

                ddd_aux = matrix_recursive_expR();

                ddd->UnKeep(cur_level_ddd);

                add_minor(&cur_level_ddd, ddd_aux, Index[min_elem_row][col]);

            }
            // restore the affacted rows
            row_m->update(col,  INCREASE);

            // then unexpand the expanded col 
            col_m->restore_rc(col);
        }

        // restore the affacted cols
        col_m->update(min_elem_row, INCREASE);

        // unexpand the expanded column
        row_m->restore_rc(min_elem_row);
            
    } 
    else{
        // it is singular submatrix, ingore it
        delete [] row_visited; 
        delete [] col_visited; 
        if(de_row != -1 && de_col != -1){
            row_m->restore_rc(de_row);
            col_m->restore_rc(de_row);
        }
        return (DDDnode *)NULL;
    }
    delete [] row_visited; 
    delete [] col_visited; 
    if(de_row != -1 && de_col != -1){
        row_m->restore_rc(de_row);
        col_m->restore_rc(de_row);
    }
    if(cur_level_ddd == ddd->Empty()){
        return (DDDnode *)NULL;
    }
    else
        return cur_level_ddd;
}


/*
** add a DDD monir into the whole DDD hash tree and update
** the cur_ddd which points the DDD reresenting all the minors
** in current expansion level(matrix).
*/
void
IndexMatrix::add_minor(DDDnode **cur_ddd,DDDnode *new_ddd, int index)
{
    DDDnode *ddd_now;
    if(!new_ddd) // in case of singular submatrix
        return;
    // if it is a nontrival minor, then
    if(new_ddd != ddd->Empty())
       put_minor(new_ddd);
    ddd_now = ddd->Base();
    ddd_now = ddd->Change(ddd_now, index);
    if(new_ddd != ddd->Empty())
        ddd_now = ddd->Product(ddd_now,new_ddd);
    *cur_ddd = ddd->Union(*cur_ddd, ddd_now);

    // do garbage collection if necessary
    if(ddd->NumNodes() > Max_ddd_size){
        print_raw("\nImplicitly force GC ...");
        ddd->Keep(*cur_ddd);
        ddd->GC();
        ddd->UnKeep(*cur_ddd);
        Max_ddd_size += Delta_ddd;
        print_mesg("done.");
    }
}

/* 
** routines to implement the hash table for storing the DDD minor 
*/ 

// Use the DDD internal hashcache to store the minor DDD node
// we transfer the row and column patterm into two DDDs and
// use them to do the hashing.

DDDnode * 
IndexMatrix::query_minor()
{
    DDDnode *ddd_row = ddd->Base();
    DDDnode *ddd_col = ddd->Base();
    DDDnode *minot_r;

    for( int i = 1; i <= size; i++){
        if(row_m->get_state(i) == NORMAL)
            ddd_row = ddd->Change(ddd_row,i);

        if(col_m->get_state(i) == NORMAL)
            ddd_col = ddd->Change(ddd_col,i);
    }

    minot_r = ddd->GetMinor(ddd_row,ddd_col);
    return minot_r;
}

void
IndexMatrix::put_minor(DDDnode* minor)
{
    DDDnode *ddd_row = ddd->Base();
    DDDnode *ddd_col = ddd->Base();

    for( int i = 1; i <= size; i++){
        if(row_m->get_state(i) == NORMAL)
            ddd_row = ddd->Change(ddd_row,i);

        if(col_m->get_state(i) == NORMAL)
            ddd_col = ddd->Change(ddd_col,i);
    }
    ddd->PutMinor(minor,ddd_row,ddd_col);
}

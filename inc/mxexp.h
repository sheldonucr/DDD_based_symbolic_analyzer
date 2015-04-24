#ifndef MATRIX_EXP_H
#define MATRIX_EXP_H

/*
 *    $RCSfile: mxexp.h,v $
 *    $Revision: 1.5 $
 *    $Date: 2002/09/30 20:55:21 $
 */

 /*
 ////////////////////////////////////////////////////
 //
 // exp_determ.h : determinant calcualtion
 // of MNA head file
 //
 //  X. Sheldon Tan
 //  (c) Copyright the University of Iowa, 1997
 //
 //
 ///////////////////////////////////////////////////
 */

#include "unixstd.h"
#include "ddd_base.h"

extern long Max_ddd_size;    
extern long Ddd_delta;

class MNAElement;

typedef enum 
{
	NORMAL = 0,// normal state
	EXPANDED, // deleted due to matrix expansion
	DELETED   // has been deleted in the matrix expansion and labeling
} RowColState;
    
typedef struct _row_col 
{
	int    count; // number of nonzero element
	RowColState state; // NORMAL, DELETED, EXPANDED 
} ROWCOL;

class RowColManager 
{

protected:
	int    size;
	ROWCOL    *list;
	int    **ibool;

public:

	RowColManager(int _size, int **_ibool);
	virtual ~RowColManager();

	void     reset();   /* set all the row and col state to NORMAL 
						   and reset all the count num to 0 */
	void     initial(); 
	int &     get_count( int num );
	RowColState get_state( int num );
	void     set_state( RowColState _state, int num );
	int      get_min();

	virtual void update( int num, int direction){;}
	void     delete_rc( int num ){ list[num].state = DELETED; }
	void     expand_rc ( int num ){ list[num].state = EXPANDED;} 
	void     restore_rc( int num ){ list[num].state = NORMAL;}

	void     print_state();
	virtual    void print_state( int num ){;}
};

class RowManager : public RowColManager 
{
private:
    
public:

	RowManager(int size, int **ibool);
	void update( int col, int direction);
	void print_state( int col );
};

class ColManager  : public RowColManager 
{
    
public:
    
	ColManager( int size, int **ibool);
	void update( int row, int direction);
	void print_stat( int row); 
};
        
// the index matrix for matrix expansion
class IndexMatrix 
{

protected:
	int     size;          // the size of the matrix
	int     num_elem;      // the num of element in the matrix
	int     **Index;       // the index matrix
	int     **Bool;        // the boolean matrix
	DDDmanager    *ddd;    // DDD manager
    
	RowManager    *row_m; 
	ColManager     *col_m;

	static int    cur_label; // index used for varibale labeling

	friend class MNAMatrix;
public:
	IndexMatrix( int size , DDDmanager *_ddd);
	~IndexMatrix();
	void     initial_rowcol(); 
	void     reset_rowcol(); 

	void     print_bool();
	void     print_index();
	void     dump_index(ofstream & outfile);
	int      get_size_matrix() { return size;}
	int      get_num_elem() { return num_elem; }    
	void     add_element( int row, int col );
	void     add_label( int row, int col, int label );
	void     delete_element( int row, int col);

	// the row or col is the index value for each node
	void     set_label( int _row, int _col, int _index ) {Index[_row][_col] = _index;}
	int      get_label( int _row, int _col ) { return Index[_row][_col];}

	int      get_min_index();    
	/* element label(ordering) procedure */
	void     min_label_assignR();
	void     FindMinMarkowitzElement(int &, int &);
	void     assign_symbolic_elements(MNAMatrix *mna_matrix);
	void     force_label_assign();
	int      check_unassigned_elem();
	void     random_assign();

	/* matrix expansion procedure */
	DDDnode * matrix_recursive_expR(int de_row, int de_col); 
	DDDnode * matrix_recursive_expR()
		{ return matrix_recursive_expR( -1, -1 ); }

	void     add_minor(DDDnode **cur_ddd,DDDnode *new_ddd, int index);
    
	void     put_minor(DDDnode *_minor); // put minor into DDD hash table
	DDDnode *query_minor(); // get minor back from DDD hash tree according
	// to minor represented by row_m anc col_m 
	void ddd_statistic(DDDnode *_ddd) { ddd->statistic( _ddd );}
	static void reset_label() { cur_label = 1;}
};

extern IndexMatrix *theIndMax;

#endif //MATRIX_EXP_H

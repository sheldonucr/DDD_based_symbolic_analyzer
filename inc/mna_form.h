     
#ifndef MNA_FORM_H
#define MNA_FORM_H

/*
 *    $RCSfile: mna_form.h,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/09/29 17:11:51 $
 */

#include "node.h"
#include "branch.h"
#include "dtstruct.h"

#include "hash_map.h"

#ifndef bool
#define bool boolean
#endif

class HasherTree;

class DDDnode;
class CktCall;
class CompElemList;

/* MNA data structures: 
      An MNA matrix consists of a list of MNA rows (MNARow's).
      
      Each row has a list of MNAElements (->elem_list). 
      Each row also has a pointer to the pivot element for 
      that row (->pivot).  The MNA columns are maintained 
      by keeping pointers to variable nodes in each MNA 
      element (->node).


      e.g.          v1   v2    v3

                1   x,y  -x     z   
                
                2   a,-x  -a,x      

                3   -a    a     b

  mna{1}->row { v1, (x,y)} ->next { v2, (-x)} ->next { v3, (z)} ->next{NULL} 
    \
    next{2}->row { v1, (a,-x)} ->next { v2, (-a,x)} ->next{NULL}
      \
      next{3}->row { v1, (-a)} ->next { v2, (a)} ->next{ v3, (b)} ->next{NULL}
        \
        NULL


*/

class MNAMatrix;
class IndexMatrix;
class DDDmanager;
class CompElemList;

/* 
** the Modified Nodal Formulation for circuit equations setup 
*/

class MNAElement 
{
	int     flag;        // used to indicate the state of the element
	int     deleted;
	int     num_deled;   // deleted in numerator
	int     den_deled;   //deleted in denominator
	Node    *pivot;      // the corresponding pivot(row) node 
	Node    *node;       // the corresponding node variable for the element
	BranchList  *blist;  // branch related to the element
	CompElemList *clist;  /* the expressions from subckt 
							 if it is boundary element. */
	char *clist_sname;    // symbol for the clist
	MNAElement  *next;
	double      freq;     // frequency now used
	complex     value;    // the numerical value of this node for given freq

	bool m_is_symbolic:1;       // is the branch a symbolic branch. Used
                                // in semi-symbolic analysis
	friend class MNARow;
	friend class MNAMatrix;

public:
	MNAElement(Node * _node, Node *_pivot);
	~MNAElement();

	// the wrapper functions
	int        get_row() { return pivot->get_index();}
	int        get_col() { return node->get_index();}
	BranchList    *get_blist(){ return blist;}
	CompElemList  *get_clist() { return clist;}
	int     &     get_flag() { return this->flag; }
	Node  * &     get_node() { return this->node; }
	boolean        check_valid();
	complex elem_value( double _freq); 
	complex elem_cur_value() const {return value;} 
	rcl_complex get_cvalue() const { return this->blist->get_cvalue();}
	rcl_complex get_cvalue(PolyType ptype) const 
		{   
			this->blist->calculate_rcl_value(ptype); 
			return this->blist->get_cvalue();
		}

	void     add_clist( CompElemList *_clist);
	void     copy_clist( CompElemList *_clist);

	void     add_branch( Branch *branch, int sign) 
		{
			blist->add_branch(branch, sign);
			m_is_symbolic |= branch->is_symbolic();
		}
	void     add_branch( Branch *branch) 
		{ 
			blist->add_branch(branch, 1);
			m_is_symbolic |= branch->is_symbolic();
		}
	void     add_branch( Branch *branch, double _value, int _sign) 
		{ 
			blist->add_branch(branch, _value, _sign);
			m_is_symbolic |= branch->is_symbolic();
		}

	void set_is_symbolic(bool val) { m_is_symbolic = val;}	
	bool is_symbolic() { return m_is_symbolic;}
	
	// for decancellation
	int  find_findex(char *name, int cind);

	// for whole element elimination
	void calc_cvalue() { blist->calculate_rcl_value();}
	void update_cvalue(char *name, PolyType ptype) 
		{ blist->update_cvalue(name, ptype);}
	void restore_cvalue(char *name) { blist->restore_cvalue(name);}
	int  delete_branch(char *name, PolyType ptype) 
		{return blist->delete_branch(name,ptype);}
	int num_branch( PolyType ptype) { return blist->num_branch( ptype ); }

	MNAElement * Next() const { return next; }
	void Next(MNAElement *_next) { next = _next; }

	//void print_elem(ostream & out, myStack<CompElemList> & compList);

};

class MNARow 
{
	int isSpCurEq; /* boolean, indicates MNA row is a
					  special current output variable
					  equation */
	int  size;    // num of elements in the row
	Node *pivot;    // the pivot node for this row
	MNAElement *elem_list;    // the element list in this row    
	MNAMatrix  *matrix; // the MNA matrix the row belongs to
	MNARow     *next;        

	friend class MNAMatrix; 

public:
	MNARow(Node *_node, MNAMatrix *_matrix, MNARow *_next);
	~MNARow();

	MNARow * Next() const { return next;}
	void Next(MNARow *_next) { next = _next;}

	int       get_size() { return size;}
	Node *    get_pivot() { return pivot;}

	MNAElement * create_element( Node *node );
	MNAElement * add_create_elem( Node *node );
	void     add_element( MNAElement *elem );
	MNAElement * query_element( Node *node );
	MNAElement * query_element_by_index(int index);	

	int     check_element();

	void     calc_elems_value();
	void     calc_elems_value(PolyType ptype);

	void     process_branch(Branch *branch);
	void     process_rcl(Branch *branch);
	void     process_mutual_ind(Branch *branch);	
	void     process_cvs(Branch *branch);
	void     process_vccs(Branch *branch);
	void     process_cccs(Branch *branch);
	void     process_vi(Branch *branch);
	void     process_mosfet(Branch *branch);
	void     process_jfet(Branch *branch);
	void     process_bjt(Branch *branch);
	void     process_diode(Branch *branch);

	void     print_info();
};

typedef hash_map<int, MNAElement *, hash<int> > Int2ElemMap;
typedef hash_map<int, int, hash<int> > Int2IntMap;

// the complex DDD label to MNA matrix element map
class Label2MNA 
{
	int m_size; // the range of the labels
	Int2ElemMap label2ElemMap;
	Int2IntMap label2Row;
	Int2IntMap label2Col;	

public:
	Label2MNA(int size);
	~Label2MNA();

	// some wrapper functions
	int get_size() { return m_size;}
	void set_elem(int _index, MNAElement *_mna_elem);	
	MNAElement * get_elem(int _index);
	  
	void set_row( int _index , int _row );		
	int get_row(int _index);		
	void set_col( int _index , int _col );		
	int get_col(int _index);
	
	void swap_two_labels(int label1, int label2);

};

class MNAMatrix
{

	int    num_elem;        // number of elements in MNA matrix
	int    size;            // the dimension of the MNA matrix

protected:

	Circuit      *ckt;        // the circuit for the MNA
	DDDnode      *sys_det;    // the system determinant
	MNARow       *rowlist;    // MNA row list
	MNARow       *cur_row;    // the row under processing
	IndexMatrix  *index_mat;    // index matrix used for matrix expansion
	Label2MNA    *label2elem;    // the mapping from index to MNA elements
	DDDmanager   *ddd_mgr;    // DDD manager
	int          init_label;    // initial label for the circuit

	friend class MNARow; 
	friend class Circuit;
	friend class ACAnalysisManager;

public:

	MNAMatrix(Circuit *_ckt,  DDDmanager *_ddd);
	~MNAMatrix();
	MNARow *   new_mna_row(Node *node);
	int        get_label(int _row, int _col);

	// matrix dimension reduction
	void       delete_element( int label );

	Label2MNA *get_label2elem() { return label2elem;}
	MNARow    *find_row_real( int _node); // in local real number    
	void       process_subckt_call( CktCall *_call );

	// in local real num ber
	CompElemList  *build_comp_expr(int _row, int _col); 

	// in internal matrix row and col indices.
	MNAElement *get_mna_element(int _row, int _col);

	// in local real number
	MNAElement *get_mna_element_real(int _row, int _col); 
    
	int      get_num_elem() { return num_elem;}
	int      get_matrix_size() { return size;}
	IndexMatrix *get_imatrix() { return index_mat;}
	DDDnode* get_sys_det() { return sys_det; }

	int      max_branch_num();
	void     calc_value();
	void     calc_value(PolyType ptype);
	void     print_mna();
	void     print_mna(PolyType ptype);
	void     print_bool(); 
	void     create_index();
	void     create_mapping();
	DDDnode *build_sys_ddd();
	complex  evaluate_sys_det( double freq, HasherTree *);
	int      real2label( int real );
};

#endif /* ifndef MNA_FORM_H */ 

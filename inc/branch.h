#ifndef BRANCH_H
#define BRANCH_H

/*
 *    $RCSfile: branch.h,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/11/21 02:42:38 $
 */


/*
 * branch.h:
 *
 *     'branch' contains the row and column data structs
 *      and routines for manipulating them.
 *
 *
 * DEPENDENCY: node.h must be included prior to branch.h 
 */



/* branch states */

typedef enum 
{
	B_USED,     /* used branch */
	B_SYMB,     /* symbolic branch which including variable */
	B_DELETED,  /* deleted branch */
	B_EUQAL     /* the incident nodes which should be merged */
} BranchStat;
 
#define PLUS     1
#define MINUS     -1

#include "vector.h"
#include "hash_map.h"
#include "node.h"
#include "rclcompx.h"
#include "utility.h"

/* DEFINE's and TYPEDEF's */

class BranchList;
class Circuit;

class Branch 
{

protected:

	BranchStat stat ;            // branch state
	bool wasProcessed:1;         /* indicating if this branch
									has been processed previously */
	bool m_is_symbolic:1;       // is the branch a symbolic branch. Used
                                // in semi-symbolic analysis
	
	DeviceType type ;           /* Type of element, such as r,c,l  */
	int part;                   /* partition index */
	int num ;
	char *name;                 // branch name
	char *var_name;             // branch variable name if it is symoblic
						        // branch
	Node *node1 ;
	Node *node2 ;
	union {
		Node *cnode1 ;
		Node *node3 ;
	} extra1 ;
	union {
		Node *cnode2;          /* For controlled sources           */
		Node *node4;           /* For active elements              */
	} extra2 ;

	/* extra nodes for current or voltage variables: */
	Node *cvar ; // for current variable
	Node *evar ; // for voltage variable
	Branch *cbranch; // control branch for CCVS and CCVS

	// for mutual inductors
	Branch *m_ind1;  // inductor1
	Branch *m_ind2;  // inductor2

	char   *model_name; // the model name for nonlinear device

	double value ;      /*numeric value of element 0 if purely symolic*/
	rcl_complex cvalue; /* the rlc complex value */
	Branch *next ;

	friend class BranchList;
	friend class DummBranch;
	friend class Circuit;

public:
	// two nodes branch
	Branch(char *_keyword, DeviceType _type, 
		   Node *n1, Node *n2, double _value, int _index);
	Branch(char *_keyword, DeviceType _type, 
		   Node *n1, Node *n2, char *_var_name, int _index);
	// four nodes branch
	Branch(char *_keyword, DeviceType _type, 
		   Node *n1, Node *n2, Node *n3, Node *n4, 
		   double _value, int _index );
	Branch(char *keyword, DeviceType _type, 
		   Node *n1, Node *n2, Node *n3, Node *n4, 
		   char *_var_name, int _index );

	/* arbitrary branch */
	Branch(char *keywork, DeviceType _type, int _index);   

	~Branch();

	// wrapper functions
	Node *get_node1() { return node1;}
	Node *get_node2() { return node2;}
	Node *get_node3() { return extra1.node3;}
	Node *get_node4() { return extra2.node4;}
	BranchStat & get_stat() { return this->stat;}
	char * get_name() { return this->name;}
	DeviceType get_type() { return this->type;}
	Branch * get_cbranch() { return this->cbranch;}
	char * get_model_name() { return model_name; }

	void set_is_symbolic(bool val) { m_is_symbolic = val;}	
	bool is_symbolic() { return m_is_symbolic;}
	
	// for mutual industor
	void set_mut_ind1(Branch *val) { m_ind1 = val;}
	Branch *get_mut_ind1() { return m_ind1;}
	void set_mut_ind2(Branch *val) { m_ind2 = val;}
	Branch *get_mut_ind2() { return m_ind2;}

	bool was_processed() { return this->wasProcessed;} 
	void set_was_process(bool val) { this->wasProcessed = val;}
	
	void check_branch();
	void lump_branch(char *_keyword, double _value);

	void calc_rcl_value();
	rcl_complex get_cvalue() { return this->cvalue;}

	void set_dvalue (double val) { this->value = val; }	
	double get_dvalue() { return this->value;}

	Branch * Next() { return next;}
	void Next(Branch *_next) { next = _next;}

	void add_cur_variable(Node *var) { cvar = var;}
	void add_vol_variable(Node *var) { evar = var;}
	Node *get_cur_var() { return cvar;} 
	Node *get_vol_var() { return evar;} 

	void add_ctrl_branch(Branch *branch) { cbranch = branch;}
	void add_model_name(char *model); 

	void print_branch(ostream & out);
};

/* 
** Since there exist several branch lists in the course of
** circuit analysis, we need a dummy branch list
*/

class DummBranch {

protected:
	bool        num_deleted:1;   // flag for numerator;
	bool        den_deleted:1;   // flag for denominator;

	bool        m_is_symbolic:1;  // is the branch a symbolic branch. Used
                                 // in semi-symbolic analysis
	Branch         *branch;
	BType           type;        // Res, Cap or Ind type branch
	DummBranch     *next;
	double         sign;         // sign in cirucit equation 
	int            IsSpecial;    // if special branch(value is 1 or -1)
	double         value;        // can only be 1 or -1.
	double         bvalue;       // the branch value
    
	double         mut_ind_ratio; // the 1/(1-k) ratio for mutual	                              // inductor. set to 1.0 by default.

	friend class BranchList;

public:
	
	DummBranch(Branch *_branch, double _value, int _sign)
		{
			this->branch = _branch; IsSpecial = 1;
			this->value = _value; this->sign = (double)_sign; 
			this->next = NULL;
			num_deleted = den_deleted = 0;
			mut_ind_ratio = 1.0;
			m_is_symbolic = _branch->is_symbolic();		

			// must be the last statement
			calc_bvalue(); 
		} 

	DummBranch(Branch *_branch, int _sign ) 
		{ 		   
			this->branch = _branch; this->next = NULL;
			IsSpecial = 0; this->sign = (double)_sign; 
			num_deleted = den_deleted = 0;
			mut_ind_ratio = 1.0;
			m_is_symbolic = _branch->is_symbolic();			

			// must be the last statement
			calc_bvalue(); 
		};	

	DummBranch(Branch *_branch)
		{ 
			this->branch = _branch; this->next = NULL;
			IsSpecial = 0; this->sign = 1.0; 
			num_deleted = den_deleted = 0;
			mut_ind_ratio = 1.0;
			m_is_symbolic = _branch->is_symbolic();		

			// must be the last statement
			calc_bvalue(); 
		} 


	DummBranch(DummBranch *_dbranch)
		{
			branch = _dbranch->branch;
			next = NULL;
			sign = _dbranch->sign;
			IsSpecial = _dbranch->IsSpecial;
			value = _dbranch->value;
			mut_ind_ratio = _dbranch->mut_ind_ratio;			
			num_deleted = den_deleted = 0;
			m_is_symbolic = _dbranch->is_symbolic();
			
			
			// must be the last statement
			calc_bvalue();
		}

	~DummBranch(){;} //if(next) delete next;}

	// wrapper functions
	BType get_type() { return this->type; }
	double get_bvalue() { return this->bvalue; }
	double get_value() { return this->value;}
	char * get_bname() { return this->branch->get_name(); }
	void calc_bvalue();
	int get_num_deleted() { return num_deleted;}
	int get_den_deleted() { return den_deleted;}

	Branch * get_branch() { return branch;}
	int is_special() { return this->IsSpecial;}

	int get_sign() { return (int)sign;}

	void set_is_symbolic(bool val) { m_is_symbolic = val;}	
	bool is_symbolic() { return m_is_symbolic;}

	double get_mut_ind_ratio() { return mut_ind_ratio; }
	void set_mut_ind_ratio(double val) 
		{
			mut_ind_ratio = val;
		}

	DummBranch    * Next() const { return next;}
	void Next(DummBranch *_next) { next = _next;}
};

class BranchList {

protected:

	DummBranch    *blist;
	int        num;
	int        Nnum; // for numerator;
	int        Dnum; // for denominator;
	rcl_complex     cvalue;

public:

	BranchList() { blist = NULL; num = 0; cvalue = 0.0; Nnum = Dnum = 0;}
	~BranchList();

	DummBranch *get_blist() const {return blist;}
	int        get_num() const {return num;}

	void add_branch(Branch *branch, int sign);
	void add_branch(Branch *branch, double value, int sign);
	void add_branch(Branch *branch) { this->add_branch(branch, PLUS); }
	void add_dbranch(DummBranch *dbranch);

	void merge_blist(BranchList *_blist);
	BranchList * copy_blist( BranchList *_blist);

	rcl_complex get_cvalue() { return cvalue;}

	Branch * query_branch(const char *name);

	void calculate_rcl_value();
	void calculate_rcl_value(PolyType ptype);

	int chk_exist(const Branch *branch);

	void print_branch( ostream & out);
	void print_branch( ostream & out, PolyType ptype);

	// for full expansion
	DummBranch * get_dummbranch( int bindex );
	int get_branch_index( char *name );
	double    get_dbranch_val(int bindex) 
		{ return get_dummbranch( bindex )->get_bvalue();}
	BType   get_dbranch_type( int bindex )
		{ return get_dummbranch( bindex )->get_type();}
	char  * get_dbranch_name( int bindex )
		{ return get_dummbranch( bindex )->get_bname();}
    
	// for whole element elimination
	void update_cvalue(char *name, PolyType ptype);
	void restore_cvalue(char *name);
	int  delete_branch(char *name, PolyType ptype);
	int  num_branch(PolyType ptype);
};


/*
 * Class for a symbolic branch which indicates that the branch is
 * treated a symbolic branch and its variation range.
 */
class SymbBranch
       {    
		   char *name;
		   double start_value;  
		   double end_value;

		   public:
		   SymbBranch(char *new_name, double start, double end)
			   {
				   name = ToLower(CopyStr(new_name));
				   start_value = start;
				   end_value = end;
			   }
		   ~SymbBranch();
		   
		   // wrapper functions
		   char *get_name() { return name;}
		   double get_start_value() { return start_value;}
		   double get_end_value() { return end_value;}		   
       };

extern vector<SymbBranch *> theSymbBranchList;
typedef hash_map<DummBranch *, SymbBranch *,  HashFunc > DummBch2SymbBrchMap;

#endif //BRANCH_H

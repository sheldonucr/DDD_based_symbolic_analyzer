/* 
 * circuits.h:
 *
 *    Contains misc. global data structures and definitions.
 *
 */

/*
 *    $RCSfile: circuits.h,v $
 *    $Revision: 1.10 $
 *    $Date: 2002/11/21 02:42:38 $
 */

#ifndef CIRCUITS_H
#define CIRCUITS_H

#include "globals.h"
#include "node.h"
#include "branch.h"
#include "mna_form.h"
#include "rclcompx.h"
#include "ddd_base.h"
#include "control.h"
#include "hasher.h"

class sp_parser;
class SubCircuit;
class CktCall;
class Cofactor;

typedef enum { 
    cktTop, // the top circuit in circuit hierarchy 
    cktSub    // all the circuit levels lower than top circuit level
} CktLevel;

class Circuit {

	char *title;        // the first line in the input deck 
	char *spice_file;    // the circuit name

protected:

    CirType   cir_type;    // circuit type(linear or nonlinear
    CktLevel  cir_level;    // cktTop or cktTop
    int     nsize ;     // # nodes
    int     bsize ;     // # branchs
    int     extra_var_num;    // # num of extra variable 

    Node *rnlist;       /* used for parsing and setup of MNA matrix 
						   Sorted by node->num (internal number)*/

    Branch     *rblist ;    /* Only used for the main circuit at 
							   initial reading until B is built. */

    int        largestRealNode;

    CktCall    *ckt_call;    // sub-circuit calling list 
    CktCall    *cur_call;    // the current calling being parse
    SubCircuit    *sub_ckt;     // sub-circuit list;

    AnalysisType  analysisMode;  // analysis Mode -- AC, TRANSIENT,INIT_DC 
    ExpMethod     exp_method;    // matrix expansion method -- 

	/* OpParameter *op_param;  the DC small signal operating point 
	   paramater values of nonlinear device
	*/

    MNAMatrix     *matrix ;  // The MNA matrix formulation
    DDDmanager    *ddd;     // corresponding DDD representation

    friend class sp_parser;
    friend class IndexMatrix;
    friend class ACAnalysisManager;
    friend class CompElemList;
    friend int StatisticCmd(char *string);

public:

    ACAnalysisManager    *ac_mgr; // structure related to AC analysis

    /* defined in circuit.cpp */
    Circuit(char *cir_name ); // for main circuit instantiantion
    Circuit(); // for sub circuit instantiation
    ~Circuit(); 

	// some wrapper functions
    char *get_spice_file() { return spice_file;}
	MNAMatrix *  get_mna_matrix() { return  matrix ; }

    // parse the SPICE circuit input file 
    int read_spice_file(char *filename );
    void add_title( char *str ){ title = CopyStr(str); }

    /* get the DC operating opint parameters 
       for all nonlinear device obtained from SPICE */
    void get_op_data(){;} 

    /* misc convenient function */
    DDDmanager * & get_ddd() { return ddd;}
    CktLevel    which_level() { return cir_level;}
  
    /* functions for rnlist operation */
    Node *query_node( int _node);
    void add_node(Node *_node);
    void print_nodes(ostream & out);

    /* functions for rblist operation */
    void add_branch(Branch *_b);
    Branch * query_branch(const char  *_name);

    // lumpen RLC element
    Branch * check_lump( Node *, Node *, DeviceType );

    /* function for sub circuit */
    void add_sub_ckt(SubCircuit *_ckt);
    void add_ckt_call(CktCall *_call);

    /* parse function definied in parse_bch.cpp */
    void parse_rlc(char *, char *, char *, char *, char *);
    void parse_vi(char *, char *, char *, char *);
    void parse_vi(char *keyword, char *n1, char *n2) 
        { parse_vi(keyword, n1, n2, NULL); }
    void parse_vcs
	(
		char *keyword, char *n1, char *n2, char *cn1, 
		char *cn2, char *value, char *var_name
	);
    
	void parse_ccs
	(
		char *keyword, char *n1, char *n2, char *ctr_branch, 
		char *value, char *var_name
	);
	void parse_mutual_inductor(char *keyword, char *l1, char *l2, char *value);
	
    void parse_diode(char *keyword, char *n1, char *n2, char *mname);
    void parse_active(char *keyword, char *n1, char *n2, 
					  char *n3, char *n4, char *mname);
    void parse_bjt(char *keyword, char *n1, char *n2, 
				   char *n3, char *n4, char *mname)
		{parse_active(keyword, n1, n2, n3, NULL, mname);}
    void parse_jfet(char *keyword, char *n1, char *n2, 
                    char *n3, char *mname)
		{parse_active(keyword, n1, n2, n3, NULL, mname);}
    void parse_mosfet(char *keyword, char *n1, char *n2, 
					  char *n3, char *n4, char *mname)
		{parse_active(keyword, n1, n2, n3, n4, mname);}

    void parse_noise(char *, char *, char *, char *, char *);
    int read_nz_sources(char *);

    SubCircuit * parse_subckt( char *);
    void parse_ckt_call(char *);
    void parse_ckt_call_node( char *);
    void parse_ckt_call_param( char *, char *);
    
    /* parse function definied in parse_ctrl.cpp */
    void parse_ac_cmd(char *, char *, char *, char *);

    void parse_print(char *keyword, char *n1, char *n2);

    void linearizing(); // linearizing all the nonlinear circuit
    void build_mna(); // build the MNA matrix

    void statistic();

    // decancellation function
    void fcoeff_decancel();
    void fcoeff_gen_decancel(Branch *, Branch *);
    void fcoeff_decancel_op(int,int,int,int,char*, char*, char*, char*);
	// new decanllation function
	void fcoeff_new_decancel();
    void fcoeff_new_gen_decancel(Branch *, Branch *);
    void fcoeff_new_decancel_op(int,int,int,int,char*, char*, char*, char*);

    // approximation
    void whole_elem_elim();
    void whole_elem_elim_both();

    // polezero extraction
    void whole_elem_elim_polezero(int);

    // used in the DDD evaluation in subcircuits
    void sub_init_ht();

	// for semi-symbolic analysis
	void mark_symbolic_branchs();
	
};

// interface node list
class NList {
    int        node;
    NList    *next;

	friend class SubCircuit;
	friend class CktCall;

public:
    NList( int _node ) { node = _node; next = NULL;}
    ~NList() { if(next) delete next;}
    NList    *Next() { return next;}
    int get_node() { return node; }
}; 

// default variable name and value pair list
class DVarList {
    char        *name;
    double        value;
    DVarList    *next;

	friend class SubCircuit;
	friend class CktCall;

public:
    DVarList( char *_name, double _value )
        { name = CopyStr(ToLower(_name)); value = _value; next = NULL;}
    ~DVarList() { delete [] name; if(next) delete next;}
    DVarList    *Next() { return next;}
};

class SubCircuit : Circuit {

protected:
    SubCircuit  *parent; // the parent circuit of the sub-circuit
    char        *name; // sub-circuit name
    NList       *interface;    // interface node list
    DVarList    *var_list;    // variable name and value pair list
    int         num_infs_node;    // number of interface node
    int         num_param;    // number of parameters(variables)
    Cofactor    *cof_list; // the cofactors used by upper level ckt
    HasherTree  *ht; // used for evaluation
	HasherTree  *prec_ht; // used for pre-calculation.
    bool  	visit_flag; // used for DDD travesal
    SubCircuit  *next;

    friend class sp_parser;
    friend class CktCall;
    friend class Circuit;
    friend class CompElemList;

public:
    SubCircuit( SubCircuit *_parent, char *_name);
    ~SubCircuit();
    
    char * get_name() { return name;}
    
    void parse_inter_node( char *_cnode);
    void parse_deflt_value( char *_name, char *_cvalue);
    void add_inter_node( int _node );
    void add_deflt_value( char *_name, double _value );
    void add_cofactor( Cofactor *_cof);
    void init_ht();
    HasherTree * get_ht() {return ht;}
	HasherTree * get_prec_ht() { return prec_ht; }
    bool get_visit_flag() {return visit_flag;}
    Cofactor *query_cofactor(int _row, int _col);
    SubCircuit    *Next() { return next;}
};

/* 
**    some global variables for debugging and data collections
*/

// for collecting coeff or fcoeff related statistics
//#define COLLECT_COEFF_DATA

#endif //CIRCUITS_H

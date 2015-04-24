#ifndef AC_ANALYSIS_H
#define AC_ANALYSIS_H

/*
 *    $RCSfile: acan.h,v $
 *    $Revision: 1.10 $
 *    $Date: 2002/11/21 02:42:38 $
 */

#include "unixstd.h"
#include "globals.h"
#include "ddd_base.h"
#include "xdraw.h"
#include "coeff.h"
#include "fcoeff.h"
#include "dtstruct.h"

class ACAnalysis;
class ACAnalysisManager;
class Cofactor;
class ExpList;
class Circuit;

typedef enum {
	oMEG,    // magnitude -- default output format
	oREAL,    // real part
	oIMAG,    // imagine part
	oPHASE,  // phase part
	oNOISE,  // noise spetral density (V^2 or A^2 volt^2/Hz)
	oDB    // decibel -- 20*log10(magnitude)
} OutFormat;

/* the variation type for frequency swapping */
typedef enum {
	vDEC, // decade variation
	vOCT, // octave variation
	vLIN // linear variation
} VariationType;

// the input and output lists
class IOList {

protected:
	int        	   node1; // the positive node
	int        	   node2; // the negative node
	int    	   dep_freq; // 0: independent, 1, yes, 2, special function
	DeviceType     type; // only VOL or CUR allowed
	OutFormat      format;
	double         value; // value of the independent source
	IOList         *next;

	friend class ExpList;
	friend class Cofactor;
	friend class ACAnalysis;
	friend class ACAnalysisManager;

public:
	IOList( int, int, DeviceType, double, 
			OutFormat format = oMEG, int dep_freq = 0);
	~IOList();
	IOList  * Next() { return next;}
	void Next( IOList *_next) { next = _next;}
};
    
class ACAnalysis {

private:
	int    index;     // the internal No of this AC analysis
	VariationType   var_type; 
	int    np; 		// number of points in each variation range
	int    num_pts; 	// total frequency sampling points
	double    fstart; 	// start frequency
	double    fstop; 	// stop frequency
	double    *freq_pts; 	// calculated frequency points

	/* approximation */
	double    mag_err; // relative error for magnitude
	double    phs_err; // phase error for magnitude

	friend class ACAnalysisManager;
	friend class ExpList;
	friend class Cofactor;

public:

	ACAnalysis(VariationType _type, 
			   int _np, double _fstart, double _fstop);

	~ACAnalysis();
	int     check_params();
	void    cal_samp_pts();
	void    cal_freq_pts();
};

/* cofacor = (-1)^(row+col)*det(A) */
class Cofactor {

protected:

	DDDmanager    	*ddd_mgr; // the ddd manager used
	DDDnode     	*cddd; // complex DDD
	DDDnode     	*sddd; // simplifed DDD.
	char          *symb_name;	// the symbolic name of the cofacors (in subckt)
	int           sign;  // the sign of the cofactor
	int           osign;  // the original sign of the cofactor
	int           row;   // row deleted (in terms of MNA index)
	int           col;   // column deleted
	double       	freq;  // previous freq used
	complex       value; // the value of previous calculation
	Cofactor    	*next;
	CoeffList    	*coeff_list; // coefficient list 
	FCoeffList    *fcoeff_list; // fully expanded coefficient list 
	FCoeffList    *sfcoeff_list; // simplified fcoefficient list 

	friend class ACAnalysis;
	friend class ACAnalysisManager;

public:

	Cofactor(DDDmanager *,DDDnode *, int , int , int );
	~Cofactor();
	void     initial_calc(HasherTree *);
	void     sddd_init(HasherTree *);
	complex evaluate( double, HasherTree *, HasherTree *, bool); 
	int    get_row() { return row;}
	int    get_col() { return col;}
	int    get_sign() { return sign;}
	void     set_sign(int _sign) { sign = _sign;}
	DDDnode *get_cddd() { return cddd;}
	DDDnode *get_sddd() { return sddd;}
	void    set_sddd( DDDnode *_sddd ) { sddd = _sddd; }
	DDDmanager    *get_ddd_mgr() { return ddd_mgr; }
	FCoeffList *get_fcoeff_list() { return fcoeff_list;}
	FCoeffList *get_sfcoeff_list() { return sfcoeff_list;}

	Cofactor * Next() { return next;}
	void  Next(Cofactor *_next) { next = _next;}

	/* coefficient */
	complex coeff_evaluate(double ); 
	complex fcoeff_evaluate(double, bool);


	/* approximation */
	void simp_cofactor( double freq, double mag_ref );
	// generate dominant terms from fcofactor
	void simp_fcofactor_method1( double freq, double mag_ref );
	void simp_fcofactor_method2( double freq, double mag_ref );
	void simp_fcofactor_method3( double freq, double mag_ref );
	void simp_fcofactor_method4( double freq, double mag_ref );

	void fcoeff_supp(double freq, double meg_ref );    
	boolean elem_elim(double *_freq, complex *val,int _fnum, 
					  int index, PolyType _type);
	void fcoeff_elem_elim(int index, PolyType _type);

	void keep_coeff_ddd() 
		{ 
			if(coeff_list) coeff_list->keep_coeff_ddd();
		}
	void unkeep_coeff_ddd() 
		{ 
			if(coeff_list) coeff_list->unkeep_coeff_ddd();
		}
	void keep_fcoeff_ddd() 
		{ 
			if(fcoeff_list) fcoeff_list->keep_fcoeff_ddd();
		}
	void unkeep_fcoeff_ddd() 
		{ 
			if(fcoeff_list) fcoeff_list->unkeep_fcoeff_ddd();
		}
	void keep_sfcoeff_ddd() 
		{ 
			if(sfcoeff_list) sfcoeff_list->keep_fcoeff_ddd();
		}
	void unkeep_sfcoeff_ddd() 
		{ 
			if(sfcoeff_list) sfcoeff_list->unkeep_fcoeff_ddd();
		}

	void keep_cddd() { if(cddd) ddd_mgr->Keep(cddd);}
	void unkeep_cddd() { if(cddd) ddd_mgr->UnKeep(cddd);}
	void keep_sddd() { if(sddd) ddd_mgr->Keep(sddd);}
	void unkeep_sddd() { if(sddd) ddd_mgr->UnKeep(sddd);}

	void fcoeff_decancel(int, int, int, int);
	void whole_elem_elim(double *, complex *, int, PolyType, 
						 char *, int, int, int, int);
	void print_fcoeff_list(ostream & out);
	void fcoeff_print_transfer_func(ostream & out);
	

	// for pole/zero extraction
	int whole_elem_elim_polezero(char *, int, int, int, int, int);
	void factor_polezero(MNAMatrix *, int);

	// for noise analysis
	void maple_cmd( ostream& out );

	// print out symbolic expresion
	void print_expression(ostream & out);
	void addSymbName(char *name);
	char * get_symb_name() { return symb_name;}
};

/* Due to superposition, each independent input (voltage, current) has
** corresponding output contribution. Following class sperates each
** contribution of independent source and combine all of them togather
** for final output variable calculation.  
*/

class ExpList { 

protected:

	IOList   *input;   // the input nodes expression
	double   coeff;    // numerical coefficient of the expression
	Cofactor *num_list; /* the numerator cofactor list for input node 
						   if it is not ground node */
	Cofactor *den_list; /* the denominator cofactor list for a input node 
						   if it is not ground node */
	int     num_pts; // number of sampling points.
	int	    index;	// index of the polynomail expression.
	ExpList    *next;

	/* approximation */
	complex *num_val;  // values for numerator at each freq point
	complex *den_val;  // values for denominator at each freq point

	friend class ACAnalysisManager;
public:

	ExpList( IOList *, int , int,  double _coeff = 1.0);
	~ExpList();
	void     new_freq_number( int );
	void     add_num_cofactor( Cofactor *);
	void     add_den_cofactor( Cofactor *);
	complex evaluate( double, int, MNAMatrix *, HasherTree *, HasherTree *, bool, bool); 

	complex noise_evaluate( double, int, MNAMatrix *, HasherTree *, HasherTree *); 
	void print_expression(ostream & out);
	void maple_cmd( ostream& out );
	void print_transfer_func(ostream & out);


	/* coefficient */
	complex coeff_evaluate( double); 
	complex coeff_noise_evaluate( double); 
	complex fcoeff_evaluate( double, bool); 
	void calc_fcoeff_list();
	void calc_sfcoeff_list();

	ExpList *Next() { return next;}
	void Next(ExpList *_next) { next = _next;}

	/* approximation */
	void simp_expr( double , double , int );
	void simp_fcexpr(double , int );
	void fcoeff_supp_expr(double , double , int );
	int whole_elem_elim_both(DDDmanager *,  MNAMatrix *, double *, int , 
							 char *, int , int , int , int );
	void factor_ddd_node(DDDmanager *, MNAMatrix *, double *, int);
	int  find_diag_index(Branch *, int , IndexMatrix *);
};


class ACAnalysisManager {
 
protected:
 
	DDDmanager     *ddd_mgr;// the DDD manager used
	ACAnalysis     *command;
	MNAMatrix      *matrix;
	int            num_pts; // number of frequency points
	int		       num_exprs; // number of polynomial expressions
	complex        *out_value; // output result vector 
	complex        *exact_value; // exact output result vector
	GraphOutput    *graph_out; // the vector structure for display
	ExpList        *poly; // the output linear polymonial expression

	static IOList    *inputs; // the input stimula
	static int       input_num; // number of input stimulas
	static IOList    *outputs; // the output nodes
	static int       output_num; // number of output stimulas

	/* simplification */
	int ddd_simplied;
	int fcoeff_simped;

	friend class Circuit;

public:

	ACAnalysisManager(MNAMatrix *_matrix, DDDmanager * _ddd_mgr);
	~ACAnalysisManager();
	void         add_exprs( ExpList *expr); 
	DDDnode *    build_cofactor_ddd( int _row, int _col ); 
	void         build_sys_ddd();
	complex      evaluate( double , int, HasherTree *, HasherTree * , bool, bool);

	void         free_command();
	void         new_command( ACAnalysis *);
	void         add_command( ACAnalysis *);
	void         new_output( IOList *);


	void         ac_analysis(); 
	void         build_expressions(); 
	void         freq_sweep(bool, bool); 
	void         graph_display(char *);
	void         graph_compare(char *);
	void         text_output(char *_filename);

	// coefficient construction
	void        coeff_build_expressions();    
	complex     coeff_evaluate( double);
	void        coeff_freq_sweep();
	void        coeff_ac_analysis();

	// fully expanded coefficient list construction
	void        fcoeff_build_expressions_from_ddd(bool decancel);    
	void        fcoeff_build_expressions_from_sddd(bool decancel);    
	complex     fcoeff_evaluate( double _freq, bool use_simp);
	void        fcoeff_freq_sweep(bool);
	void        build_fcoeff_from_ddd(bool decancel);
	void        build_fcoeff_from_sddd(bool decancel);
	void        calc_fcoeff_list_value();
	void        calc_sfcoeff_list_value();
    void        transfer_func_cmd(ostream & out);

	// approximation
	void        simplify_ddd();
	void        simplify_coefflist();
	void        fcoeff_suppress();
	void        elem_elimination();
	void        fcoeff_elem_elimination();
	void        fcoeff_decancel(int, int, int, int);
	void        whole_elem_elim(char *, int, int, int, int);
	int         whole_elem_elim_both(char *, int, int, int, int);
	void        factor_ddd_node();

	/* garbage collection */

	void         forceGC(char *);
	void         forceGCForAll();

	void         coeff_GC();
	void         fcoeff_GC();
	void         coeff_forceGC();
	void         fcoeff_forceGC();
	void         sfcoeff_forceGC();
	void         cddd_forceGC();
	void         sddd_forceGC();

	/* noise analysis */
	complex      noise_evaluate( double, int, HasherTree *, HasherTree *);
	void         noise_freq_sweep(); 
	void         noise_analysis(); 

	complex      coeff_noise_evaluate( double); 
	void         coeff_noise_freq_sweep(); 
	void         coeff_noise_analysis(); 
	void	     maple_cmd(ostream& );
    
	int          get_output_num() const { return output_num;}
	ACAnalysis   *get_command() { return command;}

	void         print_fcoeff_list(ostream & out);
	static  	 void add_input( IOList *_inlist );
	static  	 void add_output( IOList *_output );

	void         new_freq_sweep(char *_spec);

	// for pole/zero extration
	int whole_elem_elim_polezero(char*,int, int, int, int, int);
	void factor_polezero(int);

	// for semi-symbolic analsys and dynamic ordering
	void complex_ddd_semi_variable_reordering();	
	void fcoeff_semi_symbolic_analysis();
	void complex_ddd_minimization_via_reordering();
  
	// print symbolic expression
	void print_expression(ostream & out);
};

// independant function
double noise_source_func(double);

#endif // AC_ANALYSIS_H

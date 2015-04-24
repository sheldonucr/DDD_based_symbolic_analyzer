#ifndef _FCOEFF_H
#define _FCOEFF_H

/*
 *    $RCSfile: fcoeff.h,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/11/21 02:42:38 $
 */

#include "unixstd.h"
#include "globals.h"
#include "ddd_base.h"
#include "hash_map.h"

/* we deal with fully expanded ( with respect to each circuit element )
   coefficient in following classes definition.
*/

class FCoeffList;
class Cofactor;
class DummBranch;

typedef hash_map<int, DummBranch*, hash<int> > Int2DummbchMap;
typedef hash_map<DummBranch*, int,  HashFunc > Dummbch2IntMap;
typedef hash_map<int, int,  hash<int> > Int2IntMap;


class FLabelMap 
{
    int offset;
    int modulus;
	
	// for dynmaic varibale ordering. In this case, we the label and
	// its dummbranch can't be computed by using fcoeff2ddd() and
	// get_index() functions anymore.

	// s-expanded DDD label tp dummy branch map
	Int2DummbchMap flabel2brchMap;

	// dummy branch to s-expanded DDD label map
	Dummbch2IntMap brch2flabelMap;
	
	// s-expanded DDD label to complex DDD label map 
	Int2IntMap flabel2labelMap;
	

    public:

    FLabelMap( int _offset, int _mod);
    ~FLabelMap() {;};

	// wrapper functions
	int get_offset() { return offset;}
	int get_modulus() { return modulus; }

	// add entry into the flabel2brchMap.
	void update_maps(int label, int flabel, DummBranch *dbch);
	void swap_two_labels(int flabel1, int flabel2);
	DummBranch * get_dummbranch(int flabel);	
	int get_flabel(DummBranch *bch);
	int get_complex_ddd_label(int flabel);

	void printFlabel2BchMap(ostream & out);		

	// this function is used to compute the initial label of a dummy
	// branch. It will not be valid once dynamic ordering is
	// performed.
    int ddd2fcoeff(int _olabel, int _index);

};

class FCoeff 
{
    int       order;    // order of the laplace variable s, i,e. s^order
    complex   value;    // value of the real coefficient
    double    dval;     // the value with without sign coming from power of s
    int       num_paths;    // number of product terms(paths)
    DDDnode *fcoeff_ddd;
    boolean  keep;       //keep the coefficient?
    DDDnode *prev_path;  // path found in previous simplifying step
    double   prev_pval;  // the corresponding path value

    friend class FCoeffList;
public:
    FCoeff();
    ~FCoeff();
    void     set_ddd_mgr(DDDmanager *);
    void     set_order(int _order){ order = _order;}
    int      get_order() const { return order;}
    DDDnode  *get_ddd() {return fcoeff_ddd;}
    void     set_ddd( DDDnode *_ddd ){ fcoeff_ddd = _ddd; }
    complex  fcoeff_value() const { return value;}
    double   get_dvalue() const { return dval; }
    void     calc_fcoeff(DDDmanager *, HasherTree *, HasherTree *, int);
    void     statistic(ostream& out);
    boolean  keep_val(){ return keep;}
    DDDnode * & Prev_path() { return prev_path;}
    double &  Prev_pval() { return prev_pval;}
};

class FCoeffList 
{

    FCoeff    *fcoeff;
    int        max_order;      // number of coefficients with different order
    int        offset;         // order offset; since the order is from -n to n
    DDDmanager *ddd_mgr;       // the ddd manager used.
    int        supp_init;      // flag for coefficient suppressions
    int        fcoeff_calced;  // if the fcoefficient has been calculated?
    double     cratio;         // fcoeff[pindex]/fcoeff[pindex+1];

    public:
    FCoeffList( DDDmanager *_ddd_mgr, int _num);
    ~FCoeffList();

    FCoeffList *fco_product
	(
		FCoeffList *_list, int _new_label, 
        ElemPart _part
	);
	FCoeffList *fco_product_with_decancel
	(
		FCoeffList *_list, int _new_label, 
        ElemPart _part
	);
	
    FCoeffList *fco_union(FCoeffList *_list1, FCoeffList *_list2);
    FCoeffList *fco_copy();
    FCoeffList *list_ddd_product
	(
		FCoeffList *_list, 
        DDDnode *_node, 
		PolyType 
		ptype,
		bool decancel
	);
    FCoeffList *first_fcoefflist(DDDnode *_node, PolyType ptype);

    void     calc_fcoeff_list(HasherTree *prec_ht, HasherTree *ht);
    complex evaluate( double); 
    void    statistic( ostream& out);
    void     keep_fcoeff_ddd();
    void     unkeep_fcoeff_ddd();
    int     get_max_order() { return max_order;}
	int     get_offset() { return offset; }

    DDDnode *get_fcoeff_ddd( int ind ) 
        { return fcoeff[ind].fcoeff_ddd; }
    void  set_fcoeff_ddd( DDDnode *_fcddd, int _ind ) 
        { fcoeff[_ind].fcoeff_ddd = _fcddd;}
    FCoeff *get_fcoeff( int ind ) { return &fcoeff[ind]; }


    void fcoeff_suppress(double _freq, double mag_ref);
    void fcoeff_suppress(int);
    void fcoeff_supp_init();
    void reset_fcoeff_calced() { fcoeff_calced = 0;}

    void fcoeff_elem_elim(int index, int bindex);
    void fcoeff_decancel(int, int, int, int);
    DDDnode * fcoeff_dyn_decancel(DDDnode *, DDDnode *, DDDnode *);
    DDDnode * fcoeff_decancel_index(DDDnode *, int, int, int, int);

	// functions for DDD size reduction.
	void simplify_fcoeff();	

    // functions for printing out symbolic terms
    void print_fcoeff_list(ostream& );
    void print_fcoeffs(DDDnode *, double, ostream& );
	void print_transfer_func(ostream& out, int sign);
	
    //void print_symbolic_path(DDDnode *, ostream& );

    // for pole and zero extraction
    void    calc_cratio( int pindex ) 
		{ 
			cratio =  (fcoeff[pindex].get_dvalue()
					   /fcoeff[pindex+1].get_dvalue());
			cout<<"exact pole/zero: "<<cratio<<endl;
		}
    double    get_cratio( int pindex ) { return cratio;}

	// for semi-symbolic analysis
	void semi_symbolic_statistic(ostream& out, 
								 HasherTree *node_ht,
								 HasherTree *term_ht );

};

extern FLabelMap *theFLabMap;

#endif //_FCOEFF_H

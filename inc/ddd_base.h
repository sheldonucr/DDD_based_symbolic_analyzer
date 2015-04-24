#ifndef DDD_BASE_H
#define DDD_BASE_H

/*
 *    $RCSfile: ddd_base.h,v $
 *    $Revision: 1.15 $
 *    $Date: 2002/11/21 02:42:38 $
 */


#include <iostream.h>
//#include <strstream.h>
#include "compable.h"
#include "hasher.h"
#include "hashcach.h"
#include "tnode.h"
#include "floatobj.h"
#include "dtstruct.h"
#include <stdlib.h>


#ifndef bool
#define bool boolean
#endif

#include "hashcach.h"
#include "floatobj.h"
#include "rclcompx.h"
#include "mna_form.h"

#define    DDD_PRIME  104729    
#define UNION       1 
#define INTSECT     2 
#define SUBSET1     3
#define SUBSET0     5
#define CHANGE      7
#define DIFF        11 
#define PRODUCT     12 
#define MINOR       13 
#define DELETION    14 

#define MAXINDEX       4095    

// Following dominant term generation algorithm, only one of them
// should enabled at any time.  

// dynamic programming based algorithm
//#define __USE_SECOND_TERM_GENERATION 

// use second shortest path term generation algorithm
//#define  __USE_THIRD_TERM_GENERATION 

// use incremental shortest path algorithm
#define __USE_FOURTH_TERM_GENERATION 

class DDDnode;
class DDDmanager;
class DDDcomputed1;
class DDDcomputed2;
class DDDcomputed3;
class FCoeffList;
class CoeffList;
class ACAnalysisManager;

//**
extern integer ZRand;

class DDDnode : public Tnode 
{
	integer index;
	bool dc;                 // if dc is true, all variables are don't care

	bool sign:1;             // node sign in denominator 
	bool visit_flag:1;       // used for DDD traversal
	complex *val_ptr;        // the determinant value of the node and its subtree
	CoeffList *coefflist;    // the coefficient list that the DDD represents
	FCoeffList *fcoefflist;  // the fully expanded coefficient list.

	/* used in approximation */
	bool color:1;    // 0:white and 1:black. The color of the DDD in BFS or DFS.
	DDDnode *pi;     // the predecessor of the DDDnode(in BFS)	

#ifdef __USE_SECOND_TERM_GENERATION
	integer tm_index;
	DDDstack *terms_stack;
#endif

#ifdef __USE_THIRD_TERM_GENERATION
	DDDUniqueStack *parents_stack;
#endif

#ifdef __USE_FOURTH_TERM_GENERATION
	//DDDstack *children;
#endif
public:

	DDDnode(integer v, DDDnode *a, DDDnode *b, boolean dcare)
		{
			index=v; 
			left=a; 
			right=b; 
			dc=dcare; 
			val_ptr=NULL;
			sign = 1; 
			visit_flag=0; 
			color = 0; 
			pi = NULL; 
			coefflist = NULL;
			fcoefflist = NULL;
			
#ifdef __USE_SECOND_TERM_GENERATION
			tm_index = 0;
			terms_stack = NULL;
#endif

#ifdef __USE_THIRD_TERM_GENERATION
			parents_stack = NULL;
#endif
			
#ifdef __USE_FOURTH_TERM_GENERATION
			//children = NULL;
#endif

		}
	DDDnode( DDDnode* a ) 
		{ 
			index=a->index; 
			left=a->left; 
			right=a->right; 
			dc=a->dc;
			val_ptr=a->val_ptr; 
			sign = a->sign; 
			visit_flag=a->visit_flag;
			color=a->color; 
			pi=a->pi; 
			coefflist =a->coefflist;
			fcoefflist=a->fcoefflist;			
#ifdef __USE_SECOND_TERM_GENERATION
			tm_index = a->tm_index;
			terms_stack = a->terms_stack;		
#endif


#ifdef __USE_THIRD_TERM_GENERATION
			parents_stack = a->parents_stack;
#endif	

#ifdef __USE_FOURTH_TERM_GENERATION
			//children = a->children;
#endif
		
		}
	~DDDnode() 
		{ 
			if(val_ptr) 
			{
				delete val_ptr;
				val_ptr = NULL;
			}
			left=NULL; 
			right=NULL;
			dc=0; 
			val_ptr=NULL;
			sign = 0; 
			visit_flag=0; 
			color = 0; 
			pi = NULL; 
			coefflist = NULL;
			fcoefflist = NULL;			
#ifdef __USE_SECOND_TERM_GENERATION
			if(terms_stack) 
			{
				delete terms_stack;
				terms_stack = NULL;
			}
#endif

#ifdef __USE_THIRD_TERM_GENERATION
			if(parents_stack)
			{
				delete parents_stack;
				parents_stack = NULL;
			}
#endif

#ifdef __USE_FOURTH_TERM_GENERATION
			/*
			if(children) 
			{
				delete children;
				children = NULL;
			}
			*/
#endif


		}

	//Virtual hash functions
	unsigned Hash( unsigned h = 0 ) 
		{
			return ( ((integer)index)   //<<  5)
					 ^ ((integer)Then()   <<  5)
					 ^ ((integer)Else()   << 11)
					 ^ ZRand
					 ^ (h << 1))  % DDD_PRIME;
		}
  
	integer isa() { return DDDNODE;}
	virtual integer Compare(Comparable *);

	inline integer Index() {return index;}
	inline void SetIndex(int idx) {index = idx;}

	inline complex GetValue() {return *val_ptr;}
	inline complex *GetValueP() {return val_ptr;}
	inline bool GetVisitFlag() { return visit_flag; }
	int getSign() { return sign;}
	boolean isDC()  {return dc;}
	inline DDDnode* Then() { return (DDDnode *) left;}
	inline DDDnode* Else() { return (DDDnode *) right;}

#ifdef __USE_SECOND_TERM_GENERATION
	DDDstack * get_terms_stack() { return terms_stack; }
#endif
	friend class DDDmanager;
  
	void Print( ostream& out = cout );
};


class DDDmanager : public Object 
{

protected:

	DDDnode *one;           // terminal 1
	DDDnode *zero;          // terminal 0
	integer num_inputs;      // total number of variables (levels)      
	integer num_nodes;       // total number of allocated nodes
	integer num_cubes;       // total number of cubes (1-pathes)

	Label2MNA    *label2elem;
  
	HasherTree** unique;         //Keeps each node unique
	HashCache *computed;         //Cache for computed nodes

	float CountR( DDDnode* P, HasherTree* ht);
	void NodesInFunctionR(DDDnode *f, HasherTree *ht);
   
	void doGC(integer);
	void RGC(DDDnode*);

public:

	DDDmanager( integer n=0 );
	~DDDmanager();

	integer NumInputs()   { return num_inputs; }
	integer NumLiterals() { return num_nodes; }
	integer NumCubes()    { return num_cubes; }
	integer NumNodes()    { return num_nodes; }   

	DDDnode* Empty()     { return zero; }
	DDDnode* Base()      { return one;  }

//    Generates a node with index v, and subgraphs g and h
	DDDnode* GetNode( integer v, DDDnode* g, DDDnode* h );
	DDDnode* InputNode( integer v ){ return GetNode( v, one, zero );}

	DDDnode* Subset1( DDDnode* P, integer var ); //Returns the 1 subset of P at var
	DDDnode* Subset0( DDDnode* P, integer var ); //Returns the 0 subset of P at var
	DDDnode* Change(  DDDnode* P, integer var );  //Returns P when var is inverted
//
	DDDnode* ExistentialAbstraction( DDDnode* P, integer var ) 
		{
			DDDnode* temp = Union(Subset0(P,var),Subset1(P,var));
			return ( Union(temp,Change(temp,var)));
		}
//
	DDDnode* Union( DDDnode *P, DDDnode *Q );   //Returns P or Q
	DDDnode* Product( DDDnode *P, DDDnode *Q );   //Returns P * Q
	DDDnode* Intsect( DDDnode* P, DDDnode* Q ); //Returns P and Q
	DDDnode* Diff( DDDnode* P, DDDnode* Q );    //Returns P - Q
	float     Count( DDDnode* P );                //Returns # of elements
	float     PathsInFunction(DDDnode* P) { return Count( P ); }
	integer   NodesInFunction(DDDnode* P);

	DDDnode* One()  { return one; }
	DDDnode* Zero() { return zero; }

	DDDnode* And( DDDnode* P, DDDnode* Q ) { return Intsect( P, Q ); }
	DDDnode* Or ( DDDnode* P, DDDnode* Q ) { return Union( P, Q ); }

	DDDnode* Universe(integer);

	void Keep  ( DDDnode *P) { P->Reference(); }
	void UnKeep( DDDnode *P) { P->UnReference(); }
	void ForceGC();
	void GC();
  
	void PrintComputed() { computed->Print(); }
	void PrintSets( DDDnode* P, ostream& out = cout );   //Prints Comb. Sets
	void PrintSetsR( DDDnode* P, cstring set, ostream& out = cout );
	void PrintNodes( DDDnode* P, ostream& out = cout );  //Prints DDD structure P
	void TermPrint( ostream& out = cout );         //Prints terminal nodes
	void PrintDDDTree( DDDnode *); // Print DDD tree in a bottom up fasion
	void PrintDDDTreeR( DDDnode *, HasherTree * ); 

	void PrintStats( ostream& out = cout );        //unique/computed stats... 
	void statistic( DDDnode * P, ostream & out = cout); // prints the statistis of DDDnode P

	// functions for minor hashing April, 1997
	DDDnode * GetMinor(DDDnode* row, DDDnode* col);
	void PutMinor(DDDnode* minor, DDDnode* row, DDDnode* col);

	// function related to determinant calculation
	void set_label2elem(Label2MNA *_ind){ label2elem = _ind; }
	Label2MNA * get_label2elem(){ return label2elem; }

	int CalcSign(DDDnode * , DDDnode * ); 
	void PreCalculationR(DDDnode *, HasherTree *);
	complex GetDetermValueR(DDDnode * ,double, HasherTree *); 
	complex GetValueFromDDD(DDDnode * P) {return *(P->val_ptr);}

	/* the functions related to DDD-based approximation */
	void DFS_ISS(DDDnode *);
	void DFS_Restore(DDDnode *);
	void Relax(DDDnode *, DDDnode*, double );
	DDDnode * DDDShortestPath(DDDnode*,double );
	DDDnode * ExtractPath();
	complex PathValue(DDDnode*, double );

	void FCRelax(DDDnode *, DDDnode*);
	DDDnode * FCDDDShortestPath(DDDnode*);
	double FCPathValue(DDDnode* node, bool compute_sign);

	/* function related to coefficient generation */
	CoeffList *generate_all_coeffs(DDDnode *, int);
	CoeffList *generate_all_coeffsR(DDDnode *, int, HasherTree *);
	double GetCoeffValueR(DDDnode *, HasherTree *); 
	void PreCoeffCalculationR(DDDnode * P);
	int CoeffCalcSign(DDDnode * Parent, DDDnode * Child); 
	double CalcCoeffCubeValue(cstring set, double error);

	/* function related to fully expanded coefficient generation */
	FCoeffList *generate_all_fcoeffs
	(
		DDDnode *, 
		int, 
		PolyType,
		bool decancel
	);
  
	FCoeffList *generate_all_fcoeffsR
	(
		DDDnode *, 
		int, 
		HasherTree *, 
		PolyType,
		bool decancel
	);
	
	void fcoeff_print_symbolic_path
	(
		DDDnode *path, 
		double   mul_adj,
		ostream& out
	);
	
	double GetFCoeffValueR(DDDnode *, HasherTree *); 
	void PreFCoeffCalculationR(DDDnode * P, HasherTree *);
	int FCoeffCalcSign(DDDnode * Parent, DDDnode * Child); 
	double CalcFCoeffCubeValue(cstring set, double error);
	
#ifdef __USE_SECOND_TERM_GENERATION
	// initialize the DDD for term generation.
	void FCSecondInitialize(DDDnode*);
	void FCSecondInitializeR(DDDnode *s, HasherTree *ht);


	// the second dominant term generation method.	
	DDDnode * FCSecondGetOneDominantTerm(DDDnode *);
	DDDnode * FCSecondGetDominantTermByIndex(DDDnode *, int);
	void FCSecondGenerateNextTerm(DDDnode *, int);

#endif

#ifdef __USE_THIRD_TERM_GENERATION

	void DFS_ISS_2(DDDnode *);
	void DFS_TRACE_2
	(
		DDDnode *s,
		DDDUniqueStack & stack,
		HasherTree *ht
	);
    // second shortest path term generation method
	DDDnode * FCDDDShortestPath_2(DDDnode**);
	
    // special Diff function for term generation algorithm
	DDDnode* DiffAndTopoSort
	(
		DDDnode* P, 
		DDDnode* Q,
		DDDUniqueStack & topoList
	); //Returns P - Q

#endif

#ifdef __USE_FOURTH_TERM_GENERATION

	// third shortest path term generation method
	//DDDnode * FCDDDShortestPath_4(DDDnode*);

	DDDnode * FCDDDShortestPath_4(DDDnode*);
	void DFS_ISS_4(DDDnode *, DDDqueue *);	
    	
	void FCRelax_4
	(
		DDDnode *u, 
		DDDnode *v
	);
	DDDnode * ExtractPath_4(DDDnode *s);
	
	DDDnode * FCDDDNextShortestPath_4(DDDnode **s);
    // special Diff function for term generation algorithm
	DDDnode* DiffAndRelax
	(
		DDDnode* P, 
		DDDnode* Q
	); //Returns P - Q
	
#endif

	///////////////////////////////////////////////////////////////
	// Functions for semi-symbolic analysis and dynamic ordering
	///////////////////////////////////////////////////////////////
	// functions for complex ddd reordering. 11/16/02
	void ComplexDDDVariableReorder();
	void ComplexDDDSwapVariableOrder(int index1, int index2);	

	// functions for fcoeff ddd reordering. 11/20/02
	void FcoeffVariableReorder(ACAnalysisManager    *ac_mgr);
	void FcoeffSwapVariableOrder(int index1, int index2);

	// appliable to both complex ddds and fcoeff ddds
	void SwapAdjacentVariableOrder(int index1, int index2);	

	// statistic functions
	integer FSymbNodesInFunction( DDDnode* f);
	void FSymbNodesInFunctionR(DDDnode *f, HasherTree *ht);
	
	integer FSymbTerminalsInFunction(DDDnode* f);
	void FSymbTerminalsInFunctionR(DDDnode *f, HasherTree *ht);
	
	float FSymbPathsInFunction(DDDnode* P );
	float FSymbPathsInFunctionR(DDDnode* P, HasherTree* ht);   

	// dynamic ordering for DDD minimization
	void ComplexDDDMinimizationByReordering();
	int SwapTwoVariableOrder(int index1,int index2); 
	
};


class DDDcomputed1 : public Comparable 
{

	DDDnode *f, *g;
	integer type;

public:
  
	DDDcomputed1(integer i, DDDnode *x, DDDnode *y)
		{ 
			f    = MaxNode( x,y ); 
			g    = MinNode( x,y ); 
			type = i;
		}

	DDDcomputed1( DDDcomputed1 *p )
		{ 
			f    = p->f;
			g    = p->g;
			type = p->type;
		}
	~DDDcomputed1() {;}

// utility functions
	DDDnode* MaxNode( DDDnode* P, DDDnode* Q ) 
		{   //Returns the larger of two
			return ((integer)P > (integer)Q)? P:Q;
		}

	DDDnode* MinNode( DDDnode* P, DDDnode* Q ) 
		{  //Returns the smaller of two
			return ((integer)P < (integer)Q)? P:Q;
		}

	unsigned Hash( unsigned n=0 )
		{
			return( ((integer) type) // << 5 )
					^ ((integer) f   << 5)
					^ ((integer) g   << 11)
					^ ( n << 1 )) % DDD_PRIME;
		}

	integer isa() { return DDDCOMPUTED1;}
	integer Compare(Comparable *);    
};


class DDDcomputed2 : public Comparable 
{

	DDDnode *f;
	integer type, var;

public:
  
	DDDcomputed2(integer i, DDDnode *x, integer j)
		{ f    = x; 
		type = i; 
		var  = j; }
	DDDcomputed2( DDDcomputed2 *p )
		{ f    = p->f;
      
		type = p->type;
		}
	~DDDcomputed2() {;}

	unsigned Hash( unsigned n=0 )
		{
			return( ((integer) type << 5 )
					^ ((integer) f   << 7)
					^ ((integer) var << 11)
					^ ( n << 8 )) % DDD_PRIME;
		}

	integer isa() { return DDDCOMPUTED2;}
	integer Compare(Comparable *);
};

class DDDcomputed3 : public Comparable 
{

	DDDnode *f, *g;
	integer type;

public:
  
	DDDcomputed3(integer i, DDDnode *x, DDDnode *y)
		{ f    = x; 
		g    = y;
		type = i;
		}

	DDDcomputed3( DDDcomputed3 *p )
		{ f    = p->f;
		g    = p->g;
		type = p->type;
		}
	~DDDcomputed3() {;}

// utility functions

	unsigned Hash( unsigned n=0 )
		{
			return( ((integer) type) // << 5 )
					^ ((integer) f   << 5)
					^ ((integer) g   << 11)
					^ ( n << 1 )) % DDD_PRIME;
		}

	integer isa() { return DDDCOMPUTED3;}
	integer Compare(Comparable *);    
};

#endif // DDD_BASE_H

#ifndef BDD_H
#define BDD_H
#define cachprofile

/*
 *    $RCSfile: bdd.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


////////////////////////////////////////////////////////////////////////////
//
// bdd.h : C++ BDD package based on [Brace, Rudell, and Bryant 27th DAC 1990]
//
// Andrew Sewawright
// 
////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <strstream.h>
#include "compable.h"
#include "hasher.h"
#include "hashcach.h"
#include "tnode.h"
#include "intobj.h"
#include "bv.h"
#include "list.h"
#include "odlist.h"
#include "indarray.h"
#include "floatobj.h"
#include "hasharry.h"
#include "unate_info.h"
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////

#define ITE_OP 1
#define COFACTOR_OP 2
#define RESTRICT_OP 3
#define XRESTRICT_OP 4
#define XRESTRICT2_OP 5
#define CORRELATION_OP 6
#define DISTINGUISH_OP 7

#ifndef BDD_PRIME        // assign these in private code if desired

// Use the following BDD_PRIMES : they are 2^BDD_PRIME_BITS - 1, and all
// three are prime numbers--the % BDD_PRIME can be done much faster when
// we use a prime which is also 2^n - 1

#define COFACTOR_PRIME_BITS 17
#define COFACTOR_PRIME 131071

#define BDD_PRIME_BITS 19
#define BDD_PRIME 524287

#endif

// Forward class declarations:
class BDDHasherArray;
class BDDmanager;
class BDDnode;
class CofactorHashKey;

typedef BDDHasherArray BDDHasher;

////////////////////////////////////////////////////////////////////////////

class BDDnode : public Tnode {
 protected:
  // a BDDnode contains whatever Tnode contains plus:

  BDDnode* next;

  BDDnode() : Tnode() {;} // reference, mark inited by tnode
  BDDnode(integer v, BDDnode *a, BDDnode *b) { index = v; left = a; right = b; reference=0; }
 ~BDDnode() {;}

 public:
  integer  GetIndex()    { return (integer)index; }
  integer  Level()    { return (integer)index; }// backwards compatibility

  // Comparable virtual functions
  virtual unsigned Hash(unsigned h=0) 
                {  
                integer ret = (   ((integer)Then()  << 5 )
                                ^ ((integer)Else()  << 7 )
                                ^ ((integer)index   << 11)
                                ^ (h << 8)            );
                return ret;
             }

  virtual unsigned Hash2( )
                { return (    ((integer)Then() << 5)
                    ^ ((integer)Else() << 7)
                                  ); }

  virtual integer Compare(Comparable *);
  BDDnode* Then() { return (BDDnode *) left;}  // True side
  BDDnode* Else() { return (BDDnode *) right;} // False side
  BDDnode* Next() { return next;}              // For Hash list... do not use
   
  // Object virtual functions
  virtual boolean Flush() { return (NumReferences()==0); }
  virtual integer isa() { return BDDNODE; }
  virtual void Print(ostream& out = cout);
  void PrintWithManager(BDDmanager *,HasherTree* ht,ostream& out = cout);

  friend class BDDmanager;
  friend class BDDHashTable;
  friend class BDDHasherTree;
  friend class BDDHasher_iterator;
};

////////////////////////////////////////////////////////////////////////////

class BDDnodeSave : public BDDnode {
 protected:
  BDDnodeSave(integer v, BDDnode *a, BDDnode *b) { index=v; left=a; right=b; reference = 0; }
 ~BDDnodeSave() {;}

 public:
  void    Reference() {;}
  void      UnReference() {;}
  unsigned NumReferences() { return 1; }

  friend class BDDmanager;
};

////////////////////////////////////////////////////////////////////////////
/*
class AutoBDDnode : public BDDnode { };

AutoBDDnode* operator=(AutoBDDnode* f, BDDnode *g ) {
  if( f != NULL ) f->UnReference();
  f=g;
  f->Reference();
  return f;
  }
*/
////////////////////////////////////////////////////////////////////////////

#include "bddcache.h"
class BDDCache;
class BDDCacheKey;

class BDDmanager : public Object {
  integer                timestamp;

public:
  BDDnode        *one;        // terminal 1
  BDDnode        *zero;        // terminal 0

  BDDnode        *tempnode;    // temporary used by MakeNode()
  BDDCacheKey        *tempcomputed;    // temporary used by ite()

  integer         num_inputs;    // current number of variables
  integer         num_nodes;    // total number of ALLOCATED bdd nodes
  integer         num_ite_calls;    // total number of ite() calls
  BDDHasher        *unique;      // unique "hash table"
  BDDCache        *computed;    // computed cache
  BDDCache        *cofactor_hc;    // cofactor cache
  BDDCache        *restrict_hc;    // restrict cache

  BDDmanager( integer bits=19, integer cofbits=17 );
  // used to copy index order from 'bdd2'
  BDDmanager( BDDmanager* bdd2, integer bits=19, integer cofbits=17 );
 ~BDDmanager();

  BDDnode* Copy2Manager( BDDnode *f, BDDmanager *bdd2, HasherTree *ht = NULL);
  void     Dump2Manager( BDDmanager *bdd2 );

  integer NumInputs() { return num_inputs; }
  integer NumNodes()  { return num_nodes; }
  integer NumNodes(integer level);
  integer GetLevel(BDDnode*);
  integer iteCalls()  { return num_ite_calls; }
  integer TimeStamp() { return timestamp;}
  boolean ReOrdered() { return timestamp != 0;}

  void PrintStructure(BDDnode *, ostream& out = cout);
  void PrintCubes(BDDnode *, char eol, ostream& out = cout);
  void PrintCubes(BDDnode *f, ostream& out = cout)
                { PrintCubes(f, '\n', out); }
  void PrintLabeledCubes(BDDnode *, char **var_names, int name_len=1,
             char eol='\n', ostream& out = cout);
  void Dump(ostream& out=cout);
  void WriteBinary(List *, ostream& out);
  List *ReadBinary(istream &in);

  // XDump takes a BDDnode, a list of BDDnodes, or nothing (unique hasher)
  void XDump(Object *f = NULL, char* window_title="BDD Tree");    // defaults to entire unique hasher
  void XDump_nowait(Object *f = NULL, char* window_title="BDD Tree");    // defaults to entire unique hasher

  void GC();
  void ForceGC();
  void GC_Sift(int start = 0, int end = -1, int mod = 1);
  void GC_Window3(int start = 0, int end = -1, int mod = 1);

  void Keep(BDDnode*f)    { f->Reference();   }
  void UnKeep(BDDnode *f) { f->UnReference(); } 

  BDDnode* ite    (BDDnode*, BDDnode*, BDDnode*);

  BDDnode* not  (BDDnode *f)              { return ite(f, zero, one);    }
  BDDnode* and  (BDDnode *f, BDDnode *g);
  BDDnode* or   (BDDnode *f, BDDnode *g)  { return ite(f, one,    g);     }
  BDDnode* nand (BDDnode *f, BDDnode *g)  { return ite(f, not(g), one);   }
  BDDnode* nor  (BDDnode *f, BDDnode *g)  { return ite(f, zero,   not(g));}
  BDDnode* xor  (BDDnode *f, BDDnode *g)  { return ite(f, not(g), g);     }
  BDDnode* xnor (BDDnode *f, BDDnode *g)  { return ite(f, g,      not(g));}
  BDDnode* diff (BDDnode *f, BDDnode *g)  { return ite(g, zero,   f);     }

  boolean  Intersect( BDDnode* f, BDDnode* g, HasherTree* ht=NULL);  // do f and g intersect?
  boolean  Subset   ( BDDnode* f, BDDnode* g, HasherTree* ht=NULL);  // is g a subset of f?
  BDDnode* Cofactor (BDDnode *, BDDnode *); // Generalized Cofactoring Alg
  BDDnode* Constrain (BDDnode *f, BDDnode *c) { return Cofactor (f,c); }
  BDDnode* Restrict (BDDnode *, BDDnode *);
  BDDnode* XRestrict (BDDnode *, BDDnode *, boolean greedy=FALSE); // experimental restrict
  BDDnode* Compose (BDDnode *f, BDDnode *v, BDDnode *g);
  BDDnode* CondConstraint (BDDnode *f, BDDnode *v, BDDnode *g);

  BDDnode* ExistentialAbstraction (BDDnode*, List *);
  BDDnode* Smooth(BDDnode*, integer, integer, integer m=1, integer o=0, HasherTree* ht=NULL);
  BDDnode* Smooth(BDDnode*, ODList*, HasherTree* ht=NULL, boolean InList=TRUE );
  BDDnode* Smooth(BDDnode*, IndexArray*, HasherTree* ht=NULL, boolean InList=TRUE );
  BDDnode* USmooth(BDDnode*, IndexArray*, HasherTree* ht=NULL, boolean InList=TRUE );
  BDDnode* UniversalAbstraction (BDDnode*, List *);
  BDDnode* VariableShift(BDDnode*, integer shift);
  BDDnode* VariableShift(BDDnode*, integer shift, integer max, integer min=0);
  BDDnode* VariableShift(BDDnode*, integer shift, IndexArray *ia);
  void     VariableSwap( int upper_index );
  void       SwapMVar( int level, int mod );
  void     Swap2Var( int index1, int index2 );
  void       Reorder_Sift(int downto = 0, int from = -1, int group_size = 1);
  void       Reorder_Window3(int downto = 0, int from = -1, int group_size = 1);

  BDDnode* Translate(BDDnode *f, HasherTree *translate, HasherTree *dup_check=NULL);

  BDDnode* RealizeCube( string cube_descr);
  BDDnode* Read(istream& in = cin);
  BDDnode* ReadCubes(istream& in=cin) { return ReadEq( in);}
  BDDnode* ReadEq  (istream& in=cin, integer base=0, char eol='\n') ;
//  BDDnode* ReadEq  (char* in, integer base=0, char eol='\n') { return ReadEq  (istrstream(in), base, eol);}
  BDDnode* ReadCube(istream& in=cin, integer base=0, char eol='\n');
//BDDnode* ReadCube(char* in, integer base=0, char eol='\n') { return ReadCube(istrstream(in), base, eol);}
  integer  NodesInFunction(BDDnode *, HasherTree* = NULL);
  integer  NumCubes(BDDnode *f)            { return (integer)PathsInFunction(f); }
  integer  PathsToOne(BDDnode *f)          { return (integer)PathsInFunction(f); }
  double   PathsInFunction(BDDnode *);  
  integer  NumMinterms(BDDnode *f, integer n) { return OnSetSize(f,n); }
  double   OnSetSizeD(BDDnode *, integer, HasherTree* = NULL);
  float    OnSetSizeF(BDDnode *, integer, HasherTree* = NULL);
  integer  OnSetSize(BDDnode *, integer, HasherTree* = NULL);
  integer  DepthOfFunction(BDDnode *);
  integer  NumSupport(BDDnode *);
  float    Correlation(BDDnode* f1, BDDnode* f2, HasherTree* corr_ht = 0);
  void     MaxMinterms( BDDnode *set, int bitsOn, int h, List *, HasherTree *onsetsize );

  void       Support(BDDnode*, HasherTree*); 
  boolean    Evaluate(BDDnode*, BitVector *);
  BDDnode*   Code2Cube(List* functions, integer code);
  BDDnode*   Range(List *);
  integer*   NodeDistribution(BDDnode*, integer* i=NULL);
  BitVector* DCterms ( BDDnode* f, integer bot_index=0, integer top_index=-1);
///////
  UnateInfo** FormUnateArray(BDDnode*, integer, UnateInfo**);
  BitVector   FindOnePath(BDDnode*);
  BitVector   FindOnePath1(BDDnode*, integer);
///////
  OList*   ListCubes(BDDnode* f, integer min, integer max);
  BDDnode* BuildHamming(BDDnode* minterm, BDDnode* constraint, integer hamming_distance);
  BDDnode* DC2Zero(BDDnode*, IndexArray* ia, HasherTree* ht=NULL);  
  BDDnode* DC2Zero(BDDnode*, integer index, HasherTree* ht=NULL);  
  BDDnode* DC2One (BDDnode*, IndexArray* ia, HasherTree* ht=NULL);  
  BDDnode* DC2One (BDDnode*, integer index, HasherTree* ht=NULL);  
  BDDnode* One2DC (BDDnode*, IndexArray*, HasherTree* ht=NULL);
  BDDnode* Zero2DC (BDDnode*, IndexArray*, HasherTree* ht=NULL);
  BDDnode* SampleCube( BDDnode* f, boolean var_pref=TRUE);     
  IndexArray* Minterm2IA( BDDnode* set );

  BDDnode* UpToN   ( integer n, IndexArray* ia);
  BDDnode* ExactlyN( integer n, IndexArray* ia) { return Tuple( n, ia);}
  BDDnode* AtLeastN( integer n, IndexArray* ia);
  BDDnode* Tuple   ( integer n, IndexArray* ia);
  BDDnode* SetUnion( BDDnode*, IndexArray*, HasherTree* ht=NULL);
  BDDnode* Distinguish(BDDnode *s1, BDDnode *s2, integer max, IndexArray *va, HasherTree *ht=NULL);
  BDDnode* Distinguish(BDDnode *s, integer max, IndexArray *va, HasherTree *ht=NULL);

  BDDnode* One()                { return one; }
  BDDnode* Zero()               { return zero; }
  BDDnode* Input(integer n)
    { return (MakeNode(n, one, zero)); }
  BDDnode* Inputs(integer n)    { return Input(n);}
  void     OrderIndexes( IndexArray *ia );

  // Object virtual functions

  virtual void Print(ostream &out = cout);
  virtual void Scan (istream &in  = cin) { Read(in);}
  virtual integer isa () {return BDDMANAGER;}
  
  // Temporary functions

  void        FindRLE();
  void     CountFunctions(integer index, ostream& out = cout);
  void       CheckReferenceCounts( ostream& out = cout);
  void       ProfileBDDnodes( ostream& out = cout);


friend class ZBDDmanager;
friend class XZBDDmanager;

private:
  virtual  BDDnode *MakeNode(integer, BDDnode *, BDDnode *);
  boolean  IntersectR( BDDnode*, BDDnode*, HasherTree*);
  boolean  SubsetR( BDDnode*, BDDnode*, HasherTree*);
  void     NodesInFunctionR(BDDnode *, HasherTree *);
  double  PathsInFunctionR(BDDnode *, HasherTree *);
  void     DepthOfFunctionR(BDDnode *, integer, integer&, HasherTree *);
  BDDnode* SmoothR(BDDnode*, IndexArray*, HasherTree*, boolean );
  BDDnode* USmoothR(BDDnode*, IndexArray*, HasherTree*, boolean );
  void     SupportR(BDDnode*, HasherTree*, HasherTree*); 
  void     OnSetSize(BDDnode *, HasherTree*, integer& p, integer& h);
  void     OnSetSizeFR(BDDnode *, HasherTree*, float& p, integer& h);
  void     OnSetSizeDR(BDDnode *, HasherTree*, double& p, integer& h);
  void     ListCubesR(BDDnode* f, string cube, OList* cubes, integer min, integer max);
  BDDnode* DC2ZeroR(BDDnode*, IndexArray* ia, HasherTree* ht);  
  BDDnode* DC2ZeroR(BDDnode*, integer level, HasherTree* ht);  
  BDDnode* DC2OneR (BDDnode*, IndexArray* ia, HasherTree* ht);
  BDDnode* DC2OneR (BDDnode*, integer level, HasherTree* ht);
  BDDnode* One2DCR (BDDnode*, IndexArray* ia, HasherTree* ht);
  BDDnode* Zero2DCR (BDDnode*, IndexArray* ia, HasherTree* ht);
  BDDnode* SetUnionR( BDDnode*, IndexArray*, HasherTree* ht);
  BDDnode* DistinguishR(BDDnode *s1, BDDnode *s2, integer max, HasherTree *ht, IndexArray *ia, BDDnode **t);
  BDDnode* DistinguishR(BDDnode *s1, integer max, IndexArray *ia, HasherTree *ht, BDDnode **t);
  boolean  XRestrictIgnoreVar( BDDnode *fchild, BDDnode *f, BDDnode *c, BDDnode *cchild );
  BDDnode* XRestrictIgnoreVarR( BDDnode *f, BDDnode *g, integer &count );
  boolean  XRestrictRedCheck( BDDnode *f, BDDnode *c, BDDnode* f1, BDDnode *c1,
                integer &count1, integer &count2 );
  BitVector* DCtermsR( BDDnode* f, BitVector* bv, integer bot, integer top, HasherTree* ht);
///////
  void     FormUnateArrayR(BDDnode*, UnateInfo**, HasherTree*);
  boolean  FindOnePathR(BDDnode*, BitVector&);
///////
  void     PrintStructureR(BDDnode *, HasherTree *ht, ostream& out = cout);
  void     PrintCubesR( BDDnode *, string, ostream& out = cout, char eol='\n');
  void PrintLabeledCubesR(BDDnode *, string, ostream& out = cout, char eol='\n',
              char **var_names=NULL, int pos=0);
  void       RemoveNodeR( BDDnode* node );
  void     doGC(boolean clear_caches);
  void     RGC(BDDnode *);

  BDDnode* VariableShift(BDDnode* , integer shift, HasherTree* ht);
  BDDnode* VariableShift(BDDnode* , integer shift, HasherTree* ht, IndexArray *ia);
  void       VSGSR( BDDnode* );
  BDDnode* TranslateR(BDDnode *f, HasherTree *translate, HasherTree *dup_check=NULL);
  BDDnode* Copy2ManagerR( BDDnode *f, BDDmanager *bdd2, HasherTree *ht = NULL);
  BDDnode* FastCopy2ManagerR( BDDnode *f, BDDmanager *bdd2, HasherTree *ht = NULL);
  void     WriteBinaryR(BDDnode *, ostream &out, HasherTree *);
  BDDnode* CopyNode( BDDnode*, BDDnode *, BDDnode *, BDDnode *, HasherTree& );
  void     NodeDistributionR(BDDnode*, HasherTree*, integer*);
  float    CorrelationR(BDDnode* f1, BDDnode* f2, HasherTree* corr_ht);

  BDDnode* Shannon1(BDDnode* n, integer v)
  {
    // assumes v >= n->index
    return (v == n->index) ? n->Then() : n;
  }

  BDDnode* Shannon0(BDDnode* n, integer v)
  {
    // assumes v >= n->index
    return (v == n->index) ? n->Else() : n;
  }

};

////////////////////////////////////////////////////////////////////////////

class BDDsubstitution : public Comparable {

  BDDnode *function;
  BDDnode *variable;

  friend class BDDmanager;

public:
  BDDsubstitution () {;}
  BDDsubstitution (BDDnode *f, BDDnode *v) { function = f; variable = v;}
 ~BDDsubstitution () {;}
};

////////////////////////////////////////////////////////////////////////////

#endif


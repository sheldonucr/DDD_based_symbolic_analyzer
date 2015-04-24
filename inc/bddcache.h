#ifndef BDD_CACHE_H
#define BDD_CACHE_H

/*
 *    $RCSfile: bddcache.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#include "mem.h"
#include "object.h"
#include "bdd.h"

extern MemGroup mem_default_group;

#define BDD_CACHE_MOD( x ) x = (x & bdd_prime ) + (x >> bdd_prime_bits); \
               x = (x & bdd_prime ) + (x >> bdd_prime_bits);

class BDDnode;


class BDDCacheKey : public Comparable {

  integer op;
  BDDnode *f, *g, *h;

public:
  BDDCacheKey() {;}
  BDDCacheKey( integer o,BDDnode* x,BDDnode* y,BDDnode* z=NULL) { op=o; f=x; g=y; h=z; }
 ~BDDCacheKey() {;}
  
  void    Set(integer o,BDDnode* x,BDDnode* y,BDDnode* z=NULL) { op=o; f=x; g=y; h=z; }
  void    Set( BDDCacheKey& key) { op = key.op; f=key.f; g=key.g; h=key.h; }

  virtual unsigned Hash(unsigned i=0){ return(    ((integer)f << 3)
                        ^ ((integer)g << 5)
                        ^ ((integer)h << 8)
                        ^ i << 1);            }

  unsigned Hash2()          { return(   ((integer)f << 3)
                    ^ ((integer)g << 5)
                    ^ ((integer)h << 8)  ); }
  
  void ReferenceAll()    { f->Reference(); g->Reference() ; if(h) h->Reference(); }
  void UnReferenceAll()    { f->UnReference(); g->UnReference() ; if(h) h->UnReference(); }

  integer Compare( integer o, BDDnode *x, BDDnode *y, BDDnode *z) {
    return ( x != f || y != g || z != h || o!=op); }
  virtual integer Compare( Comparable* key ) {
    BDDCacheKey* k= (BDDCacheKey*) key;
    return ( (k->f != f) || (k->g != g) || (k->h != h) || (op != k->op) ); }

  // this saves a virtual function call!

  integer NotEqual( BDDCacheKey* k ) {
    return ( (k->f != f) || (k->g != g) || (k->h != h) || (op != k->op) ); }
    
  boolean Flush()    { return (f->NumReferences()==0  ||
                 (g->NumReferences()==0) ||
                  (h&&h->NumReferences()==0)   ); }

  virtual void Print( ostream& out = cout);
  virtual integer isa() {return BDDCOMPUTED;}

};

////////////////////////////////////////////////////////////////////////////////////

class BDDCache : public Object {

protected:

  BDDCacheKey     **keys;
  BDDnode    **result;
  integer      bdd_prime;
  integer      bdd_prime_bits;
  integer      puts;
  integer      collisions;
  integer      hits;
  integer      total;

public:
  
  BDDCache( integer prime, integer bits );
 ~BDDCache();
  
  void        Replace(BDDnode*, BDDCacheKey&);// 3 input cache (ite)
  boolean    Get( BDDnode**, BDDCacheKey& ); // 3 input cache

  void        Clear();
  void        ConditionalClear();
  void        KeepNodes();
  void        UnKeepNodes();

  virtual void     Print( ostream& out = cout);
  void PrintLong( ostream& out = cout);

  virtual integer isa() { return BDD_CACHE; }
};

/////////////////////////////////////////////////////////////////////////////////////

#endif

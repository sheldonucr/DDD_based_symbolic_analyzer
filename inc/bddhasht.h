#ifndef BDD_HASH_TABLE_H
#define BDD_HASH_TABLE_H

/*
 *    $RCSfile: bddhasht.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#define START_SIZE 1

#define BDD_HASH_TABLE_MOD( x )    x = x % bincount;

//#define BDD_HASH_TABLE_MOD( x ) while( x >= bincount) {    x = ( x & bincount) + ( x >> (2 * hsize + 1)); }

#include "bdd.h"

class BDDnode;
class BDDmanager;

class BDDHashTable : public Object {

  friend class BDDmanager;
  friend class BDDHasherArray;
  friend class BubbleMgr;

protected:
  BDDnode **bin;
  integer bincount, hsize;
  integer count;

#ifdef DEBUG
  integer maxDepth;
  integer rehashCount;
  integer puts,hits,total;
#endif
  
public:
  integer Count()  { return count; }
  
  BDDHashTable();
  BDDHashTable(int size);
 ~BDDHashTable()      { delete bin; }
  static unsigned hash_size[];
  
  void        Add      ( BDDnode *a);    // don't check uniqueness
  boolean     Put      ( BDDnode *a);    // ensures uniqueness
  BDDnode    *Get      ( BDDnode *a);    // returns node or NULL
  BDDnode    *ForceGet ( BDDnode *a);    // returns node or NULL
  boolean     Remove   ( BDDnode *a);    // return TRUE if found

  void Grow()    { if( count > bincount * 6) Rehash(+1); };
  void Shrink();
  void Rehash(integer change);

  void Clear();
  void ConditionalClear();
  void PrintStats(ostream &out = cout);
  void PrintHistogram(ostream &out = cout);
  
  virtual void Print(ostream &out = cout);
};

inline ostream& operator << (ostream &out, BDDHashTable& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, BDDHashTable* f)
{ out << "(ptr->)"; f->Print(out); return out; }

#define BDDHashTable_ForEach_Node(table,ob) { \
  integer _bddhtIt; \
  BDDnode *ob; \
  for( _bddhtIt = 0; _bddhtIt < table->bincount; _bddhtIt++) { \
    for( ob = table->bin[_bddhtIt] ; ob ; ob = ob->next ) {

#define BDDHashTable_End_ForEach }}} \

#endif // BDD_HASH_TABLE_H


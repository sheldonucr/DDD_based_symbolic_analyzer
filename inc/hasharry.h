/*
 *    $RCSfile: hasharry.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#ifndef BDD_HASHER_ARRAY_H
#define BDD_HASHER_ARRAY_H

#include "bddhasht.h"
#include "bdd.h"

#define MAX_BDD_VAR 10000
////////////////////////////////////////////////////////////////////////////
class BDDnode;
class BDDHashTable;

typedef BDDHashTable   SubHasher;
typedef SubHasher * TablePtr;

class BDDHasherArray : public Object {
protected:
  integer count;
  integer numTables;
  integer *lookup_level;
  integer *lookup_index;
  integer maxTables;

public:
  TablePtr *hashTables;

  BDDHasherArray();
  BDDHasherArray( integer *lui, integer *lul );
  BDDHasherArray(BDDHasherArray *, boolean copy=FALSE);
 ~BDDHasherArray();        // clear hashers

  integer Count() { return count;}
  void Clear();

  void    AddHasher( );
  integer NumTables()    { return numTables; }

  // in BDD's, the key and object (data) are the same
  boolean Put      (BDDnode* o);    // returns TRUE if successful
  void    ForcePut (BDDnode *o);    // jam it!!
  BDDnode *ForceGet (BDDnode *o);    // get it, or else put it there


  BDDnode* Get (BDDnode* key);
  BDDnode* Get (BDDnode& key) { return Get( &key); }

  // returns TRUE if in tree
  boolean Get (BDDnode** o, BDDnode* key) { return NULL != ( *o=Get( key));}

  boolean Remove( BDDnode *node );

  void    SwapIndex( integer level ) {
    ASSERT( lookup_index, "Use new BDDHasherArray");
    SWAP( integer, lookup_index[level], lookup_index[level-1] );
  }
  void      SwapLevel( integer index1, integer index2 ) {
    ASSERT( lookup_level, "Use new BDDHasherArray");
    SWAP( integer, lookup_level[index1], lookup_level[index2] );
  }
  integer LookupIndex( integer level )    { return (lookup_index)?lookup_index[level]:0; }
  integer LookupLevel( integer index )  { return (lookup_level)?lookup_level[index]:0; }

  void    Print ( ostream& out = cout);        // print the contents
  friend ostream& operator << (ostream &out, BDDHasherArray f) { f.Print(out); return out; }
  friend ostream& operator << (ostream &out, BDDHasherArray* f)
                      { out << "(ptr->)"; f->Print(out); return out; }

  void    ShortPrint ( ostream& out = cout);    // just stats
  integer isa() { return BDDHASHERARRAY; }

  friend class BDDmanager;
};

////////////////////////////////////////////////////////////////////////////

#define BDDHasherArray_ForEach_Type(ht, o)  \
        { \
         integer _level; \
         integer _bddhtIt; \
         BDDnode *o, *next; \
         for ( _level=0 ; _level < ht->NumTables() ; _level++ ) { \
           SubHasher* _ht = ht->hashTables[_level]; \
 \
           for( _bddhtIt = 0; _bddhtIt < _ht->bincount; _bddhtIt++) { \
         o = _ht->bin[_bddhtIt]; \
             for( ; o ; o=next ) { \
           if( o ) next=o->Next();



#define BDDHasherArray_ForEach_in_Level(ht, l, o)                       \
        { \
        integer _bddhtIt; \
        BDDnode *o, *next; \
        if( ht->NumTables() > l ) { \
          SubHasher* _ht = ht->hashTables[ l ]; \
 \
          for( _bddhtIt = 0; _bddhtIt < _ht->bincount; _bddhtIt++) { \
        o = _ht->bin[_bddhtIt]; \
        for( ; o ; o=next ) { \
          if( o ) next=o->Next();



#define BDDHasherArray_End_ForEach }}}}

////////////////////////////////////////////////////////////////////////////

#endif // BDD_HASHER_ARRAY_H


#ifndef HASH_CACHE_H
#define HASH_CACHE_H

/*
 *    $RCSfile: hashcach.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

 //
 // hashcache.h : simple cache for Comparable Objects 
 //
 // Andrew Seawright
 //

#include "compable.h"

#define HASH_CACHE_DEFAULT_SIZE 211

class HashCache : public Object {

protected:

  integer      size;
  Comparable **keys;
  Object     **cache;
  integer      puts;
  integer      collisions;
  integer      hits;
  integer      total;
       
public:

  HashCache(integer = HASH_CACHE_DEFAULT_SIZE);
 ~HashCache();

  void        Put(Object* o, Comparable* key); 
  Comparable* Replace(Object* o, Comparable* key); // returns old key ptr
  void        Put(Comparable* key)     { Put( key, key);}
  Comparable* Replace(Comparable* key) { return Replace( key, key);}

  void        Clear();
  void          ConditionalClear();    // check virtual function Flush before clearing
  Object*     Get( Comparable* key);
  boolean     Get(Object** o, Comparable* key) { 
    return (*o = Get( key)) != NULL;}

  // Object virtual functions

  virtual void Print ( ostream& out = cout);       // print the stats
  virtual void PrintLong ( ostream& out = cout);       // print the contents
  virtual integer isa() { return HASH_CACHE; }
};

inline ostream& operator << (ostream &out, HashCache& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, HashCache* f)
{ out << "(ptr->)"; f->Print(out); return out; }

////////////////////////////////////////////////////////////////////////////

#endif // HASH_CACHE_H


/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/


/*
 *    $RCSfile: hashcash.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */

#include <iostream.h>
#include "hashcach.h"

// hashcache.C         Andrew Seawright

////////////////////////////////////////////////////////////////////////////

HashCache::HashCache (integer s) {

  ASSERT(s>0, "HashCache : size must be > 0");
  size = s;
  keys  = new Comparable*[size];
  cache = new Object*[size];
  hits = total = collisions = puts = 0;

  integer i;
  for (i=0; i<size; ++i) {
    keys[i] = 0;
  }
}


////////////////////////////////////////////////////////////////////////////

HashCache::~HashCache () {

  delete cache;
  delete keys;
}

////////////////////////////////////////////////////////////////////////////

void
HashCache::Clear() {

  if( puts==0)
    return;

  integer i;

  for (i=0; i<size; ++i) {
    if (keys[i])
      {
      delete keys[i];  // **
      keys[i] = 0;
      }
  }
}

////////////////////////////////////////////////////////////////////////////

void
HashCache::ConditionalClear() {

  if( puts==0)
    return;

  integer i;

  for (i=0; i<size; ++i) {
    if ( keys[i] && (keys[i]->Flush() || cache[i]->Flush()) ) {
      delete keys[i];  // **
      keys[i] = 0;
    }
  }
}
////////////////////////////////////////////////////////////////////////////

void
HashCache::Put(Object *o, Comparable *key) {

  puts++;
  integer bin = key->Hash() % size;
  cache[bin] = o;
  if (keys[bin])
    delete keys[bin]; // warning see *** below
  keys[bin] = key;
}

//
// *** only calls the generic Comparable delete function
//     if this is a problem use function below
//

////////////////////////////////////////////////////////////////////////////

Comparable *
HashCache::Replace(Object *o, Comparable *key) {

  puts++;
  Comparable *old_key;
  integer bin = key->Hash() % size;
  cache[bin] = o;
  if (keys[bin])
    old_key = keys[bin], collisions ++;
  else
    old_key = 0;
  keys[bin] = key;
  return old_key;
}

////////////////////////////////////////////////////////////////////////////

Object*
HashCache::Get( Comparable *key) {

  ++total;
  integer bin = key->Hash() % size;
  if ((keys[bin] != 0) && key->Compare(keys[bin]) == 0) {
    ++hits;
    return cache[bin];
  } else
    return NULL;
}

////////////////////////////////////////////////////////////////////////////

void
HashCache::Print(ostream& out) {

  out << "\nHashCache\n";
  out << "size      =  " << size << "\n";
  out << "hits      =  " << hits << "\n";
  out << "total gets=  " << total << "\n";
  out << "collisions=  " << collisions << endl;
  out << "total puts=  " << puts << endl;
  out << "hit ratio =  " << (double)hits / (double)total << "\n";
}

////////////////////////////////////////////////////////////////////////////

void
HashCache::PrintLong( ostream& out) {
  for( int bin=0 ; bin<size ; bin++ )
    if( keys[bin] )
      {
      out << bin << "\tkey: " << keys[bin];
      out << "\tdata: " << cache[bin];
      }
  }


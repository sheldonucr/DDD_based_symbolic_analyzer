#ifndef INDARRAY_H
#define INDARRAY_H

/*
 *    $RCSfile: indarray.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include "object.h"
#include "list.h"
#include "odlist.h"

class IndexArray: public Object {


private:
  integer  size;
  integer  timestamp;
  integer  count;

  integer  ptr;

  integer* index;
  integer* level;

  void Swap( integer i, integer j) {
    integer t = index[i];
    index[i]  = index[j];
    index[j]  = t;

    t        = level[i];
    level[i] = level[j];
    level[j] = t;
  }

  void Qsort_Descend( integer left, integer right );
  void Qsort_Ascend( integer left, integer right );

protected:
  // functions used by BDDmanager exclusively
  integer operator[] (integer pos)        { return level[pos]; }
  void      StampIt(integer ts)            { timestamp = ts; }
  void    Order( integer *levels, integer ts=-2 );    // this also stamps the time

public:
  // user functions

  IndexArray( integer s)         { size = s; timestamp = -1; count = 0; ptr = 0;
                  index = new integer[ size];
                  level = new integer[ size];
                }
  IndexArray( List* list);
  IndexArray( OList* list);
  IndexArray( ODList* list);
  IndexArray( integer beg, integer end, integer mod, integer off);
 ~IndexArray()            { delete index; delete level;}

  void    Clear()               { timestamp = -1; count = 0; ptr = 0;}

  boolean Find( integer level);
  int     GetHeight( int level );
  integer TimeStamp()         { return timestamp; }
  integer Size ()         { return size; }
  integer Count()         { return count; }
  integer LowestLevel()         { return level[ count-1];}
  integer HighestLevel()        { return level[ 0];}
  integer Level( integer pos)     { ASSERT( pos>=0 && pos<size, "violated array bounds");
                      return level[ pos]; }
  integer Index( integer pos)     { ASSERT( pos>=0 && pos<size, "violated array bounds");
                      return index[ pos]; }
  void Put( integer i )        { ASSERT( count < size, "violated array bounds");
                  index[count++] = i; timestamp = -1; }
  virtual integer isa()        { return IND_ARRAY; }
  virtual void Print( ostream& out = cout);

  friend class BDDmanager;
};

#endif // INDARRAY_H


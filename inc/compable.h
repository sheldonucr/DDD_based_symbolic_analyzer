/*
 *    $RCSfile: compable.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/09/30 20:58:35 $
 */

#ifndef COMPARABLE_H
#define COMPARABLE_H

#include "object.h"

class Comparable : public Object {

public:

  virtual ~Comparable() {};
  virtual boolean isComparable() { return TRUE;} 
  virtual integer isa()          { return COMPARABLE;} 
     
  // Return random yet deterministic int based on the object and parameter i
  virtual unsigned Hash(unsigned i=0)     { return i; }
  // virtual uinteger Hash( unsigned i=0 )	{return (uinteger) i;}
  // If this object is greater than x return a positive value
  // else if equal return zero
  // else return a negative value
  virtual integer Compare(Comparable *) { return 0; }
  integer Compare(Comparable &a) { return Compare(&a); }

  boolean operator< ( Comparable &x) { return Compare(&x)< 0;}
  boolean operator<=( Comparable &x) { return Compare(&x)<=0;}
  boolean operator==( Comparable &x) { return Compare(&x)==0;}
  boolean operator!=( Comparable &x) { return Compare(&x)!=0;}
  boolean operator> ( Comparable &x) { return Compare(&x)> 0;}
  boolean operator>=( Comparable &x) { return Compare(&x)>=0;}
};

#endif

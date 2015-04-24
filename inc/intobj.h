#ifndef INTOBJ_H      
#define INTOBJ_H

/*
 *    $RCSfile: intobj.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#include <iostream.h>
#include "compable.h"

#define INT_PRIME 7919

class IntObj: public Comparable {
public:
  integer val;

  IntObj( void)         { val = 0;}
  IntObj( integer v)    { val = v;}

  integer operator=( integer i) { val = i; return i;}

  // You can use IntObj's as int's
  operator int() { return val; }

  virtual integer isa() { return INT_OBJ;}
  virtual Object* Copy(){ return new IntObj( val);}   

  virtual integer Compare( Comparable* c) {
    ASSERT (c->isa() == INT_OBJ, "expecting IntObj");
    return val - ((IntObj*) c)->val;
  }

  virtual uinteger Hash( uinteger i=0) {
    return (uinteger) ((val+i)^0x5ab);}
  virtual void Print( ostream& out = cout){ out << val;}
  virtual void Scan ( istream&  in = cin ){ in  >> val;}
};

inline ostream& operator << (ostream &out, IntObj& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, IntObj* f)
{ out << "(ptr->)"; f->Print(out); return out; }

#endif // INTOBJ_H


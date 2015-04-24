#ifndef FLOATOBJ_H      
#define FLOATOBJ_H

/*
 *    $RCSfile: floatobj.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#include <iostream.h>
#include "compable.h"

#define FLOAT_PRIME 7919

class FloatObj: public Comparable {
public:
  float val;

  FloatObj( void)    { val = 0;}
  FloatObj( float v) { val = v;}

  float operator=( float i) { val = i; return i;}
  operator float()      { return val;}

  virtual integer isa() { return FLOAT_OBJ;}
  virtual Object* Copy(){ return new FloatObj( val);}   

  virtual integer Compare( Comparable* c) {    // Cannot just return diff (round off errors!
    ASSERT (c->isa() == FLOAT_OBJ, "expecting FloatObj");
    float diff = val - ((FloatObj*) c)->val;
    if ( diff == 0.0) return  0;
    if ( diff <  0.0) return -1;
    return 1;
  }

  //virtual uinteger Hash( unsigned i) {
  //  return (uinteger) (((uinteger)val+i)^0x5ab);}
  virtual unsigned Hash( unsigned i ){     
    return (unsigned) (((unsigned)val+i)^0x5ab);}
  
  virtual void Print( ostream& out = cout){ out << val;}
  virtual void Scan ( istream& in  = cin ){  in >> val;}
};


class DoubleObj: public Comparable {
public:
  double val;

  DoubleObj( void)     { val = 0;}
  DoubleObj( double v) { val = v;}
  virtual ~DoubleObj() {};

  double operator=( double i) { val = i; return i;}
  operator double()     { return val;}

  virtual integer isa() { return DOUBLE_OBJ;}
  virtual Object* Copy(){ return new DoubleObj( val);}   

  virtual integer Compare( Comparable* c) {
    ASSERT (c->isa() == DOUBLE_OBJ, "expecting DoubleObj");
    double diff = val - ((FloatObj*) c)->val;
    if ( diff == 0.0) return  0;
    if ( diff <  0.0) return -1;
    return 1;
  }

  //virtual uinteger Hash( unsigned i) {
  //  return (uinteger) (((uinteger)val+i)^0x5ab);}  
  virtual unsigned Hash( unsigned i ){
    return (unsigned) (((unsigned)val+i)^0x5ab);}

  virtual void Print( ostream& out = cout){ out << val;}
  virtual void Scan ( istream& in  = cin ){  in >> val;}
};

#endif // FLOATOBJ_H


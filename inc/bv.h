#ifndef BITVECT_H
#define BITVECT_H

/*
 *    $RCSfile: bv.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

// 
// This is the BitVector class header file.
//
// For descriptions of individual functions
// see bv.C file.
//

#include <iostream.h>
#include "compable.h"

/////////////////////////////////////////// // machine dependant stuff here //

typedef unsigned int bit_vector_word;
#define BIT_VEC_WORD_SIZE             32
#define BIT_VEC_ALL_ONES              ((bit_vector_word)0xffffffff)
#define BIT_VEC_ZERO                  ((bit_vector_word)0)
#define BIT_VEC_ONE                   ((bit_vector_word)0x00000001)
#define BIT_VEC_TOP_ONE               ((bit_vector_word)0x80000000)

//////////////////////////////////////////////////////////////////////////////

class BitVector : public Comparable {

  integer          num_bits;
  integer          num_words; 
  bit_vector_word *bit_vector;
 
  void PClear(integer n);
 
public:

  BitVector()    { num_bits=num_words=0; bit_vector=0; }
  BitVector(integer n);
  BitVector(const BitVector *x);
  BitVector(const BitVector& x);
 ~BitVector()    { delete bit_vector; }

  boolean GetElement(int n);
  boolean operator[](int n) { return GetElement( n);}
  BitVector operator=(const BitVector& x);
  integer  VectorLength();        
  integer  Size() { return VectorLength(); }
  void Set(integer n);
  void Clear(integer n);
  void ShiftLeft(integer n);
  void ShiftRight(integer n);
  void RotateLeft(integer n);
  void RotateRight(integer n);
  void ClearAll();
  boolean AllClear(); 
  integer NumOnes();
  void PrintWordHex(ostream&, bit_vector_word);
  virtual void Print2(ostream& out = cout);

// These generate temporary storage on the stack
  BitVector operator~();
  BitVector operator |(BitVector& f);
  BitVector operator &(BitVector& f);
  BitVector operator ^(BitVector& f);
  BitVector Expand0 (integer n);
  BitVector Expand1 (integer n);
  
// These operators do not - work via accumulation
  BitVector& operator |=(BitVector& f);
  BitVector& operator &=(BitVector& f);
  BitVector& operator ^=(BitVector& f);

// Comparable virtual functions
  virtual integer Compare(Comparable *);
  virtual unsigned Hash(unsigned n) { return bit_vector[0]^n; }

// Object virtual functions
  virtual void Print(ostream& out = cout);
  virtual integer isa() { return BIT_VECTOR; }

};

inline ostream& operator << (ostream &out, BitVector& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, BitVector* f)
{ out << "(ptr->)"; f->Print(out); return out; }

#endif // BITVECT_H


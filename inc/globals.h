
/* globals.h
 *   Contains external (global) variable declarations that are
 *   referenced in multiple files.  globals.h can be included in
 *   in .c file where these variables are referenced, rather than 
 *   repeating extern declarations separately for each use.  Some 
 *   global variables may be declared in other header files, such as 
 *   tfa_in in parseControl.h, since the variable is closely tied to
 *   a single source file.  
 */

/*
 *    $RCSfile: globals.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/09/29 17:11:51 $
 */

#ifndef GLOBALS_H
#define GLOBALS_H

/* common information needed by most of files */
#include "unixstd.h"
#include "stddefs.h"
#include <assert.h>

/* Circuit Type */
typedef enum {
    cLinear,
    cNonLinear
} CirType;

/* Define analysis modes: */
typedef enum {
    aAC,
    aDC,
    aTRANSIENT,
    aSENS
} AnalysisType;

/* device type definition */

typedef enum {
    dUNKNOWN,// unknown type
    dRES,    // resistor
    dCAP,    // capacitor
    dIND,    // inductor
	dMUIND,  // mutual inductor
	dVCCS,   // voltage controlled current source
    dVCVS,   // voltage controlled voltage source
    dCCVS,   // current controlled voltage source
    dCCCS,   // current controlled current source
    dVOL,    // independent volage
    dCUR,    // independent current
    dDIODE,  // diode
    dBJT,    // bipolar junction transistor
    dJFET,   // junction field effect transistor
    dMOSFET, // mosfet
    dMESFET  // mosfet
} DeviceType;

typedef enum {
    NUM,    // numerator
    DEN    // denominator
} PolyType;

/* Define TRANSIENT model types: */
#define BACK_EULER 0
#define TRAPEZOIDAL 1
#define GEARS 2

/* matrix expansion methods */
typedef enum {
    Closed_LOOP,
    EXPANSION,
    SUBSET
} ExpMethod;

/* type of a branch (R, C, L) */
typedef enum {
    ResPart,
    CapPart,
    IndPart
} BType;

#define ElemPart BType

/* Global vairable declarations */

extern class Circuit *theCkt; // the top (main) ckt  
extern class SubCircuit *theCurCkt; // current ckt being passed

// 32 bit hasher function
struct HashFunc
{
  int operator()(const void *val) const
  {
	  long  key = (long)val;
	  // Robert Jenkins' 32 bit Mix Function
	  // from http://www.cris.com/~Ttwang/tech/inthash.htm
	  key += (key << 12);
	  key ^= (key >> 22);
	  key += (key << 4);
	  key ^= (key >> 9);
	  key += (key << 10);
	  key ^= (key >> 2);
	  key += (key << 7);
	  key ^= (key >> 12);

	  return key;
  }	
};

/* 64 bit hash function
struct HashFunc
{
  long operator()(void *val)
  {
	  key += ~(key << 32);
	  key ^= (key >>> 22);
	  key += ~(key << 13);
	  key ^= (key >>> 8);
	  key += (key << 3);
	  key ^= (key >>> 15);
	  key += ~(key << 27);
	  key ^= (key >>> 31);
	  return key;
  }	
};
*/

#include "message.h"
#include "utility.h"

#endif /* ifndef  GLOBALS_H */

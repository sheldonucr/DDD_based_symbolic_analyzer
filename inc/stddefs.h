#ifndef STDDEFS_H
#define STDDEFS_H

/*
 *    $RCSfile: stddefs.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/11/21 02:42:38 $
 */

/*
 *
 * stddefs.h : standard header file
 * This file should be compatable with c and c++
 *
 */


/* The folowing is for portability between IBM PC's and SUN.
   'integer' is a 32 bit number. */

#define BITS_PER_INTEGER 32
//typedef int                integer;  /* subst long for PC's */   
typedef long	integer;
//typedef unsigned int       uinteger;  /* subst unsigned long for PC's */   
typedef unsigned long	uinteger;
typedef unsigned int       boolean;  
typedef char*              cstring; 
typedef unsigned long       ulong;

#define UCSB            1

#ifndef TRUE
#define TRUE            ((boolean)1)
#endif
#ifndef FALSE
#define FALSE           ((boolean)0)
#endif
#define YES             1
#define NO              0
#define RAD_CODE        YES

#define NIL(X)   ((X)0)
#ifndef MAX
#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#endif
#ifndef MIN
#define MIN(X,Y) (((X)>(Y))?(Y):(X))
#endif
#ifndef ABS
#define ABS(X) (((X)>0)?(X):(X)*-1)
#endif
#ifndef SWAP
#define SWAP(T,X,Y) {T S; S=X; X=Y; Y=S;}
#endif

//////////////////////////////////////////////////////////////
// c++ specific macros

#ifdef DEBUG
#define FATAL(x) { cerr << "fatal error in file "; \
           cerr << __FILE__ << " at line "; \
           cerr << __LINE__ << ", " << x << "\n"; \
           *((int*)0) = 42; \
           exit(-1); }
#else
#define FATAL(x) { cerr << "fatal error in file "; \
           cerr << __FILE__ << " at line "; \
           cerr << __LINE__ << ", " << x << "\n"; \
           exit(-1); }
#endif

#define ASSERT(exp, y) if (!(exp)) FATAL(y);

//////////////////////////////////////////////////////////////

#include <time.h>


//extern void srandom(int seed);
//extern long random(void);

#define SRANDOM() {  time_t seed = 0; \
             srand48( (long) time( &seed)); \
           }


/////////////////////////////////////////////////////////////
// STL library

//#define __STL_NO_BOOL
//#include "vector.h"
//#include "map.h"

#endif

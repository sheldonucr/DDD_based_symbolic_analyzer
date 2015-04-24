#ifndef MEM_H
#define MEM_H

/*
 *    $RCSfile: mem.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

///////////////////////////////////////////////////////////////////////////////
//
//  mem.h : C++ Memory Allocation / Management object
//
///////////////////////////////////////////////////////////////////////////////


#include <iostream.h>
#include <stdlib.h>
#include "stddefs.h"

#define CACHE_BIGGEST    120          // size of biggest object that is cached
#define CACHE_THRESHOLD  3            // # hits before starting cacheing
#define PAGESIZE         4096         // number of objects in fresh new page
#define MEM_WORD_SIZE    4            // number of bytes in a word
#define FREE_TAG         0xdead       // header tag for freed objects (not used)


///////////////////////////////////////////////////////////////////////////////
//
//  How to use:
//
//  o In a base class overload the new() and delete() operators:
//
//    void * operator new(size_t n)
//      { return mem_default_group.Malloc(n); }
//    void operator delete (void *p, size_t n)
//        { mem_default_group.Free(p, n); }
//       
//      This has already been done to Object if MEM is #defined, see object.h
//    
//  o If you are only using the memory class from HomeBrew, you must construct 
//       one (or several) MemGroup instance.  Otherwise skip this step:
//
//       extern MemGroup *mem_default_group;  // typically a global variable 
//             
//       mem_default_group = new MemGroup;
//
//  o Allocate new objects as usual:
//     
//       Foo *f;
//       f = new foo;
//
//
///////////////////////////////////////////////////////////////////////////////

#ifdef MEM_STATS
extern integer _global_num_alloc;
extern integer _global_num_inuse;
extern integer _global_num_freed;
#endif


class MemGroup {  

  integer    num_malloced;  // total num of bytes from system malloc
  integer    num_inuse;     // total num of bytes in active use
  integer    num_free;      // total num of recycled available mem
  integer    num_big;       // total num of big chunks
  integer    malloc_count;  // number of times system malloc called
  integer    alloc_count;   // number of times MemGroup::Malloc() called
  integer    alloc_watch;   // 

  char     **free;        // free list
  integer   *hits;        // 
  integer   *allocated;   //
  integer   *free_count;  //

public:
  
  MemGroup();
 ~MemGroup();
  void * Malloc(size_t);
  void Free(void *, size_t);
  void PrintStats( ostream& out = cout);
  void RecoverMem();

#ifndef MEM_STATS
  void * operator new(size_t n)       // make sure this object is allocted
      { return ::malloc(n); }     // with system malloc
  void operator delete (void *p)
         { ::free((char*) p); }
#endif
};

///////////////////////////////////////////////
// Extra functions which uses pstat values

// look in memfunts.C for the source

integer AvailSwap();
integer AllocSwap();

#ifdef MEM_STATS
void *operator new(size_t n);
void operator delete (void* p );
#endif


#endif

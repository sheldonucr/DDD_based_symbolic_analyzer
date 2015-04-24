#ifndef CLASSDEF_H
#define CLASSDEF_H

/*
 *    $RCSfile: classdef.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


////////////////////////////////////////////////////////////////////////////
// 
// classdef.h : class definitions                         Andrew Seawright
//
// **WARNING** all associated values must be below 1000
//
// for generic class library
//
////////////////////////////////////////////////////////////////////////////

#define OBJECT         0
#define COMPARABLE     1
#define STICKY         2
#define REFCOMP           3

#define LIST           5
#define LINK           6
#define DOUBLELINK     7
#define OLIST          8
#define ODLIST         9

 
#define DG_NODE        10
#define DG_EDGE        11
#define DG_SOURCE      13
#define DG_SINK        14
#define DG_SOURCE_SINK 15 
#define DG             16
#define FA_NODE        17
#define FA_EDGE        18
#define FA             19

#define HASH_TREE      20

#define BDDHASHERARRAY 22
#define BDDHASHTREE    25

#define HASHER_TREE    30

#define BIT_SPARSE     40
#define INT_SPARSE     41

#define LEX            50

#define BDDNODE        60
#define BDDMANAGER     61
#define BDDCOMPUTED    62
#define BDDFUNCTION    63
#define BDD_CACHE      64
#define BDD_CACHE_KEY  65
#define LOBDDFUNC      66

#define STRING_OBJ     70

#define INT_OBJ        80
#define UNSIGN_OBJ     81
#define LONG_OBJ       82
#define ULONG_OBJ      83
#define FLOAT_OBJ      84
#define INT_OBJ2       85
#define DOUBLE_OBJ     86

#define RB_TREE        90
#define RB_NODE        91
#define HEAP           99 

#define BIT_VECTOR    100

#define BIT_STACK     110

#define HASH_CACHE    120

#define ARRAY         130
#define IND_ARRAY      135

#define BUBBLE_OBJ    140
#define BUBBLEMGR     141

#define HGRAPH        150
#define HGRAPH_NODE   151
#define HGRAPH_EDGE   152

#define COFACTORHASHKEY 160

#define DDDCOMPUTED1 423
#define DDDCOMPUTED2 424
#define DDDNODE      425
#define DDDCOMPUTED3 426

#define SPECIAL_CLASS    666

#endif

#ifndef _APPROXIMATE_H
#define _APPROXIMATE_H
 
/*
 *    $RCSfile: approx.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/09/30 20:55:21 $
 */

 /*
 ////////////////////////////////////////////////////
 //
 //  aproxiate.h : 
 //     head file for DDD approximation 
 //     ddd - determinant decision diagram
 //
 //  X. Sheldon Tan
 //  (c) Copyright the University of Iowa, 1997
 //
 //
 ///////////////////////////////////////////////////
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <stddef.h>
#include <fstream.h>
#include "dtstruct.h"

extern void AfterApproximation(DDDmanager *, DDDnode *, double , double );

#define dWHITE (bool)0
#define dBLACK (bool)1

#define INF_WEIGHT 1e60
extern DDDstack *theTopoList;
extern DDDstack *theCurPath;

extern int UseSimpExp; // use the simplified expression 
extern int EnBranchElim; // enable branch elimination mechnasim

#endif //_APPROXIMATE_H


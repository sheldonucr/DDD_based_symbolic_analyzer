/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: yainit.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/20 04:22:01 $
 */


/* INCLUDES: */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

// Local
#include "globals.h"
#include "branch.h"
#include "circuits.h"
#include "timer.h"

extern Timer *theTimer;

/* DEFINE's and TYPEDEF's */

/* GLOBAL VARIABLES */

class Circuit *theCkt; 
class SubCircuit *theCurCkt; 

extern void add_signal_handler();
//static void help(int argc, char *argv[]);
static void check_cmd_line(int argc,char *argv[]); 


void
init (int argc, char *argv[])
{
  theCkt = NULL;
  theCurCkt = NULL;
  theTimer = new Timer;

  check_cmd_line(argc,argv); /* Check command line for errors    */
  //help(argc, argv);

}

/*----------------------------------------------------
    FuncName: check_cmd_line
    Spec:checks the command line for errors.
 -----------------------------------------------------
    Inputs: argc, **argv
    returns: circuit name without "." suffix if
             it is available.

    Global Variblas used:
    FILE*     fp;
----------------------------------------------------*/
static void
check_cmd_line(int argc, char **argv)
{
	//char buf[256];
	//int c;

  /* extern int optind;*/            /* from getopt, points to first
                                          non-option argument */
  extern int opterr;            /* from getopt, option error flag */
 
  /* using getopt to handle command line args.
      - should check if getopt is very portable
      */
  opterr = 0;  /* disable getopt's error messages */
 
  /*
  
  while((c = (char)getopt(argc,argv, "ct:ad")) != EOF ) {
    switch (c) {
    case 'e':
      break;
    case 'b': 
      break;
    case 'a': 
      break;
        
    case '?':
    case 'h':
      help(argc, argv);
      break;
    default:
      break;
    }
  }
  */
  
  

}

/*----------------------------------------------------
Spec: print out Usage information
-----------------------------------------------------*/

/*
static void
help(int argc, char *argv[])
{
	argc = 1;
    fprintf(stderr,"Usage: %s {spice_file}",argv[0]);

	
}
*/

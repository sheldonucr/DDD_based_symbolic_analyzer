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
 *    $RCSfile: scad3.cc,v $
 *    $Revision: 1.9 $
 *    $Date: 2002/09/29 17:04:56 $
 */

/* main.cc -
 */

#include <stdio.h>
#include "yasis.h"

char *inputSpFile;

float version_index = 1.8;

extern int CmdDo(char*);
extern char *RunStats();
extern void Command(FILE *_iobuf);
extern void header();
extern void init(int argc, char **argv);

////////////////////////////////////////////////////
//
//  Main function for scad3 system.
//
////////////////////////////////////////////////////
int main
(
	int argc, 
	char *argv[]
)
{
    char cmd[128];

    /* There should be no more than one command line switch, and
     * it contains the name of the .sp file.
     */

    init(argc,argv);

    if (argc > 2)
	{
		printf("Too many arguments;  all but the first are ignored.\n");
	}

    if (argc > 1) 
	{
		inputSpFile = argv[1];
	}
    else
	{
		inputSpFile = NULL;
	}

	// print the head files
    ::header();

    if (inputSpFile != NULL)
	{
		printf(": source %s\n", inputSpFile);
		sprintf(cmd, "source %s", inputSpFile);
		CmdDo(cmd);
	}
    else
	{
		printf("No .sp file specified.\n");
	}

	// a simple command shell
    ::Command(stdin);

    printf("%s SCAD3 done.\n", ::RunStats());

	return true;
}


//////////////////////////////////////////////
//
//  Banner for scad3 system
//
/////////////////////////////////////////////
void
header()
{
    printf("Welcome to [1m SCAD3[m %s ---\n \
\t[1mSymbolic Circuit Analysis with Determinant Decision Diagrams[m \n",
		   "$Revision: 1.9 $" );
    printf("\tCopyright (c) 1998 - 2002, Sheldon X.-D. Tan, stan@ee.ucr.edu \n");
    printf("\t'h' for help\n");
}

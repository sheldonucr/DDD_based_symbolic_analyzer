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
 *    $RCSfile: nextfield.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


/* nextfield.c -
 *
 * This file contains a single routine used for parsing strings
 * containing multiple arguments.
 */

#include <stdio.h>
#include <ctype.h>

char *
NextField(char **p)
//char **p;            /* A pointer to a string pointer.*/

/*---------------------------------------------------------
 *    This procedure picks arguments off of strings.
 *    The string pointed to indirectly by p is assumed
 *    to consist of zero or more fields separated by
 *    whitespace.
 *
 *    Results:
 *    The return value is a pointer to the first character
 *    of the first field of the string.  NULL is returned
 *    if there are no fields left in the string.
 *
 *    Side Effects:
 *    The pointer p is updated to refer to the next field after
 *    one returned.  The string is modified by replacing the
 *    first white space character after the first field with
 *    a zero character.
 *---------------------------------------------------------
 */

{
    register char *p1;
    char *p2;

    p1 = *p;

    /* Strip off leading white space. */

    while (isspace(*p1)) p1 += 1;
    p2 = p1;

    while ((*p1 != 0) && !isspace(*p1)) p1 += 1;
    if (*p1 != 0) *p1++ = 0;
    *p = p1;
    if (*p2 == 0) return NULL;
    else return p2;
}

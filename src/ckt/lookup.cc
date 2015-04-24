/*
*******************************************************


   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: lookup.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


/* lookup.c -
 *
 * This file contains a single routine used to look up a string in
 * a table, allowing unique abbreviations.
 */

#include <stdio.h>
#include <ctype.h>


int
Lookup(char str[], char *(table[]))
//* which are the valid commands.  The strings
//* must be ordered monotonically (i.e. all
//* strings whose first characters are identical
//* must be adjacent in the table).  Each string
//* consists of a command name terminated either
//* by white space or the end of the string.
//*/

/*---------------------------------------------------------
 *    Lookup searches a table of strings to find one that matches a
 *    given string.
 *
 *    Results:
 *    If str is an unambiguous abbreviation for one of the entries
 *    in table, then the index of the matching entry is returned.
 *    If str is an abbreviation for more than one entry in table,
 *    then -1 is returned.  If str doesn't match any entry, then
 *    -2 is returned.
 *
 *    Side Effects:    None.
 *---------------------------------------------------------
 */

{
    /* The search is carried out by using two pointers, one which moves
     * forward through table from its start, and one which moves backward
     * through table from its end.  The two pointers mark the range of
     * strings that match the portion of str that we have scanned.  When
     * all of the characters of str have been scanned, then the two
     * pointers better be identical.
     */
    char **bot, **top;
    int match, index;
    match = 0;
    bot = table;
    for (top = table; *top != NULL; top++);
    if (top == bot) return(-2);
    top--;

    for (index=0; ; index++)
    {
    /* Check for the end of string */

    if (str[index] == '\0')
    {
        if (bot == top) return(match);
        else return(-1);
    }

    /* Move bot up until the string it points to matches str in the
     * index'th position.  Make match refer to the index of bot in table.
     */
    while ((*bot)[index] != str[index])
    {
        if (isspace((*bot)[index]) && (str[index] == 0)) break;
        if (bot == top) return(-2);
        bot++;
        match++;
    }

    /* Move top down until it matches */
    while ((*top)[index] != str[index])
    {
        if (isspace((*top)[index]) && (str[index] == 0)) break;
        if (bot == top) return(-2);
        top--;
    }
    }
}

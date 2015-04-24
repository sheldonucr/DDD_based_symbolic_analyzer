
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
 *    $RCSfile: signals.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


/* the procedures for UNIX signal handling in SCAD3 */

#include <signal.h>
#include <mxexp.h>

void
inter_handler(int i)
{
    signal(SIGQUIT, inter_handler);
    signal(SIGINT, inter_handler);
    cout <<"Current Max_ddd_size :" << Max_ddd_size << "\n";
    cout <<"Input new Max_ddd_size: ";
    cout.flush();
    scanf("%d",&Max_ddd_size);
}

void
add_signal_handler(void)
{
    signal(SIGQUIT, inter_handler);
    signal(SIGINT, inter_handler);
}

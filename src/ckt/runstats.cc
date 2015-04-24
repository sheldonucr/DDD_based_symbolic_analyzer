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
 *    $RCSfile: runstats.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2001/10/15 06:00:48 $
 */


/* runstats.c -
 */

#include <sys/types.h>
#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>

//extern int sbrk(int);

/* Library imports: */

extern int end;

/* The following variables keep track of the time used as of
 * the last call to one of the procedures in this module.
 */

int lastSysTime = 0;
int lastUserTime = 0;


char *
RunStats()

/*---------------------------------------------------------
 *    This procedure collects information about the
 *    process.
 *
 *    Results:
 *    The return value is a string of the form
 *    "[mins:secs mins:secs size]" where the first time is
 *    the amount of user-space CPU time this process has
 *    used so far, the second time is the amount of system
 *    time used so far, and size is the size of the heap area.
 *
 *    Side Effects:    None.
 *---------------------------------------------------------
 */

{
    struct tms buffer;
    static char string[100];
    int umins, size, smins;
    float usecs, ssecs;

    times(&buffer);
    lastUserTime = buffer.tms_utime;
    lastSysTime = buffer.tms_stime;
    umins = buffer.tms_utime;
    usecs = umins % 3600;
    usecs /= 60;
    umins /= 3600;
    smins = buffer.tms_stime;
    ssecs = smins % 3600;
    ssecs /= 60;
    smins /= 3600;
    size = (((long) sbrk(0) - (long) &end) + 512)/1024;
    sprintf(string, "[%d:%04.1fu %d:%04.1fs %dk]",
			umins, usecs, smins, ssecs, size);
    return string;
}


char *
RunStatsSince()

/*---------------------------------------------------------
 *    This procedure returns information about what's
 *    happened since the last call.
 *
 *    Results:
 *    Just the same as for RunStats, except that the times
 *    refer to the time used since the last call to this
 *    procedure or RunStats.
 *
 *    Side Effects:    None.
 *---------------------------------------------------------
 */

{
    struct tms buffer;
    static char string[100];
    int umins, size, smins;
    float usecs, ssecs;

    times(&buffer);
    umins = buffer.tms_utime - lastUserTime;
    smins = buffer.tms_stime - lastSysTime;
    lastUserTime = buffer.tms_utime;
    lastSysTime = buffer.tms_stime;
    usecs = umins % 3600;
    usecs /= 60;
    umins /= 3600;
    ssecs = smins % 3600;
    ssecs /= 60;
    smins /= 3600;
    size = (((long) sbrk(0) - (long) &end) + 512)/1024;
    sprintf(string, "[%d:%04.1fu %d:%04.1fs %dk]",
			umins, usecs, smins, ssecs, size);
    return string;
}

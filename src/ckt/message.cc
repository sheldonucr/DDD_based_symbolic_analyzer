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
 *    $RCSfile: message.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/09/29 17:04:56 $
 */

#include "unixstd.h"
#include "message.h"

char _buf[1024];
//char buf[256];

void error_mesg(int code, char *mesg);

void
error_mesg(int code, char *mesg)
{
    if(code == FAT_ERROR)
        cout <<"FATAL ERROR: "<<mesg<<endl;
    else if(code == INT_ERROR)
        cout <<"INTERNAL ERROR: "<<mesg<<endl;
    else if(code == IO_ERROR)
        cout <<"I/O FILE ERROR: "<<mesg<<endl;
    else if(code == PARSE_ERROR ||
            code == SYN_ERROR )
        cout <<"SYNTEX ERROR: "<<mesg<<endl;
    cout <<"";
    cout.flush();
}

void
print_mesg(char *mesg)
{
    cout <<mesg<<endl;
    cout.flush();
}

void
print_warn(char *mesg)
{
    cout <<"WARNING: ";
    cout <<mesg<<endl;
    cout.flush();
}

void
print_raw(char *mesg)
{
    cout <<mesg;
    cout.flush();
}

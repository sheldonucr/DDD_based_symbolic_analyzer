
#ifndef MESSAGE_H
#define MESSAGE_H

/*
 *    $RCSfile: message.h,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/09/30 20:55:21 $
 */


#include <stdio.h>
#include <iostream.h>

#define FAT_ERROR    0        // fatal error
#define INT_ERROR    1        // internal error
#define IO_ERROR    2        // I/O file error
#define PARSE_ERROR 3        // parse error
#define SYN_ERROR     4        // syntex error


/* function interface delaratioins */
extern void print_error(int code, char *mesg);
extern void error_mesg(int code, char *mesg); 
extern void print_mesg(char *mesg); 
extern void print_warn(char *mesg); 
extern void print_raw(char *mesg);


#define print_error(code, mesg) \
{ \
    cout <<"IN FILE: "<<__FILE__<<"  LINE: "<<__LINE__<<endl;\
    error_mesg(code,mesg); \
}

extern char _buf[1024]; // used for buffering output message

#endif //MESSAGE_H


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
 *    $RCSfile: control.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/20 04:22:01 $
 */


#include "control.h"

/* declaration of some global parameter */

double  opMagErr    = DF_opNMegError;
double  opPhsErr    = DF_opNPhsError;
double  opRelThd    = DF_opNRelThreshold;
double  opFCRelThd  = DF_opNFCRelThreshold;
double  opFCNRelThd  = DF_opNFCRelThreshold;
double  opFCDRelThd  = DF_opNFCRelThreshold;
double  opAlpha        = DF_opNAlpha;

ControlMagr    *control_mgr;

///////////////////////////////////////////////////
// member function definition of OptCommand
///////////////////////////////////////////////////
OptCommand::OptCommand()
{
    group = 0; 
    arg_list = NULL; 
    num_arg = 0; 
    next = NULL;
}

OptCommand::~OptCommand()
{
    Args *list = arg_list;
    Args *list1;

    while(list){
        list1 = list->next;
        delete [] list->arg;
        delete list;
        list = list1;
    }
    if(next)
        delete next;
}

/* 
** arguments are added with the order of
** their adding
*/
void
OptCommand::add_arg(char *str)
{
    Args *arg_aux = arg_list;

    if(!str)
        return;

    if(!arg_aux){
        arg_list = new Args;
        arg_list->arg = CopyStr(str);
        arg_list->next = NULL;
    }
    else{
        while(arg_aux->next) arg_aux = arg_aux->next;
        arg_aux->next = new Args;
        arg_aux = arg_aux->next;
        arg_aux->arg = CopyStr(str);
        arg_aux->next = NULL;
    }
    num_arg++;
}

///////////////////////////////////////////////////
// member function definition of Set
///////////////////////////////////////////////////
Set::Set() : OptCommand()
{ ; }

void
Set::analyze_command(char *comm)
{
    char    *str, *comm1,*comm2;
    char    *org = comm;

    if(!comm || !theCkt)
        return;

    // change all letter into lower case
    comm = ToLower(comm);

    // get rid of all the "=" if it is presert
    comm2 = comm1 = CopyStr(comm);
    if(strchr(comm,'=')){
        char *p = comm1;
        while(*p) {
            if(*p == '=') 
                *p = ' ';
            p++;
        }
    }

    while((str = NextField(&comm1))){
        if(!strcmp(str,opNMegError)){
            str = NextField(&comm1);
            if(!str){
                sprintf(_buf,"Syntex error -> %s <-",org);
                error_mesg(PARSE_ERROR,_buf);
                delete [] comm2;
                return;
            }
            opMagErr = TransValue(str);
            continue;
        }
        else if(!strcmp(str,opNPhsError)){
            str = NextField(&comm1);
            if(!str){
                sprintf(_buf,"Syntex error -> %s <-",org);
                error_mesg(PARSE_ERROR,_buf);
                delete [] comm2;
                return ;
            }
            opPhsErr = TransValue(str);
            continue;
        }
        else if(!strcmp(str,opNRelThreshold)){
            str = NextField(&comm1);
            if(!str){
                sprintf(_buf,"Syntex error -> %s <-",org);
                error_mesg(PARSE_ERROR,_buf);
                delete [] comm2;
                return ;
            }
            opRelThd = TransValue(str);
            continue;
        }
        else if(!strcmp(str,opNFCNRelThreshold)){
            str = NextField(&comm1);
            if(!str){
                sprintf(_buf,"Syntex error -> %s <-",org);
                error_mesg(PARSE_ERROR,_buf);
                delete [] comm2;
                return ;
            }
            opFCNRelThd = TransValue(str);
            continue;
        }
        else if(!strcmp(str,opNFCDRelThreshold)){
            str = NextField(&comm1);
            if(!str){
                sprintf(_buf,"Syntex error -> %s <-",org);
                error_mesg(PARSE_ERROR,_buf);
                delete [] comm2;
                return ;
            }
            opFCDRelThd = TransValue(str);
            continue;
        }
        else if(!strcmp(str,opNAlpha)){
            str = NextField(&comm1);
            if(!str){
                sprintf(_buf,"Syntex error -> %s <-",org);
                error_mesg(PARSE_ERROR,_buf);
                delete [] comm2;
                return ;
            }
            opAlpha = TransValue(str);
            continue;
        }
        else{
                sprintf(_buf,"Unkown parameter -> %s <-",org);
                error_mesg(PARSE_ERROR,_buf);
                str = NextField(&comm1);
                delete [] comm2;
                continue;
        }
    }
    delete [] comm2;
}

            
///////////////////////////////////////////////////
// member function definition of ControlMagr command
///////////////////////////////////////////////////
ControlMagr::ControlMagr()
{
    comm_list = NULL;
}

ControlMagr::~ControlMagr()
{
    if(comm_list)
        delete comm_list;
}

/*
**    Add a command into the command list with
**    the order of their reading
**  
*/

void
ControlMagr::add_command(OptCommand *comm)
{
    OptCommand *com_aux = comm_list;

    if(!com_aux)
        comm_list = comm;
    else{
        while(com_aux->next) com_aux = com_aux->next;
        com_aux->next = comm;
    }
}


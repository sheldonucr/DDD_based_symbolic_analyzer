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
 *    $RCSfile: parse_ctrl.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


#include "globals.h"
#include "circuits.h"
#include "acan.h"

/* Control class member function definitions */

/*----------------------------------------------------
Spec: 
-----------------------------------------------------*/
void
Circuit::parse_ac_cmd( char *_var, char *_np, 
                    char *_fstart, char *_fstop)
{ 
    ACAnalysis *ac_aux;
    char    *str = ToLower(CopyStr(_var));
    double    np = TransValue(_np);    
    double    fstart = TransValue(_fstart);    
    double    fstop = TransValue(_fstop);    

    if(ac_mgr->get_command()){
        print_warn("More than two ac analysis commands exist, ignored");
        return;
    }
    if(!strcmp(str,"dec"))
        ac_aux = new ACAnalysis(vDEC, (int)np,fstart,fstop);
    else if(!strcmp(str,"lin"))
        ac_aux = new ACAnalysis(vLIN, (int)np,fstart,fstop);
    else 
        ac_aux = new ACAnalysis(vOCT, (int)np,fstart,fstop);
    
    ac_mgr->add_command(ac_aux);
    delete [] str;

}

void
Circuit::parse_print(char *keyword, char *_n1, char *_n2)
{
    IOList *iolist = NULL;
    char *str = ToLower(CopyStr(keyword));
    int    n1 = atoi(_n1);
    int n2 = (_n2 != NULL ? atoi(_n2):0);
    if(strlen(str) == 1 || str[1] == 'r'){
        iolist = new IOList(n1, n2, dVOL, 0.0, oREAL);
        ACAnalysisManager::add_output(iolist);
    }
    else if (strlen(str) > 1 && str[1] == 'i'){
        iolist = new IOList(n1, n2, dVOL, 0.0, oIMAG);
        ACAnalysisManager::add_output(iolist);
    }
    else if(strlen(str) > 1 && str[1] == 'm'){
        iolist = new IOList(n1, n2, dVOL, 0.0, oMEG);
        ACAnalysisManager::add_output(iolist);
    }
    else if(strlen(str) > 1 && str[1] == 'p'){
        iolist = new IOList(n1, n2, dVOL, 0.0, oPHASE);
        ACAnalysisManager::add_output(iolist);
    }
    else if(strlen(str) > 1 && str[1] == 'd'){
        iolist = new IOList(n1, n2, dVOL, 0.0, oDB);
        ACAnalysisManager::add_output(iolist);
    }
    delete [] str;
}



#ifndef CONTROL_H
#define CONTROL_H

/*
 *    $RCSfile: control.h,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/20 04:32:31 $
 */

/* control.h:
 *   Contains parsing class related to options and 
 *   contol statements from optimization control file
 */

#include "globals.h"

/* parameters acceptable to gopsim */
#define opNMegError         "magerr"    // relative magnitude error
#define opNPhsError         "phserr"    // relative phase error
#define opNRelThreshold     "relth"        // relative threshold
#define opNFCNRelThreshold  "nrelth"    // relth for numerator coefficient list
#define opNFCDRelThreshold  "drelth"    // relth for denominator coefficient list
#define opNAlpha            "alpha"        // relth for controlling 
                                        //coefficient  elimination.

/* default value for this parameters */
#define DF_opNMegError            0.1
#define DF_opNPhsError            0.1
#define DF_opNRelThreshold        0.9
#define DF_opNFCRelThreshold      0.00 // by default, we do not do any appproximation
#define DF_opNAlpha               0.01

extern double    opMagErr;
extern double    opPhsErr;
extern double    opRelThd;
extern double    opFCNRelThd;
extern double    opFCDRelThd;
extern double    opAlpha;

/*************************************/

/* the analysis command following the first argument */
typedef struct _args 
{
    char    *arg;
    struct    _args *next;
} Args;

class OptCommand 
{ // optimizing commands

     protected:

    int    group;         // the group the command belongs
    Args    *arg_list;     // the argument list of the commands
    int    num_arg;    // the number of arguments
    OptCommand    *next;

    friend class ControlMagr;

      public:
    OptCommand();
    virtual ~OptCommand();
    void add_arg(char *str);
    OptCommand    * Next() { return next;}

    virtual void analyze_command( char *str ){;}
    virtual void print_command(FILE *fp){;}
    virtual void print_command(ostream & fp){;}
    virtual int check_command(){ return 0;}
};


/* Sweep command defintion */

class Set : public OptCommand {
    private:
    
    public:
    Set();
    ~Set(){;}
    void analyze_command( char *str );
    int check_command(){return 0;}
};

class ControlMagr {

  protected:
    OptCommand    *comm_list;

  public:
    ControlMagr();
    ~ControlMagr();

    void  add_command(OptCommand *comm);
};

#endif /* ifndef CONTROL_H */

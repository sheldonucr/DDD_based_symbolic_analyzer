
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
 *    $RCSfile: utility.cc,v $
 *    $Revision: 1.4 $
 *    $Date: 2002/09/29 17:04:56 $
 */


#include "globals.h"
#include "utility.h"
#include <math.h>
#include <time.h>
#include <sys/types.h>

void InitRandom();

/* some convenient functions definition */

/*----------------------------------------------------
    FuncName: CopyStr(char *str)
    Spec: alloc space and copy the content
 -----------------------------------------------------
    Inputs: char *str
    Outputs: the address of alloced space

    Global Variblas used: none
----------------------------------------------------*/

#include <string.h>
#include <ctype.h>

char *
CopyStr(const char *str)
{
    if(!str)
        return NULL;

    int length = strlen(str);
    char *aux = new char[length+1];
    aux = strncpy(aux, str,length);
    aux[length] = '\0';
    return aux;
}

/*----------------------------------------------------
    FuncName:ToUpper
    Spec: convert all the charaters into capital case
 -----------------------------------------------------
    Inputs: char *str
    Outputs: the converted string

    Global Variblas used: none
----------------------------------------------------*/
char *
ToUpper(char *str)
{
    if(!str)
        return NULL;
    int length = strlen(str);
    for(int i = 0; i<length; i++)
        str[i] = toupper(str[i]);
    return str;
}    
    
        
/*----------------------------------------------------
    FuncName: ToLower
    Spec: convert all the charaters into lower case
 -----------------------------------------------------
    Inputs: char *str
    Outputs: the converted string

    Global Variblas used: none
----------------------------------------------------*/

char *
ToLower(char *str)
{
    if(!str)
        return NULL;
    int length = strlen(str);
    for(int i = 0; i<length; i++)
        str[i] = tolower(str[i]);
    return str;
}

/*----------------------------------------------------
    FuncName: TransValue
    Spec:
    function that gets the floating point value from a string
    that represents the value. If value has a value like 3547K
    this function returns the value 3547 by putting 1000 in the
    mulfac variable.It decodes the K,M,G,U,...
 -----------------------------------------------------
    Inputs: char *str with SPICE stardard unit suffixes
    Outputs: double type value

    Global Variblas used: none
----------------------------------------------------*/

/* MEMO unit used in SPICE
T=1e12 G=1e9 MEG=1e6 K=1e1 MIL=25.4e-6 M=1e-3 U=1e-6
N=1e-9 P=1e-12 F=1e-15
*/

double
TransValue(const char *_value)
{
    if(!_value){
        error_mesg(INT_ERROR,"NULL pointer is passed for TransValue");
        return 0.0;
    }

    ASSERT(strlen(_value) > 0,"Value number");

    char *value = CopyStr(_value);
    char ch = value[0];
    int i = 0;
    double val;
    double mulfac = 1.0;
    //cout <<"-->input "<<value;
    // skip the digits.
        while ( ch >= '0' && ch <= '9'|| 
            ch == '.' || ch == '+' || ch == '-') {
        ch = value[++i];
    }
 
    // decode the letter symbols
        if (ch != ' ') {
        switch (ch) {
 
        case 'T' :
        case 't' :
            mulfac = 1e12;
            break;
 
        case 'G' :
        case 'g' :
            mulfac = 1e9;
            break;
 
        case 'K' :
        case 'k' :
            mulfac = 1e3;
            break;
 
        case 'U' :
        case 'u' :
            mulfac = 1e-6;
            break;
 
        case 'N' :
        case 'n' :
            mulfac = 1e-9;
            break;
 
        case 'P' :
        case 'p' :
            mulfac = 1e-12;
        break;
 
        case 'F' :
        case 'f' :
            mulfac = 1e-15;
            break;
 
        case 'M' :
        case 'm' :
            switch (value[i+1]){
            case  'E' :
            case 'e' :
                mulfac = 1e6;
                break;
            case  'I' :
            case 'i' :
                mulfac = 25.4e-6;
                break;
            default :
                mulfac = 1e-3;
                break;
            }
            break;
        case 'E' :
        case 'e' :
            double var1;
            var1 = strtod(&value[i+1],NULL);
            mulfac = pow(10,var1);
            break;
        }
    }
    value[i] = 0;
    sscanf( value, "%lg", &val);
    //cout<<"  output(base)->"<<val <<endl;
    //cout<<"  output->"<<val * mulfac<<endl;
    delete [] value;
    return (val * mulfac);
}
 
/*----------------------------------------------------
    FuncName: 
    Spec: routines related to random generator
 -----------------------------------------------------
    Inputs:
    Outputs:

    Global Variblas used:
----------------------------------------------------*/
void
InitRandom()
{
   unsigned seed;
   time_t tloc;
   time_t t;
 
   t = time(&tloc);
   seed = t%1000;
   srand(seed);
}

static int rand_initialized = 0;
double 
RandVar(double lbound, double ubound)
{
    double value;

    if(!rand_initialized){
        InitRandom();
        rand_initialized = 1;
    }

    value = exp(log(lbound) + (float) (rand()%1000)/1000.0 *
            (log(ubound) - log(lbound)));

    return value;
}

/*----------------------------------------------------
    FuncName: WhichType
    Spec:Findout the DeviceType from device keyword
 -----------------------------------------------------
    Inputs: char *keyword
    Outputs: DeviceType
    if keyword == NULL, return UNKNOWN

    Global Variblas used: node
----------------------------------------------------*/

DeviceType
WhichType(const char *keyword)
{
    if(!keyword)
        return dUNKNOWN;
    switch(tolower(keyword[0])){
        case 'r':
            return dRES;
        case 'c':
            return dCAP;
        case 'l':
            return dIND;
	    case 'k':
			return dMUIND;		
        case 'g':
            return dVCCS;
        case 'e':
            return dVCVS;
        case 'f':
            return dCCCS;
        case 'h':
            return dCCVS;
        case 'v':
            return dVOL;
        case 'i':
            return dCUR;
        case 'd':
            return dDIODE;
        case 'q':    
            return dBJT;
        case 'j':
            return dJFET;
        case 'm':
            return dMOSFET;
        case 'z':
            return dMESFET;
        default:
            return dUNKNOWN;
        }
}

/*----------------------------------------------------
    FuncName: ddd2coeff
    Spec:mapping from ddd node label to coefficient node label
 -----------------------------------------------------
    Inputs: ddd node label
    Outputs: coefficient node label. 

    Global Variblas used: node
----------------------------------------------------*/
int    
ddd2coeff(int label)
{
    if(label%2 == 0)
        return label*2;
    else
        return label*2 + 1;
}

/*----------------------------------------------------
    FuncName: coeff2ddd
    Spec:mapping from coefficient node label to coefficient ddd label
 -----------------------------------------------------
    Inputs: coefficient node label. 
    Outputs: ddd node label

    Global Variblas used: node
----------------------------------------------------*/
int    
coeff2ddd(int label)
{
    if(label%2 == 0)
        return label/2;
    else
        return (label-1)/2;
}

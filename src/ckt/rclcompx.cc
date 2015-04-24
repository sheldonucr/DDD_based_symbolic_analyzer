
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
 *    $RCSfile: rclcompx.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


#include <limits.h>
#include "rclcompx.h"

#ifndef DBL_MAX
#define DBL_MAX     1.7976931348623157E+308
#endif

/*********************************************************
operator reloadings of rcl_complex class
**********************************************************/

/* 
**    operator between rcl_complex and double     
**    'b' here is assumed as frequency 
*/

rcl_complex operator ^ (rcl_complex a, double b)
{
    rcl_complex c;

    c.r = a.r;
    c.c = a.c*b;
    /*
    if(!b){
        print_error(INT_ERROR,"divided by zero...");
        c.l = c.l = DBL_MAX;
        return c;
    }
    */
    c.l = a.l*(1/b);

    return c;
}

rcl_complex operator ^ (double a, rcl_complex b)
{
    rcl_complex c;

    c.r = b.r;
    c.c = b.c*a;

/*
    if(!a){
        print_error(INT_ERROR,"divided by zero...");
        c.l = c.l = DBL_MAX;
        return c;
    }
*/
    c.l = b.l*(1/a);

    return c;
}

/*
**    Operator between rcl_complex and complex
*/

complex operator+(rcl_complex a, complex b)
{
    return complex(a.r + real(b), a.c + a.l + imag(b));
}

complex operator+(complex a, rcl_complex b)
{
    return complex(b.r + real(a), b.c + b.l + imag(a));
}

complex operator-(rcl_complex a, complex b)
{
    return complex(a.r - real(b), a.c + a.l - imag(b));
}

complex operator-(complex a, rcl_complex b)
{
    return complex(real(a) - b.r, imag(a) - (b.c + b.l));
}

/* 
**    operator bewteen rcl_complex and double
*/

rcl_complex operator+( rcl_complex a, double b)
{     
    rcl_complex c;

    c.r = a.r + b;
    c.c = a.c;
    c.l = a.l;

    return c;
}

rcl_complex operator+( double a, rcl_complex b)
{
    rcl_complex c;

    c.r = a + b.r;
    c.c = b.c;
    c.l = b.l;

    return c;
}


rcl_complex operator-( rcl_complex a, double b)
{     
    rcl_complex c;

    c.r = a.r - b;
    c.c = a.c;
    c.l = a.l;

    return c;
}

rcl_complex operator-( double a, rcl_complex b)
{
    rcl_complex c;

    c.r = a - b.r;
    c.c = b.c;
    c.l = b.l;

    return c;
}

ostream& operator<< (ostream& out, const rcl_complex& data)
{
    out <<"r:"<<data.get_r()<<"c:"<<data.get_c()<<"l:"<<data.get_l();
    return out;
}   

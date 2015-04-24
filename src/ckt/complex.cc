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
 *    $RCSfile: complex.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


#include "message.h"
#include <limits.h>
#include "complex.h"

#undef _DEBUG
#ifndef DBL_MAX
#define DBL_MAX     1.7976931348623157E+308
#endif
/*
**    All the operation reloading function definition
*/

complex operator*(const complex a , const complex b)
{
    complex c;
    c.re = (a.re * b.re) - (a.im * b.im);
    c.im = (a.re * b.im) + (a.im * b.re);
    return c;
}

complex operator/(const complex a, const complex b)
{
    complex c;
    double  denom;

    denom = (b.re * b.re) + (b.im * b.im);
#ifdef _DEBUG
    if(!denom){
        print_error(INT_ERROR,"divide by zero...");
        c.re = c.im = DBL_MAX;
        return c;
    }
#endif
    c.re = ((a.re * b.re) + (a.im * b.im))/denom;
    c.im = ((a.im * b.re) - (b.im * a.re))/denom;

    return c;
}


complex operator/(double a, const complex b)
{
    complex c;
    double  denom;

    denom = (b.re * b.re) + (b.im * b.im);
#ifdef _DEBUG
    if(!denom){
        print_error(INT_ERROR,"divide by zero...");
        c.re = c.im = DBL_MAX;
        return c;
    }
#endif
    c.re = (a * b.re) /denom;
    c.im = - (b.im * a)/denom;

    return c;
}

ostream& operator<< (ostream& out, const complex& data)
{
    out <<"r:"<<real(data)<<"i:"<<imag(data);
    return out;
}

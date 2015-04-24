#ifndef _RCL_COMPLEX_H
#define _RCL_COMPLEX_H

/*
 *    $RCSfile: rclcompx.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#include "globals.h"
#include <math.h>
#include "complex.h"

#define PI 3.14159265358979323846

/* the adimttance value for branch can be represented */
class rcl_complex {
    double r;        /* the admittance for resistant type elements */ 
    double c;        /* the admittance for capacitant element */
    double l;        /* the admittance for inductant element */

  public:
    rcl_complex(void)
        {r = c = l = 0.0;}
    rcl_complex(double _r, double _c, double _l)
        { r = _r; c = _c; l = _l;}
    rcl_complex(double _r)
        {r = _r; c = l = 0.0;}
    ~rcl_complex(){;}
    void set_r(double _r) { 
            if(_r != 0.0) 
                this->r = 1/_r; 
            else 
                print_error(FAT_ERROR,"divided by zero");
            }
    void set_c(double _c) { this->c = 2*_c*PI;}
    void set_l(double _l) { 
            if( _l != 0.0 ) 
                this->l = -1/(2*_l*PI);
            else 
                print_error(FAT_ERROR,"divided by zero");
            }

    double get_r() const { return r;}
    double get_c() const { return c;}
    double get_l() const { return l;}


    rcl_complex& operator=(rcl_complex _in){
        r = _in.r; c = _in.c; l = _in.l; 
        return *this; }
    rcl_complex& operator=(double _in) {
        r = _in; c = l = 0.0; 
        return *this; }

    friend rcl_complex operator+( rcl_complex, rcl_complex );
    friend rcl_complex operator-( rcl_complex, rcl_complex );

    friend rcl_complex operator+( rcl_complex, double );
    friend rcl_complex operator+( double, rcl_complex );
    friend rcl_complex operator-( rcl_complex, double );
    friend rcl_complex operator-( double, rcl_complex );

    friend rcl_complex operator*( rcl_complex, double );
    friend rcl_complex operator*( double, rcl_complex );

    // only used for frequency sweeping 
    friend rcl_complex operator ^ ( rcl_complex, double );
    friend rcl_complex operator ^ ( double, rcl_complex );

    friend complex operator+( rcl_complex, complex );
    friend complex operator+( complex, rcl_complex );
    friend complex operator-( rcl_complex, complex );
    friend complex operator-( complex, rcl_complex );


};

/* operation among rcl_complexes */
inline rcl_complex operator+(rcl_complex a,rcl_complex b)
    { return rcl_complex(a.r+b.r,a.c+b.c,a.l+b.l); }
inline rcl_complex operator-(rcl_complex a, rcl_complex b)
    { return rcl_complex(a.r-b.r,a.c-b.c,a.l-b.l);}
inline rcl_complex operator * (rcl_complex a, double b)
    { return rcl_complex(a.r*b, a.c*b, a.l*b);}
inline rcl_complex operator * (double a, rcl_complex b)
    { return rcl_complex(a*b.r, a*b.c, a*b.l);}

ostream& operator<< (ostream& , const rcl_complex& );

#endif /* _data_complex_h */

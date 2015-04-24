/*
 *    $RCSfile: complex.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <math.h>
#include <errno.h>


class complex {

  public:
    complex(double _v, double _i=0.0);
    complex();
    void operator+= (const complex);
    void operator+= (double);
    void operator-= (const complex);
    void operator-= (double);
    void operator*= (const complex);
    void operator*= (double);
    void operator/= (const complex);
    void operator/= (double);

    friend double  real(const complex&);    // real part 
    friend double  imag(const complex&);    // imaginary part
    friend complex conj(const complex);        // complex conjugate
    friend double  norm(const complex);        // square of the magnitude
    friend double  magn(const complex);        // magniture  
    friend double  db20(const complex);        // voltage db(=20*log10(z))  
    friend double  arg(const complex);        // angle in the complex plane

    // Create a complex object given polar coordinates
    friend complex polar(double _mag, double _arg=0.0);

    // Overloaded ANSI C math functions
    friend double  abs(const complex);
    friend complex acos(const complex);
    friend complex asin(const complex);
    friend complex atan(const complex);
    friend complex cos(const complex);
    friend complex cosh(const complex);
    friend complex exp(const complex); 
    friend complex log(const complex); 
    friend complex log10(const complex);
    friend complex pow(double, const complex _exp);
    friend complex pow(const complex, int _exp);
    friend complex pow(const complex, double _exp);
    friend complex pow(const complex, const complex _exp);
    friend complex sin(const complex);
    friend complex sinh(const complex);
    friend complex sqrt(const complex);
    friend complex tan(const complex);
    friend complex tanh(const complex);

    // Unary Operator Functions
    friend complex operator- (const complex);

    // Binary Operator Functions
    friend complex operator+ (const complex, const complex);
    friend complex operator+ (double, const complex);
    friend complex operator+ (const complex, double);
    friend complex operator- (const complex, const complex);
    friend complex operator- (double, const complex);
    friend complex operator- (const complex, double);
    friend complex operator* (const complex, const complex);
    friend complex operator* (const complex, double);
    friend complex operator* (double, const complex);
    friend complex operator/ (const complex, const complex);
    friend complex operator/ (const complex, double);
    friend complex operator/ (double, const complex);
    friend int     operator== (const complex, const complex);
    friend int     operator!= (const complex, const complex);

  private:
    double re, im;
};
inline complex::complex(double _v, double _i)    : re(_v), im(_i) { }
inline complex::complex()            : re(0.0), im(0.0) { }
inline void complex::operator+= (const complex _z) { re += _z.re; im += _z.im; }
inline void complex::operator+= (double _v2) { re += _v2; }
inline void complex::operator-= (const complex _z) { re -= _z.re; im -= _z.im; }
inline void complex::operator-= (double _z) { re -= _z; }
inline void complex::operator*= (double _v) { re *= _v; im *= _v; }
inline double real(const complex& _z) { return _z.re; }
inline double imag(const complex& _z) { return _z.im; }
inline complex conj(const complex _z) { return complex(_z.re, -_z.im); }
inline double norm(const complex _z) { return _z.re*_z.re + _z.im*_z.im; }
inline double magn(const complex _z) { return sqrt(norm(_z)); }
inline double db20(const complex _z) { return 20.0*log10(magn(_z)); }
inline double arg(const complex _z) { return atan2(_z.im, _z.re); }
inline complex operator- (const complex _z) { return complex(-_z.re, -_z.im); }
inline complex operator+ (const complex _z1, const complex _z2) {
        return complex(_z1.re + _z2.re, _z1.im + _z2.im);
    }
inline complex operator+ (double _v1, const complex _z2) {
        return complex(_v1 + _z2.re, _z2.im);
    }
inline complex operator+ (const complex _z1, double _v2) {
        return complex(_z1.re + _v2, _z1.im);
    }
inline complex operator- (const complex _z1, const complex _z2) {
        return complex(_z1.re - _z2.re, _z1.im - _z2.im);
    }
inline complex operator- (double _v1, const complex _z2) {
        return complex(_v1 - _z2.re, -_z2.im);
    }
inline complex operator- (const complex _z1, double _v2) {
        return complex(_z1.re - _v2, _z1.im);
    }
inline complex operator* (const complex _z1, double _v2) {
        return complex(_z1.re*_v2, _z1.im*_v2);
    }
inline complex operator* (double _v1, const complex _z2) {
        return complex(_z2.re*_v1, _z2.im*_v1);
    }
inline complex operator/ (const complex z1, double v2) {
        return complex(z1.re/v2, z1.im/v2);
    }
inline int operator== (const complex _z1, const complex _z2) {
        return _z1.re == _z2.re && _z1.im == _z2.im;
    }
inline int operator!= (const complex _z1, const complex _z2) {
        return _z1.re != _z2.re || _z1.im != _z2.im;
    }
inline complex pow(const complex _b, int _exp) { return pow(_b, (double)_exp); }



/*
 * Complex stream I/O
 */

#include <iostream.h>

ostream& operator<< (ostream&, const complex&);    // user may override
istream& operator>> (istream&, complex&);    // user may override


#endif /* _COMPLEX_H_ */


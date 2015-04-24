#ifndef UTILITY_H
#define UTILITY_H


/*
 *    $RCSfile: utility.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/09/30 20:55:21 $
 */

/*
** The header file for the utility functions
*/

extern char    *    CopyStr(const char *str);
extern char *    ToUpper(char *str);
extern char *    ToLower(char *str);
extern char *    NextField(char **str);
extern double     TransValue(const char *value);
extern double    RandVar(double lbound, double ubound);
extern DeviceType WhichType(const char *keyword);

extern int ddd2coeff(int);
extern int coeff2ddd(int);

#endif //UTILITY_H


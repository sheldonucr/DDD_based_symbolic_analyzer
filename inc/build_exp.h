#ifndef BUILD_EXP_H
#define BUILD_EXP_H

/*
 *    $RCSfile: build_exp.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

/* build_exp:
 *   Contains functions to build Fortran or C output expression 
 */


extern char *(*build_exp)(struct row * , char *);
extern char *build_expF(struct row *branch, char *extra_char);
extern char *build_expC(struct row *branch, char *extra_char);

#endif /*ifndef  BUILD_EXP_H */

#ifndef uinfo_H
#define uinfo_H

/*
 *    $RCSfile: unate_info.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include <iostream.h>
#include "object.h"

class UnateInfo : public Object {

    boolean     neg_unate;
    boolean     pos_unate;
    boolean     factored_out;
    
public: 
    UnateInfo()                        {neg_unate=TRUE; pos_unate=TRUE; factored_out=TRUE;}
    void    Neg(boolean b)          {neg_unate=b;} 
    void    Pos(boolean b)          {pos_unate=b;} 
    void      FactoredOut(boolean b)    {factored_out=b;}
    boolean GetNeg()                {return(neg_unate);}   
    boolean GetPos()                {return(pos_unate);}   
    boolean GetFactoredOut()        {return(factored_out);}
    ~UnateInfo()                      {;}    
 
};      
#endif

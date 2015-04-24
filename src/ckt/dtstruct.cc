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
 *    $RCSfile: dtstruct.cc,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/09/29 17:04:56 $
 */

#include "dtstruct.h"
#include "ddd_base.h"

/*----------------------------------------------------
    FuncName:
    Spec:
 -----------------------------------------------------
    Inputs:
    Outputs: when stack is empty, -1 is return

    Global Variblas used:
----------------------------------------------------*/

int
iStack::Pop()
{
    int iVal;
    iSTACKELE *iSE_aux;

    if(!head)
	{
        return -1;
	}
    else
	{
        iVal = head->key;
        iSE_aux = head;
        head = head->next;
        delete iSE_aux;
    }
    return iVal;
}

void
iStack::Push(int val)
{
    iSTACKELE *iSE_aux;

    iSE_aux = new iSTACKELE;
    iSE_aux->key = val;
    iSE_aux->next = NULL;
    if(!head)
	{
        head = iSE_aux;
	}
    else
	{
        iSE_aux->next = head;
        head = iSE_aux;
    }
}

void
iStack::Free()
{
    if(head)
	{
        FreeR(head);
	}
    head = NULL;
}

void
iStack::FreeR(iSTACKELE *list )
{
    if(list->next)
	{
        FreeR(list->next);
	}
    delete list;
}

///////////////////////////////////////
// DDD stack member function definition
///////////////////////////////////////

void
DDDstack::DDDpush(DDDnode *ddd)
{
    DDDmember * ddd_aux = new DDDmember;
    ddd_aux->ddd = ddd;
    ddd_aux->next = head;
    head = ddd_aux;
    num++;
}

DDDnode *
DDDstack::DDDpop()
{
    DDDnode   *mem_aux;
    DDDmember   *ddd_aux;

    if(head)
	{
        mem_aux = head->ddd;
        ddd_aux = head;
        head = head->next;
        delete ddd_aux;
        num--;
        return mem_aux;
    }
    else
	{// stack is empty
        return (DDDnode *)NULL;
	}
}

DDDnode *
DDDstack::GetTermByIndex
(
	int index
)
{
    DDDnode   *result = NULL;
    DDDmember   *mem_aux = head;
	int i = 0;
	
	if(index >= num)
	{
		return result;
	}
	
	int count = num - index - 1;
	
	for(i = 0; i < count; i++)
	{
        mem_aux = mem_aux->next;        
    }
	
	if(mem_aux)
	{
		result = mem_aux->ddd;
	}
	else
	{
		result = NULL;
	}

	return result;
}

///////////////////////////////////////
// DDD unique stack member function definition
///////////////////////////////////////

void
DDDUniqueStack::DDDpush(DDDnode *ddd)
{
	// first, need to make sure that the given ddd
	// does not exist.
	DDDmember *maux = head;
	bool found = false;
	for(; maux != NULL; maux = maux->next)
	{
		if(maux->ddd == ddd)
		{
			found = TRUE;
		}
	}

	if(!found)
	{
		DDDmember * ddd_aux = new DDDmember;
		ddd_aux->ddd = ddd;
		ddd_aux->next = head;
		head = ddd_aux;
		num++;
	}
}

DDDnode *
DDDUniqueStack::DDDpop()
{
    DDDnode   *mem_aux;
    DDDmember   *ddd_aux;

    if(head)
	{
        mem_aux = head->ddd;
        ddd_aux = head;
        head = head->next;
        delete ddd_aux;
        num--;
        return mem_aux;
    }
    else
	{// stack is empty
        return (DDDnode *)NULL;
	}
}

DDDnode *
DDDUniqueStack::GetTermByIndex
(
	int index
)
{
    DDDnode   *result = NULL;
    DDDmember   *mem_aux = head;
	int i = 0;
	
	if(index >= num)
	{
		return result;
	}
	
	int count = num - index - 1;
	
	for(i = 0; i < count; i++)
	{
        mem_aux = mem_aux->next;        
    }
	
	if(mem_aux)
	{
		result = mem_aux->ddd;
	}
	else
	{
		result = NULL;
	}

	return result;
}

/* remove a DDD node from the stack.  if the DDD node is removed
** successful, true is return, false otherwise.  
**/
int
DDDUniqueStack::remove(DDDnode *s)
{
    
    DDDmember   *ddd_aux, *prev, *need_del = NULL;
	boolean result = FALSE;

	//print(cout);

    if(head)
	{		       
        ddd_aux = prev = head;
        
		while(ddd_aux != NULL)
		{
			if(ddd_aux->ddd == s)
			{							
				if(ddd_aux == head)
				{
					head = head->next;
					need_del = ddd_aux;
				}
				else
				{
					prev->next = ddd_aux->next;				
					need_del = ddd_aux;
				}
			}
			prev = ddd_aux;
			ddd_aux = ddd_aux->next;
			if(need_del)
			{
				delete need_del;
				num--;
				result = TRUE;
				break;
			}
		}	
    }
	/*
	cout <<"after" << endl;
	print(cout);
	*/
	return result;
}

void
DDDUniqueStack::print(ostream& out)
{
    
    DDDmember   *ddd_aux;	

    if(head)
	{		       
        ddd_aux = head;
        
		while(ddd_aux != NULL)
		{
			out <<" " << ddd_aux->ddd;
			ddd_aux = ddd_aux->next;
		}
		out << endl;
	}	
}

///////////////////////////////////////
// queue member function definition
///////////////////////////////////////

DDDqueue::~DDDqueue()
{
    DDDmember   *ddd_aux;
    if(dddlist)
	{
        do
		{
            ddd_aux = dddlist;
            dddlist = dddlist->next;
            delete ddd_aux;
        }
		while(dddlist);
    }
}
 


void
DDDqueue::Enquque(DDDnode    *ddd)
{
    DDDmember * ddd_aux = new DDDmember;
    ddd_aux->next = NULL;
    ddd_aux->ddd = ddd;
    if(end)
	{
        end->next = ddd_aux;
	}
    end = ddd_aux;
    if(!head)
	{
        head = end;
	}
    
    num++;
	/*
    cout <<"(en)#element in queue: "<< num <<" Index: "<< 
            ddd->Index()<<endl;
    */
}

/*
** fetch a ddd from queue head
*/
DDDnode *
DDDqueue::Dequque()
{
    DDDnode   *mem_aux;
    DDDmember   *ddd_aux;

    if(head)
	{
        mem_aux = head->ddd;
        ddd_aux = head;
        head = head->next;
        if(!head) // queue is empty
		{			
			end = NULL;
		}
                
        num--;
		/*
        cout <<"(dn)#element in queue: "<< num <<" Index: "<< 
                mem_aux->Index()<<endl;
        */
        delete ddd_aux;
        return mem_aux;
    }
    else
	{
        return (DDDnode *)NULL;
	}
}



///////////////////////////////////////
// template stack member function definition
///////////////////////////////////////

/*
template <class C>
void myStack<C>::push(C *val)
{
  member * val_aux = new member;
  val_aux->val = val;
  val_aux->next = head;
  head = val_aux;
  num++;
}

template <class C> 
C * myStack<C>::pop()
{
  C    *val_aux;
  member   *mem_aux;

  if(head)
    {
      val_aux = head->val;
      mem_aux = head;
      head = head->next;
      delete mem_aux;
      num--;
      return val_aux;
    }
  else // stack is empty
    return (C *)NULL;
}

*/
///////////////////////////////////////
// tempelte queue member function definition
///////////////////////////////////////
/*
template <class C>
myQueue<C>::~myQueue()
{
    member   *mem_aux;
    if(list){
        do{
            mem_aux = list;
            list = list->next;
            delete mem_aux;
        }while(list);
    }
}
 

template <class C>
void
myQueue<C>::Enquque(C  *val)
{
    DDDmember * mem_aux = new member;
    mem_aux->next = NULL;
    mem_aux->val = val;
    if(end)
        end->next = mem_aux;
    end = mem_aux;
    if(!head)
        head = end;
   
}

*/
/*
** fetch a ddd from queue head
*/
/*
template <class C>
C * myQueue<C>::Dequque()
{
    C   *val_aux;
    member  *mem_aux;

    if(head)
      {
        val_aux = head->val;
        mem_aux = head;
        head = head->next;
        if(!head) // queue is empty
	  end = NULL;
        
        delete mem_aux;
        return val_aux;
      }
    else
      return (C *)NULL;
}
*/

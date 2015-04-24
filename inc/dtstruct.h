#ifndef BASIC_STRUCT_H
#define BASIC_STRUCT_H

/*
 *    $RCSfile: dtstruct.h,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/09/30 20:55:21 $
 */


#include <stdio.h>
#include <unixstd.h>
#include <iostream.h>

 /*
 ////////////////////////////////////////////////////
 //
 //  dtstruct.h :
 //  head file some basic data structure  
 //  definition used in our system
 //    
 //  X. Sheldon Tan
 //  (c) Copyright the University of Iowa, 1997
 //
 //
 ///////////////////////////////////////////////////
 */


/* 
**    Some basic data definition (stack, queue ...) 
*/
 
////////////////////////////////////////////////
//Stack definition
///////////////////////////////////////////////


class DDDnode;
class DDDmanager;

typedef struct _StackEle {
    int key;
    struct _StackEle *next;
} iSTACKELE;


class iStack {
    iSTACKELE *head;

    public:
    iStack() {head = NULL;}
    void Free();
    ~iStack(){Free();}
    void FreeR(iSTACKELE *list );
    int Pop();
    void Push(int val);
};

////////////////////////////////////////////////
//DDD Queue definition
///////////////////////////////////////////////

typedef struct _DDDmember {
    DDDnode    *ddd;
    struct _DDDmember *next;
} DDDmember;

class DDDqueue {

    DDDmember   *dddlist;
    DDDmember   *head;
    DDDmember   *end;
    int         num; // #element in queue

    public:
    DDDqueue() {dddlist=NULL;head=NULL;end=NULL;num=0;}
    ~DDDqueue();
	int    isEmpty() { if(!head) return 1; else return 0;}

    void Enquque(DDDnode *ddd);
    DDDnode * Dequque();
};

////////////////////////////////////////////////
//DDD Stack definition
///////////////////////////////////////////////

class DDDstack {

    DDDmember   *head;
    int         num; // #element in queue

    public:
    DDDstack() {head=NULL;num=0;}
    ~DDDstack() { ClearStack(); };

    void DDDpush(DDDnode *ddd);
    void push(DDDnode *ddd) { DDDpush (ddd);}
    DDDnode * DDDpop();
    DDDnode * pop () { return DDDpop(); }	

    int    isEmpty() { if(!head) return 1; else return 0;}
    int    StackSize() { return num; }
	DDDnode * GetTermByIndex( int num );
	DDDnode * GetHeadDDD() { return head? (head->ddd):NULL; }
    void ClearStack() 
		{
			if(head)
			{
				while(pop());
			}
			head=NULL; num=0;
		}
};

class DDDUniqueStack {

    DDDmember   *head;
    int         num; // #element in queue

    public:
    DDDUniqueStack() {head=NULL;num=0;}
    ~DDDUniqueStack() { ClearStack(); };

    void DDDpush(DDDnode *ddd);
    void push(DDDnode *ddd) { DDDpush (ddd);}
    DDDnode * DDDpop();
    DDDnode * pop () { return DDDpop(); }
	int   remove (DDDnode *);
	void  print(ostream& out = cout);

    int    isEmpty() { if(!head) return 1; else return 0;}
    int    StackSize() { return num; }
	DDDnode * GetTermByIndex( int num );
	DDDnode * GetHeadDDD() { return head? (head->ddd):NULL; }
    void ClearStack() 
		{
			if(head)
			{
				while(pop());
			}
			head=NULL; num=0;
		}
};


//////////////////////////////////
// General quene and stack
/////////////////////////////////

// template <class C> class myQueue<C> {

//  private:
//   typedef struct _member {
//     C    *val;
//     struct _member *next;
//   } member;
  
//   member   *list;
//   member   *head;
//   member   *end;
//   int       num; // #element in queue

//  public:
//   myQueue() {list=NULL;head=NULL;end=NULL;num=0;}
//   ~myQueue()
//     {
//       member   *mem_aux;
//       if(list){
//         do{
// 	  mem_aux = list;
// 	  list = list->next;
// 	  delete mem_aux;
//         }while(list);
//       }
//     }

//   void Enquque(C *val)
//     {
//       DDDmember * mem_aux = new member;
//       mem_aux->next = NULL;
//       mem_aux->val = val;
//       if(end)
//         end->next = mem_aux;
//       end = mem_aux;
//       if(!head)
//         head = end;
   
//     }

//   C * Dequque()
//     {
//       C   *val_aux;
//     member  *mem_aux;

//     if(head)
//       {
//         val_aux = head->val;
//         mem_aux = head;
//         head = head->next;
//         if(!head) // queue is empty
// 	  end = NULL;
        
//         /*
// 	  num--;
// 	  cout <<"(dn)#element in queue: "<< num <<" Index: "<< 
// 	  mem_aux->Index()<<endl;
// 	  */
//         delete mem_aux;
//         return val_aux;
//       }
//     else
//       return (C *)NULL;
//     }
// };

// ////////////////////////////////////////////////
// // Stack definition
// ///////////////////////////////////////////////

// template <class C> class myStack<C> {

// private:

//   typedef struct _member {
//     C    *val;
//     struct _member *next;
//   } member;

//   member   *head;
//   int  num; // #element in queue

// public:
//   myStack() {head=NULL;num=0;}
//   ~myStack(){};

//   void push(C *val)
//     {
//       member * val_aux = new member;
//       val_aux->val = val;
//       val_aux->next = head;
//       head = val_aux;
//       num++;
//     }

//   C * pop()
//     {
//       C    *val_aux;
//       member   *mem_aux;

//       if(head)
// 	{
// 	  val_aux = head->val;
// 	  mem_aux = head;
// 	  head = head->next;
// 	  delete mem_aux;
// 	  num--;
// 	  return val_aux;
// 	}
//       else // stack is empty
// 	return (C *)NULL;
//     }

//   int isEmpty() { if(!head) return 1; else return 0;}
//   int StackSize() { return num; }
//   void ClearStack() { head=NULL; num=0;}
// };

#endif //BASIC_STRUCT_H






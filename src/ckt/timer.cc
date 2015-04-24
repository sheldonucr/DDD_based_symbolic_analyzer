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
 *    $RCSfile: timer.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


#undef RUSAGE_SELF
#include "timer.h"

/*
extern "C" {
extern int getrusage(int who, struct rusage *rusage);
}
*/

#ifdef RUSAGE_SELF
void
Timer::Start()
{
   // zero out resource fields
   res.ru_utime.tv_sec = 0L;
   res.ru_utime.tv_usec = 0L;
   res.ru_stime.tv_sec = 0L;
   res.ru_stime.tv_usec = 0L;
   res.ru_majflt = 0;
   ASSERT(getrusage(RUSAGE_SELF,&start) == 0,"Timer::Start : getrusage error");
   res.ru_maxrss = start.ru_maxrss;
   state = STARTED;
}

// c = a - b
void
time_val_subtract(timeval *a,timeval *b, timeval *c)
{
   long usec;
   usec = a->tv_usec - b->tv_usec;
   // borrow
   if (usec < 0) {
      a->tv_sec -= 1L;
      usec += 1000000;
   }
   c->tv_usec += usec;
   c->tv_sec += a->tv_sec - b->tv_sec;
}

void
Timer::Stop()
{
   struct rusage end;

   ASSERT(state == STARTED,"Timer::Stop : timer not started");
   ASSERT(getrusage(RUSAGE_SELF,&end) == 0, "Timer::Stop : getrusage error");

   // only maintain those fields we return!
   res.ru_maxrss = end.ru_maxrss;
   res.ru_majflt += end.ru_majflt - start.ru_majflt;
   
   time_val_subtract(&end.ru_utime,&start.ru_utime,&res.ru_utime);
   time_val_subtract(&end.ru_stime,&start.ru_stime,&res.ru_stime);

   state = STOPPED;
}

void
Timer::Continue()
{
   ASSERT(state == STOPPED,"Timer::Continue : timer not stopped");
   ASSERT(getrusage(RUSAGE_SELF,&start)==0,"Timer::Continue :getrusage error");
   state = STARTED;
}

double
Timer::UserTime()
{
   return (0.000001 * res.ru_utime.tv_usec) + res.ru_utime.tv_sec;
}

double
Timer::SystemTime()
{
   return (0.000001 * res.ru_stime.tv_usec) + res.ru_stime.tv_sec;
}

void
Timer::Print(ostream &out)
{
   out << "Timer #<" << (void *)this << "> :\n";
   out << "\tUser time = " << UserTime() << "\n";
   out << "\tSystem time = " << SystemTime() << "\n";
   out << "\tRSS = " << res.ru_maxrss << "\n";
   out << "\tPage Faults = " << res.ru_majflt << "\n";
}
   
#else


void
Timer::Start()
{
   // zero out resource fields
   res.lwp_utime.tv_sec = 0L;
   res.lwp_utime.tv_nsec = 0L;
   res.lwp_stime.tv_sec = 0L;
   res.lwp_stime.tv_nsec = 0L;
   ASSERT(_lwp_info(&start) == 0,"Timer::Start : _lwp_info error");
   state = STARTED;
}

// c = a - b
void
time_val_subtract(timestruc_t *a,timestruc_t *b, timestruc_t *c)
{
   long nsec;
   nsec = a->tv_nsec - b->tv_nsec;
   // borrow
   if (nsec < 0) {
      a->tv_sec -= 1L;
      nsec += 1000000000;
   }
   c->tv_nsec += nsec;
// fix?
   if (c->tv_nsec>=1000000000) {
      c->tv_sec += 1L;
      c->tv_nsec -= 1000000000;
   }
//
   c->tv_sec += a->tv_sec - b->tv_sec;
}

void
Timer::Stop()
{
   struct lwpinfo end;

   ASSERT(state == STARTED,"Timer::Stop : timer not started");
   ASSERT(_lwp_info(&end) == 0, "Timer::Stop : getrusage error");

   // only maintain those fields we return!

   time_val_subtract(&end.lwp_utime,&start.lwp_utime,&res.lwp_utime);
   time_val_subtract(&end.lwp_stime,&start.lwp_stime,&res.lwp_stime);

   state = STOPPED;
}

void
Timer::Continue()
{
   ASSERT(state == STOPPED,"Timer::Continue : timer not stopped");
   ASSERT(_lwp_info(&start)==0,"Timer::Continue :getrusage error");
   state = STARTED;
}

double
Timer::UserTime()
{
   return (0.000000001 * res.lwp_utime.tv_nsec) + res.lwp_utime.tv_sec;
}

double
Timer::SystemTime()
{
   return (0.000000001 * res.lwp_stime.tv_nsec) + res.lwp_stime.tv_sec;
}

void
Timer::Print(ostream &out)
{
   out << "Timer #<" << (void *)this << "> :\n";
   out << "\tUser time = " << UserTime() << "\n";
   out << "\tSystem time = " << SystemTime() << "\n";
}

#endif // no RUSAGE_SELF

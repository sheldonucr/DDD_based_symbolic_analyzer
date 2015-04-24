#ifndef TIMER_H
#define TIMER_H

/*
 *    $RCSfile: timer.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

// Timer class

// This class is a "friendly" interface to the getrusage system call. It
// provides bookeeping of the rusage data structure over arbitrary user
// specified intervals. It is ideal for keeping statistics on particular
// sections of code. See the man pager for getrusage for documentation on
// rusage fields;

#include <sys/time.h>

#include <sys/resource.h>    // SunOs4.x

#ifndef RUSAGE_SELF
#include <sys/lwp.h>        // SunOs5.x
#endif

#include "object.h"

enum TimerState {
   NOT_STARTED,
   STARTED,
   STOPPED
};

class Timer : public Object {
   // rusage is used in SunOs4.x
# ifdef RUSAGE_SELF

   struct rusage res;
   struct rusage start;
   struct rusage stop;
# else    // lwp (light weight process) info used in SunOs 5.4(5.x?)
   struct lwpinfo res;
   struct lwpinfo start;
   struct lwpinfo stop;
# endif

   TimerState state;
   
 public:
   Timer() {
      state = NOT_STARTED;
   }

   void Start();       // Start the timer, also resets it to zero
   void Stop();        
   void Continue();    // Restart after a Stop();

   // These return time in seconds ...
   double UserTime(); 
   double SystemTime();

# ifdef RUSAGE_SELF
   long PageFaults() { return res.ru_majflt; }
   long RSS() { return res.ru_maxrss; }
# endif

   virtual void Print(ostream &out = cout);
};

inline ostream& operator << (ostream &out, Timer& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, Timer* f)
{ out << "(ptr->)"; f->Print(out); return out; }

#endif // TIMER_H


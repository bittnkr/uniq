//==============================================================================
// Actor • A common parent for active classes
//==============================================================================
#pragma once
#include "call.h"
#include "unistd.h"
namespace uniq {

inline void sleep() { sched_yield(); }
inline void sleep(int ms) { usleep(ms*1000); }
inline void sleep(double secs) { usleep(secs * 1000'000); }
inline const int coreCount() { return thread::hardware_concurrency(); }
#define WAIT(condition) while(!(condition)) { sleep(); }

//===================================================================== Actor<T>
struct Actor  { // todo: inherit Actor from State
 protected:
  bool _running = false;
  voidfunction beat = [] {}; // voidfunction beat = []<typename... Args>(Args&&... args) { log(args...); };

 public:
  template <typename Func, typename... Args> 
  Actor(Func&& f, Args&&... args) {
    ASSERT_INVOCABLE(Func, Args);
    beat = bind(forward<Func>(f), forward<Args>(args)...);
    start();
  }
  // BoolState running([]{
  //   on(false, []{}); 
  //   on(true, []{}); 
  // });

  Actor() { start(); }
  ~Actor() { stop(); }

  virtual void start() { _running = true; }
  virtual void stop() { _running = false; }
  inline bool running() { return _running; }
  virtual int ready() { return 1; }  // beat can be called

  // functor
  inline void operator()() { beat(); }
};

}// UniQ • Released under GPL 3 licence
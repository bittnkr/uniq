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

// ======================================================================= Actor
TEST(Actor){
  int X = 0;
  // increment X with a lambda call
  auto L = [&]{ X++; }; L(); CHECK(X==1);

  // increment X with the actor, using the same lambda
  Actor A(L); CHECK(A.running());
  A(); CHECK(X==2);

  // increment X with another actor and lambda
  Actor B([&]{ X=X+1; }); 
  B(); CHECK(X==3);

  // Actor<int>C(B); // passing a functor as the callable param copy constructor??
  // C(); CHECK(X==4);

  A.stop(); CHECK(!A.running());
}//*///

}// UniQ • Released under GPL 3 licence
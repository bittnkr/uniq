//==============================================================================
// Actor • A common parent for active classes
//==============================================================================
#pragma once
#include "call.h"
namespace uniq {

inline void sleep() { sched_yield(); }
inline void sleep(int ms) { usleep(ms*1000); }
inline const int coreCount() { return thread::hardware_concurrency(); }
#define WAIT(condition) while(!(condition)) { sleep(); }

//===================================================================== Actor<T>
template <typename T> struct Actor {
 protected:
  bool _running = false;
  uniq::voidfunction beat = [] {}; // voidfunction beat = []<typename... Args>(Args&&... args) { log(args...); };

  virtual void onempty() {}
  virtual void onfull() {}

 public:
  template <typename Func, typename... Args>
  Actor(Func&& f, Args&&... args) {
    ASSERT_INVOCABLE(Func, Args);
    beat = bind(forward<Func>(f), forward<Args>(args)...);
    start();
  }

  Actor() { start(); }
  ~Actor() { stop(); }

  virtual void start() { _running = true; }
  virtual void stop() { _running = false; }
  inline bool running() { return _running; }

  virtual int push(const T& item, bool wait = true) { return 0; }
  virtual int pop(T& item, bool wait = true) { return 0; }

  // virtual bool has(const T &item) { return 0; }
  // virtual bool T first(T &item) { return 0; }
  // virtual bool T last(T &item) { return 0; }
  // virtual bool T next(T &item) { return 0; }

  virtual bool full(){ return 1; }
  virtual bool empty(){ return 1; }

  virtual int ready() { return 1; }  // beat can be called

  // functor
  inline void operator()() { beat(); }
};

}  // namespace uniq
//==============================================================================
// Actor • You push, pop & call it
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {
#define FWD(...) forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

//===================================================================== Actor<T>
template <typename T> struct Actor {
  voidfunction beat = []<typename... Args>(Args&&... args) { log(args...); };

  template<typename Func, typename... Args> 
  Actor(Func&& f, Args&&... args){
   	ASSERT_INVOCABLE(Func, Args);
    beat = bind(forward<Func>(f), forward<Args>(args)...);
    start();
  }

  Actor(){ 
    // auto f = []<typename... Args>(Args&&... args) { log(args...); };
    // log(anyType(f));
    // beat = [&]{ log("default Actor::beat()"); }; 
    start(); 
  }

  ~Actor(){ stop(); }

  bool running = false;
  virtual void start() { running = true; } 
  virtual void stop(){ running = false; }

  virtual int push(const T &item, bool wait = true) { return 1;  }
  virtual int pop(T &item, bool wait=true) { item = T{}; return 1; }

  virtual void onempty(){ }
  virtual void onfull(){ }

  // functor
  inline void operator()(){ beat(); }
};

// ================================================================= TEST(Actor)
TEST(Actor){
  int X = 0;
  // increment X with a lambda call
  auto L = [&]{ X++; }; L(); CHECK(X==1);

  // increment X with the actor, using the same lambda
  Actor<int>A(L); CHECK(A.running);
  A(); CHECK(X==2);

  // increment X with another actor and lambda
  Actor<int>B([&]{ X=X+1; }); 
  B(); CHECK(X==3);

  // Actor<int>C(B); // passing a functor as the callable param copy constructor??
  // C(); CHECK(X==4);

  CHECK(A.push(1)); // push something on it. (accepted and ignored by default)
  CHECK(A.pop(X)); // pop by default clear the variable returning T{}
  CHECK(X==0); 

  A.stop(); CHECK(!A.running);
};//*/

// ===================================================================== Counter
struct TestCounter: public Actor<int> {
  int value=0;
  TestCounter(int start=0) : Actor<int>( [&](int v=1){ value+=v; } ), value(start) { }
};

TEST(TestCounter){
  TestCounter c(1); CHECK(c.value==1); 
  c(); CHECK(c.value==2); 
};//*/

}// uniq • Released under GPL3


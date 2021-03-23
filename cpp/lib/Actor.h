#pragma once
#include "uniq.h"
namespace uniq {

// Actor ======================================================================
class Actor { // : EventEmmiter {
protected:
public:
  Actor(){ start(); }
  Actor(void (*beat_)()){ beat=beat_; Actor(); }
  ~Actor(){ stop(); }

  bool running;
  virtual void start(){ running = true; } 
  virtual void stop(){ running = false; }

  Time interval;
  i64 counter=0;
  Time taken; 
  voidfunction beat = [this]{ }; 
  inline void operator()(){ beat(); counter++;}
};

// Tests =======================================================================
#include "test.h"
void test_Actor() { 
  Actor a; 
  CHECK(a.running);
  a.stop(); CHECK(!a.running);

  // int x = 1;
  // auto adder = Actor([&x](int v){ x += v; }); 
  // adder(1);
  // CHECK(x==2);

};
}// uniq • Released under GPL 3.0

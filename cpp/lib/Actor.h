#pragma once
#include "uniq.h"
namespace uniq {

// Actor ======================================================================
class Actor { // : EventEmmiter {
protected:
public:
  Actor(){ start(); }

  Actor(void (*lambda_)()){ lambda=lambda_; start(); }
    
  ~Actor(){  stop(); }

  bool running;  
  virtual void start(){ running = true; } 
  virtual void stop(){ running = false; }

  int counter = 0;
  voidfunction lambda = [this]{ counter++; }; 

  virtual void beat(){ lambda(); };

  inline void operator()(){ beat(); }
};

// Tests =======================================================================
TEST(Actor) { 
  Actor a; 
  CHECK(a.running);
  a.stop(); CHECK(!a.running);

  // int x = 1;
  // auto adder = Actor([&x](int v){ x += v; }); 
  // adder(1);
  // CHECK(x==2);

};
}// uniq • Released under GPL 3.0

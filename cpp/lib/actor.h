#pragma once
namespace uniq {

// Actor ======================================================================
class Actor { // : EventEmmiter {
protected:
public:
  Actor(){ start(); }
  ~Actor(){ stop(); }

  bool running;

  virtual void start(){ running = true; } 
  virtual void stop(){ running = false; }

  virtual void beat(){ };
};

// Tests =======================================================================
#include "test.h"
void test_actor() { 
  Actor a; CHECK(a.running);
  a.stop(); CHECK(!a.running);
};

}// uniq • Released under GPL 3.0

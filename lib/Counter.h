//==============================================================================
// Counter • A a simple Actor
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

// ============================================================= Counter : Actor
struct Counter : public Actor {
  Atomic<int> counter = 0;

  Counter(int start = 0)
      : Actor([&]{ counter++; }) { counter = start; }

  int operator()(int inc=1){ return counter.add(inc); }

  operator int() { return counter; }
};

ostream& operator<<(ostream& os, Counter& c) { return os << c.counter; }

TEST(Counter){ // ====================================================== Counter
  // CHECK(sizeof(Counter)==sizeof(int))
  Counter plus(1); CHECK(plus.counter==1); 
  plus(); CHECK(plus==2); 
  plus(2); CHECK(plus==4); 
  CHECK(sstr(plus)=="4");
};//*/

}// uniq • Released under GPL 3.0
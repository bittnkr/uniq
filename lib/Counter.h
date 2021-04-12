//==============================================================================
// Counter • A Counter State 
//==============================================================================
#pragma once
#include "State.h"
namespace uniq {

// ============================================================= Counter : State
struct Counter : public State<int> {
  // Counter() { }
  Counter(int start = 0) { 
    state = start; 
    onenter = [&]{ AddAtomic<int>(state, 1); };
  }
  operator int() { return state; }
  int operator()(int increment){ while(increment--) enter(); }
};

ostream& operator<<(ostream& os, Counter& c) { return os << c.state; }

TEST(Counter){ //================================================= TEST(Counter)
  // CHECK(sizeof(Counter)==sizeof(int))
  Counter<int> plus(1); CHECK(plus.state==1); 
  plus(); CHECK(plus==2); 
  plus(2); CHECK(plus==4); 
  CHECK(sstr(plus)=="4");
};

}// UniQ • Released under GPL 3 licence
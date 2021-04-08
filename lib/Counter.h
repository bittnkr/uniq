//==============================================================================
// Counter • A a simple Actor
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

// ============================================================= Counter : Actor
struct Counter : public Actor<integer> {
  Atomic<integer> value = 0;

  Counter(int start = 0)
      : Actor<integer>([&]{ value++; }) { value = start; }

  integer operator()(int inc=1){ return value.add(inc); }

  operator integer() { return value; }
};

TEST(Counter){ // ====================================================== Counter
  // CHECK(sizeof(Counter)==sizeof(integer))
  Counter plus(1); CHECK(plus.value==1); 
  plus(); CHECK(plus==2); 
  plus(2); CHECK(plus==4); 
};//*/

}// uniq • Released under GPL 3.0
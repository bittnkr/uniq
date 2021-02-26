#pragma once
namespace test{
#include "uniq.h"

atomic<int> TEST_CHECKS = 0;

#define CHECK(cond) assert(cond); TEST_CHECKS++

void runtest(voidfunc f, string name=""){
    f();
    cout << "\033[38;5;202m" << name <<" \033[0;32mpassed\033[0m" << endl;
  };
}// test
// part of UniQ library - released under GPL 3.0
#pragma once
#include "std.h"

atomic<int> TEST_CHECKS = 0;
#define CHECK(cond) assert(cond); TEST_CHECKS++

void run_test(void (*f)(void), string name=""){
  f();
  cout << " " << name <<" \033[0;32mpassed\033[0m" << ""<< endl;
};
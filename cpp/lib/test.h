#pragma once
#include "std.h"

std::atomic<int> TEST_CHECKS = 0;

#define CHECK(cond) assert(cond); TEST_CHECKS++

void runtest(std::function<void()> f, std::string name=""){
  f();
  std::cout << "\033[38;5;202m" << name <<" \033[0;32mpassed\033[0m" << endl;
};
// part of UniQ library - released under GPL 3.0
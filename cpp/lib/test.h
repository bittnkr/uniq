#pragma once
namespace test{
  #include "common.h"

  atomic<int> CHECKS = 0;

  void runtest(void (*f)(void), string name=""){
    f();
    cout << "\033[38;5;202m" << name <<" \033[0;32mpassed\033[0m" << endl;
  };
}// test

#define CHECK(cond) assert(cond); test::CHECKS++

// part of UniQ library - released under GPL 3.0
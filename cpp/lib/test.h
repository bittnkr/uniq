#pragma once
#include "std.h"

atomic<int> TEST_CHECKS = 0;

#define CHECK(cond) assert(cond); TEST_CHECKS++

void test(function<void()> f, string name=""){
    f();
    cout << "\033[38;5;202m" << name <<" \033[0;32mpassed\033[0m" << endl;
};
// uniq • Released under GPL 3.0
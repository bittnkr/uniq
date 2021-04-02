#pragma once
#include "std.h"

int TEST_PASSED = 0;
int TEST_FAILED = 0;

#define CHECK(cond) { assert(cond); TEST_PASSED++; }

typedef void (*testFunc)();

map<string, testFunc> TESTS = {};

struct Test {
  Test(string name, testFunc func) {
    TESTS[name] = move(func);
    cout << name << "\n";
  }
};

void runTests() {
  cout << "Running tests ------------------------------\n";
  for (auto const& [name, f] : TESTS) {
    f();
    cout << "\033[38;5;202m" << name << " \033[0;32mpassed\033[0m\n";
  }
  cout << TEST_PASSED << " tests passed------------------------------\n";
};

#define TEST(name)                               \
  void test_##name();                            \
  static Test test__##name(#name, &test_##name); \
  void test_##name()

TEST(Test) {
  CHECK(true);
}
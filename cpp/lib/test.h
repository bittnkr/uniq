#pragma once
#include "std.h"

struct Test {
  string expr;
  string file;
  int line;
  exception ex;

  Test(const string expr, const string file, int line)
      : expr(expr), file(file), line(line) {
    cout << "CHECK(" << expr << ") failed " << file << ":" << line << "\n";
  };
};

typedef void (*testFunc)();

struct TestCase;

map<string, TestCase*> TESTS = {};

struct TestCase {
  string name;    // name of the test case
  testFunc func;  // a function pointer to the test case
  string file;    // the file in which the test was registered (using String - see #350)
  unsigned line;  // the line where the test was registered

  TestCase(string name, testFunc f, string file, unsigned line)
    : name(name), func(f), file(file), line(line) {
    TESTS[name] = this;
  }
};

int TEST_PASSED = 0;
int TEST_FAILED = 0;

void runTests() {
  cout << "Running tests ------------------------------\n";
  for (auto [name, test] : TESTS) {
    test->func();
    cout << "\033[38;5;202m" << test->name << " \033[0;32mpassed\033[0m\n";
  }
  cout << TEST_PASSED << " tests passed------------------------------\n";
};

// #define TESTING 1
// #if TESTING
#define ASSERT(x) if(!(x)) Test(#x, __FILE__, __LINE__)
#define CHECK(cond) try{ ASSERT(cond); TEST_PASSED++;} catch(...){ TEST_FAILED++; }
#define TEST(name)                               \
  void test_##name();                            \
  static TestCase test__##name(#name, &test_##name,__FILE__,__LINE__); \
  void test_##name()
// #else
//   #define ASSERT(x) ((void)sizeof(x))
//   #define CHECK(X) ASSERT(x)
//   #define TEST(name) 
// #endif

TEST(Test) {
  CHECK(1==1);
}

#pragma once
#include "std.h"
#include "terminal.h"
namespace uniq {

#define ASSERT(expr) assert(expr);
#define ASSERT_INVOCABLE(F,A) static_assert( __is_invocable<typename decay<F>::type, typename decay<A>::type...>::value, "Actor() arguments must be invocable after conversion to rvalues");

// #ifdef TESTING
//========================================================================= Test
struct Test;

map<string, Test*> TESTS = {};

typedef void (*testFunc)();

struct Test {
  string name;
  testFunc func;
  string file;
  int line;

  Test(string name, testFunc f, string file, int line) : name(name), func(f), file(file), line(line) {
    uniq::TESTS[name] = this;
  }
};

//=================================================================== TEST(name)
#define TEST(name)                                                         \
  void test_##name();                                                      \
  static uniq::Test test__##name(#name, &test_##name, __FILE__, __LINE__); \
  void test_##name()

int TEST_PASSED = 0;
int TEST_FAILED = 0;

//========================================================================= Fail
struct Fail : public exception {
  string expr;
  string func;
  string file;
  int line;

  Fail(string expr, string func, string file, int line) : expr(expr), func(func), file(file), line(line) {
    uniq::TEST_FAILED++;
  };

	const char * what () const throw (){
    log("\n", BLD,RED," ✗ ", RST,GRN,func, RST,"{", MAG,"CHECK",RST,"(",expr, ")} ", BLD,RED,"failed", RST," at ",file,":", line);
    return nullptr;
  }
};

//=================================================================== runTests()
void runTests() {
  log("Running tests...");
  for (auto [name, test] : TESTS) {
    out(ORA, test->name, " ");
    try {
      test->func();
      out("\n");
    } catch (const exception& e) {
      log(e.what());
    };
  };
  log(TEST_PASSED, GRN," passed ", TEST_FAILED ? sstr(RST,TEST_FAILED, BLD,RED," failed.") : "");
  TESTS.clear();
};

//================================================================== CHECK(expr)
#define CHECK(expr)                                      \
  if (expr) {                                            \
    out(GRN, "✓");                                       \
    uniq::TEST_PASSED++;                                 \
  } else {                                               \
    throw uniq::Fail(#expr, __FUNCTION__, __FILE__, __LINE__); \
  }//*/

//======================================================== CHECK_EXCEPTION(expr)
#define CHECK_EXCEPTION(expr)                            \
  try {                                                  \
    expr;                                                \
    uniq::Fail(#expr, __FUNCTION__, __FILE__, __LINE__); \
  } catch (std::exception & e) {                         \
    out(GRN, "✓");                                       \
    uniq::TEST_PASSED++;                                 \
  }

//=================================================================== runTests()
TEST(Test) {
  CHECK_EXCEPTION(throw exception());
  CHECK(1 == 1);
  try{
    CHECK(1 == 2);
    throw exception();
  } catch(...) { 
    TEST_FAILED--;
    CHECK(true); 
  }
}

// #else
//   #define CHECK(x) ((void)sizeof(#x))
//   #define CHECK_EXCEPTION(x) ((void)sizeof(#x))
//   #define TEST(x) void test_##x()
//   void runTests(){}
// #endif
}// uniq • Released under GPL 3.0

#pragma once
#include "std.h"
#include "utils.h"
#include "terminal.h"
#include "numtypes.h"
#include "Named.h"
#include "OpenQueue.h"
#include "Time.h"

namespace uniq {
// #ifdef TESTS
bool SILENT_TESTS = 0; // omit successul tests
bool MUTE_TESTS = 0; // omit even failures, just return success or failure

#define OUT(...) if(!(SILENT_TESTS||MUTE_TESTS)) { out(__VA_ARGS__); }
#define FAIL(...) if(!MUTE_TESTS) { out(__VA_ARGS__); }

int TEST_COUNT = 0;
int TEST_PASSED = 0;
int TEST_FAILED = 0;
int TEST_EXCEPTION = 0;

const string TEST_OK =  BLD+GRN+"✓"+RST;
const string TEST_FAIL = BLD+RED+"✘"+RST;
const string TEST_EXCEPT = "💥";

class Fail : public exception{
	const char * what () const throw (){
    return "uniq::Fail"; 
  }
};

//========================================================================= Test
struct Test : public Name {
  bool passed;
  string expr;
  string file;
  int line;
  exception ex;

  Test(bool passed, string expr, string func, string file, int line) : Name(func), passed(passed), expr(expr), file(file), line(line) {
    if (passed) {
      uniq::TEST_PASSED++;
      OUT(TEST_OK);
    } else {
      uniq::TEST_FAILED++;
      if(SILENT_TESTS) FAIL(ORA, split(name,'_').back(), " ");
      FAIL(TEST_FAIL, GRY,"(", BLD, RED, expr, GRY, ")", RST, "(", file, ":", line, ")\n");
      throw Fail();
    }
  };

  Test(const exception& ex, string func, string file, int line) : Name(func), ex(ex), file(file), line(line) {
    if (ex.what() == string("uniq::Fail")) return;
    uniq::TEST_EXCEPTION++;
    if(SILENT_TESTS) FAIL(ORA, split(func,'_').back());
    FAIL(TEST_EXCEPT, GRY,"(", RED,BLD,ex.what(), GRY,")", RST,"(",file,":",line,")\n");
  };
};

//================================================================== CHECK(expr)
#define CHECK(expr) uniq::Test((expr), #expr, __FUNCTION__, __FILE__, __LINE__)

//======================================================== CHECK_EXCEPTION(expr)
#define CHECK_EXCEPTION(expr)                                   \
  try {                                                         \
    (expr);                                                     \
    uniq::Test(false, #expr, __FUNCTION__, __FILE__, __LINE__); \
  } catch (std::exception & e) {                                \
    uniq::Test(true, #expr, __FUNCTION__, __FILE__, __LINE__);  \
  }

// #define TESTLOG(name)                                                      \
//   void test_##name(Log log=Log(name));                                     \
//   static uniq::TestCase test__##name(#name, &test_##name, __FILE__, __LINE__); \
//   void test_##name(Log log=Log(name))

//========================================================================= TestCase
struct TestCase;
vector<pair<string, TestCase*>> TestCases = {};

typedef void (*testFunc)();

struct TestCase: public Name {
  testFunc func;
  string file;
  int line;

  TestCase(string name, testFunc f, string file, int line) : Name(name), func(f), file(file), line(line) 
    { TestCases.push_back(make_pair(name, this)); }

  TestCase(string name, string file, int line) : Name(name), file(file), line(line) {}

  void run(){
    OUT(ORA, name, " ");
    try {
      auto t = CpuTime();
      func();
      t = t(CpuTime());
      if (t > MILI) OUT(GRN, " ", t); // show time if took > 1ms
      OUT(GRY, " (", GRY, split(file, '/').back(), ":", line, ")\n");
    } catch (const exception& e) {
      Test(e, name, file, line);
    };
  }
};

//=================================================================== runTests()
int runTests() {
  string line = GRY +  repeat("=", 80);
  OUT("Running tests...\n", line, "\n");

  for (auto [name, test] : TestCases)
    test->run();

  OUT(line,"\n");
  FAIL(TEST_OK," ", TEST_PASSED
    , TEST_FAILED ? sstr("  ",TEST_FAIL," ", TEST_FAILED) : ""
    , TEST_EXCEPTION ? sstr("  ", TEST_EXCEPT," ",TEST_EXCEPTION) : "", "\n" );

  TestCases.clear();
  return TEST_FAILED + TEST_EXCEPTION;
};

//======================================================================= TEST()
#define TEST(name)                                                             \
  void test_##name();                                                          \
  static uniq::TestCase test__##name(#name, &test_##name, __FILE__, __LINE__); \
  void test_##name()

// #else
//   #define CHECK(x) ((void)sizeof(#x))
//   #define CHECK_EXCEPTION(x) ((void)sizeof(#x))
//   #define TEST(x) void test_##x()
//   int runTests(){ return 0; }
// #endif

//=================================================================== TEST(TestCase)
TEST(Test) {
  CHECK(1 == 1);
  CHECK_EXCEPTION(throw exception());
  // CHECK(false); // to see a failure
  // throw exception(); // to see an exception
}
}  // namespace uniq

/*/ New testing model
struct TestCase_Profiler : public uniq::TestCase { 
  TestCase_Profiler(string name, string file, int line) : TestCase(name, file, line){}; 
  void enter(); 
}; 
static uniq::TestCase_Profiler testCase_Profiler("Profiler", "Profiler.h", 95); 
void TestCase_Profiler::enter(){ TT
  cout << "hello\n";
}//*/
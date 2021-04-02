#pragma once
#include "std.h"

string exception_message() // https://stackoverflow.com/a/3641809/9464885
{
  try { throw; }// rethrow_exception(eptr); }
  catch (const exception &e) { return e.what()   ; }
  catch (const string    &e) { return e          ; }
  catch (const char      *e) { return e          ; }
  catch (const int        i) { return to_string(i); }
  catch (const long       l) { return to_string(l); }
  catch (...)                { return "unknown exception"; }
}

void handle_exception(){
  cerr << exception_message() << "\n";  
}

#ifdef TESTING

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
    cout << TEST_PASSED << " passed ------------------------------\n";
  };

  #define CHECK(expr)               \
    if (expr) {                     \
      TEST_PASSED++;                \
    } else {                        \
      TEST_FAILED++;                \
      Test(#expr, __FILE__, __LINE__); \
    }

  #define CHECK_EXCEPTION(expr)        \
    try {                              \
      expr;                            \
      TEST_FAILED++;                   \
      Test(#expr, __FILE__, __LINE__); \
    } catch (std::exception & e) {     \
      TEST_PASSED++;                   \
    }
    
  #define TEST(name)                                                       \
    void test_##name();                                                    \
    static TestCase test__##name(#name, &test_##name, __FILE__, __LINE__); \
    void test_##name()

  #define TEST_CASE(name) TEST(name)

#else
  #define CHECK(x) ((void)sizeof(#x))
  #define CHECK_EXCEPTION(x) ((void)sizeof(#x))
  #define TEST(x) void test_##x()
  void runTests(){}
#endif

TEST(Test) {
  CHECK(1 == 1);
  CHECK_EXCEPTION(throw invalid_argument("no good"));
}

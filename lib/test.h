#pragma once
namespace uniq {
#ifdef TESTS

int TEST_PASSED = 0;
int TEST_FAILED = 0;
int TEST_EXCEPTION = 0;

//========================================================================= Test
struct Test {
  string state;
  string expr;
  string file;
  int line;
  double time;

  Test(string state, string expr = "", string file = "", int line = 0)
      : state(state), expr(expr), file(file), line(line) {}

  static map<string, string> glyphs;

  virtual string print();

  virtual void test(){ }
};

ostream& operator<<(ostream& os, Test& t) { os << t.print() << std::flush; return os;}

//===================================================================== TestSet
struct TestSet;
static vector<TestSet*> Tests = {};

struct TestSet : public Test {
  vector<unique_ptr<Test>> tests;
  bool errors = false;

  TestSet(string state, string expr, string file, int line) 
    : Test(state, expr, file, line) { Tests.push_back(this); }

  void add(string state, string expr, string file, int line) { // called by CHECK()
    if (state == "passed") TEST_PASSED++;
    else { if (state == "failed") TEST_FAILED++; else TEST_EXCEPTION++; errors = true; };
    tests.push_back(make_unique<Test>(state, expr, file, line)); 
  };

  virtual string print() override;
  virtual void test() override;
  virtual void testFunc() { } // overriden by TEST()
};

//======================================================================= TEST()
#define TEST(T)                                                   \
  struct TestSet_##T : public uniq::TestSet {                     \
    TestSet_##T(string state, string expr, string file, int line) \
      : TestSet(state, expr, file, line){};                       \
    void testFunc() override;                                     \
  };                                                              \
  static TestSet_##T TS_##T("testset", #T, __FILE__, __LINE__);   \
  void TestSet_##T::testFunc()

//================================================================== CHECK(expr)
#define CHECK(expr) add((expr)?"passed":"failed", #expr, __FILE__, __LINE__)

//======================================================== CHECK_EXCEPTION(expr)
#define CHECK_EXCEPTION(expr) \
  try { (expr); add("failed", #expr, __FUNCTION__, __LINE__); } \
  catch (std::exception & e) { add("passed", #expr, __FUNCTION__, __LINE__); }

#else
  #define CHECK(x) ((void)sizeof(#x))
  #define CHECK_EXCEPTION(x) ((void)sizeof(#x))
  #define TEST(x) void test_##x()
  int runTests(){ return 0; }
#endif // TESTS

//=================================================================== TEST(Test)
TEST(Test) {
  CHECK(1==1);
  CHECK_EXCEPTION(throw exception());
  // CHECK(1==2);  // to see a failure
  // throw exception(); // to see an exception  
  // CHECK_EXCEPTION([&]{ throw exception(); }() );  //to do: catch exceptions thrown inside lambdas
  // CHECK_EXCEPTION( assert(false) ); // dont catch
}
}// UniQ • Released under GPL 3 licence
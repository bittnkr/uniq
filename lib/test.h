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
  Time time;

  Test(string state, string expr = "", string file = "", int line = 0)
      : state(state), expr(expr), file(file), line(line) {}

  inline static map<string, string> glyphs = {
    {"ready", "."}, {"passed", BLD + GRN + "✓" + RST}, 
    {"failed", BLD + RED + "✘" + RST}, {"exception", "💥"}, {"testset", ""}
  };

  virtual string print() {
    if (!TESTS) return "";
    if (state == "passed") return TESTS>1 ? Test::glyphs[state] : ""; 
    return sstr("\n ", Test::glyphs[state], GRY, "(", RED,BLD, expr, GRY, ")", RST, " ", file, ":", line,"");
  }

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

  string print () override {
    if (!TESTS) return "";
    string s="";
    
    for (auto& t : tests) s += sstr(*t);

    if( TESTS>1 || errors ){
      s = sstr(ORA, Test::glyphs[state], expr, RST, " ") + s;
      if (time > MILI) s+=sstr(GRN, " ", time);
      s += errors? "\n" : sstr(GRY, " (", GRY, split(file, '/').back(), ":", line, ")\n");
    }
    return s;
  }

  virtual void test() override { 
    auto tm = CpuTime();
    try { 
      testFunc(); 
      for (auto& test : this->tests) 
        test->test();
    } catch(const std::exception& e) { add("exception", e.what(), file, line); }
    time = tm(CpuTime());
  }

  virtual void testFunc() { } // overriden by TEST()
};

// ==================================================================== RootTest
int runTests() {
  string r="\n", line = GRY + repeat("=", 80);

  for (auto &t : Tests) t->test();
  
  if(TESTS>1) r+=sstr(line,"\nRunning tests...\n", line, "\n");

  for (auto &t : Tests) r += t->print();

  if(TESTS>1) r+=sstr(line, "\n");
  if(TESTS>0) r+=sstr(Test::glyphs["passed"], " ", TEST_PASSED, TEST_FAILED ? sstr("  ", 
  Test::glyphs["failed"], " ", TEST_FAILED) : "", TEST_EXCEPTION ? sstr("  ", 
  Test::glyphs["exception"], " ", TEST_EXCEPTION) : "");

  cout << r << "\n" << flush;
  Tests.clear();
  return TEST_FAILED + TEST_EXCEPTION;
};

//======================================================================= TEST()
#define TEST(T)                                                   \
  struct TestSet_##T : public TestSet {                           \
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

  // todo: test exceptions inside lambdas
  // CHECK_EXCEPTION([&]{ throw exception(); }() ); 
  // CHECK_EXCEPTION( assert(false) );
}
}// UniQ • Released under GPL 3 licence
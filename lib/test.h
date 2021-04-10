#pragma once
namespace uniq {

// #ifdef TESTS
bool SILENT_TESTS = 0;  // omit successul tests
bool MUTE_TESTS = 0;    // omit even failures, just return success or failure

int TEST_COUNT = 0;
int TEST_PASSED = 0;
int TEST_FAILED = 0;
int TEST_EXCEPTION = 0;

map<string, string> glyphs = {{"ready", "."}, {"passed", BLD + GRN + "✓" + RST}, 
  {"failed", BLD + RED + "✘" + RST}, {"exception", " 💥 "}, {"testSet", ""}};

//========================================================================= Test
struct Test{  //}, State {
public:
  string state = "ready";
  string expr;
  string file;
  int line;
  Time time;

  Test(string state, string expr = "", string file = "", int line = 0)
      : state(state), expr(expr), file(file), line(line) {}

  virtual void test() { 
    if (state == "failed") TEST_FAILED++;
    else TEST_PASSED++;
  }

  virtual string print() {
    if (MUTE_TESTS) return "";
    string r = "";
    if (!SILENT_TESTS) r += glyphs[state];
    if (state == "passed") return r;
    r += sstr(GRY, "(", RED, BLD, expr, GRY, ")", RST, "(", file, ":", line, ")");
    return r;
  }
};

ostream& operator<<(ostream& os, Test& t) { os << t.print() << std::flush; return os;}

//===================================================================== TestSet
struct TestSet;

static vector<TestSet*> Tests = {};//("root", "RootTest", __FILE__, __LINE__);

struct TestSet : public Test {
public:
  vector<unique_ptr<Test>> tests;

  TestSet(string state, string expr, string file, int line) : Test(state, expr, file, line) { 
    Tests.push_back(this); 
  }

  void addtest(string state, string expr, string file, int line) {
    tests.push_back(make_unique<Test>(state, expr, file, line));
    (*tests.back()).test();
  };

  string print () override {
    if (SILENT_TESTS) return "";
    string r = sstr(ORA, glyphs[state], expr, RST, " ");
    // string r = sstr(ORA, glyphs[state], GRY, " (", expr ,")" , RST, " ");
    for (auto& test : tests)
      r += sstr(*test);
    if (time > MILI) r+=sstr(GRN, " ", time);  // show time if took > 1ms
    return r += sstr(GRY, " (", GRY, split(file, '/').back(), ":", line, ")");
  }
};

// ==================================================================== RootTest
int runTests() {
  string line = GRY + repeat("=", 80);

  string r = sstr(CLR, line,"\nRunning tests...\n", line, "\n");

  for (auto &test : Tests){
    auto time = CpuTime();
    try {
      test->test();
    } catch (const exception& e) {
      test->state = "exception";
      test->expr = e.what();
      TEST_EXCEPTION++;
    };
    test->time = time(CpuTime());
    r += test->print()+"\n";
  };
  r+=sstr(line, "\n");

  r+=sstr(glyphs["passed"], " ", TEST_PASSED, TEST_FAILED ? sstr("  ", 
  glyphs["failed"], " ", TEST_FAILED) : "", TEST_EXCEPTION ? sstr("  ", 
  glyphs["exception"], " ", TEST_EXCEPTION) : "");

  uniq::log(r);

  Tests.clear();
  return TEST_FAILED + TEST_EXCEPTION;
};

//======================================================================= TEST()
#define TEST(T)                                                   \
  struct TestSet_##T : public TestSet {                           \
    TestSet_##T(string state, string expr, string file, int line) \
        : TestSet(state, expr, file, line){};                     \
    void test() override;                                         \
  };                                                              \
  static TestSet_##T TS_##T("testset", #T, __FILE__, __LINE__);  \
  void TestSet_##T::test()
 
//   static bool test_TestSet_##T = Tests.addtest(TestSet_##T("testset", #T, __FILE__, __LINE__)); \
//   static bool test_TestSet_##T = TestFactory::Register(#name, &test_##name); \

//================================================================== CHECK(expr)
#define CHECK(expr) addtest(expr ? "passed":"failed", #expr, __FILE__, __LINE__)

//======================================================== CHECK_EXCEPTION(expr)
#define CHECK_EXCEPTION(expr)                         \
  try {                                               \
    (expr);                                           \
    addtest("failed", #expr, __FUNCTION__, __LINE__); \
  } catch (std::exception & e) {                      \
    addtest("passed", #expr, __FUNCTION__, __LINE__); \
  }
// #else
//   #define CHECK(x) ((void)sizeof(#x))
//   #define CHECK_EXCEPTION(x) ((void)sizeof(#x))
//   #define TEST(x) void test_##x()
//   int runTests(){ return 0; }
// #endif

//=================================================================== TEST(Test)
TEST(Test) {
  CHECK(1==1);
  CHECK(1==2);  // to see a failure
  CHECK_EXCEPTION(throw exception());
  CHECK(true);
}

TEST(Test2) {
  CHECK(1);
  throw exception(); // to see an exception
}

}  // namespace uniq
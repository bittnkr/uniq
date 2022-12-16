// Dependable tests, that cannot be with original code because dependencies 
//==============================================================================
// #include "uniq.h"
namespace uniq {

#ifdef TESTS

map<string, string> Test::glyphs = {
    {"ready", "."}, {"passed", BLD+GRN+"✓"+RST}, 
    {"failed", BLD+RED+"✘"+RST}, {"exception", "💥"}, {"testset", ""}
  };

string Test::print() {
  if (state == "passed") return TESTS>1 ? Test::glyphs[state] : ""; 
  return sstr("\n ", Test::glyphs[state], GRY, "(", RED,BLD, expr, GRY, ")", RST, " ", file, ":", line,"");
}

string TestSet::print() {
  string s = "";

  for (auto& t : tests)
    s += sstr(*t);

  if (TESTS > 1 || errors) {
    s = sstr(ORA, Test::glyphs[state], expr, RST, " ") + s;
    if (time > MILI)
      s += sstr(GRN, " ", Time(time));
    s += errors ? "\n" : sstr(GRY, " (", GRY, split(file, '/').back(), ":", line, ")\n");
  }
  return s;
}

void TestSet::test() {
  auto tm = CpuTime();
  try {
    testFunc();
    // pool().run(&TestSet::testFunc, this);
    // pool().wait();

    for (auto& test : this->tests)
      test->test();
      // run(&Test::test, this);  

    // pool().wait();
  } catch (const std::exception& e) {
    add("exception", e.what(), file, line);
  }
  time = tm(CpuTime());
}

// ==================================================================== RootTest
int runTests() {
  string r="\n", line = GRY + repeat("=", 60);

  for (auto &t : Tests) t->test();
  
  if(TESTS>1) r+=sstr(line,"\nRunning tests...\n", line, "\n");

  for (auto &t : Tests) r += t->print();

  if(TESTS>1) r+=sstr(line, "\n");
  
  r+=sstr( Test::glyphs["passed"], " ", TEST_PASSED, TEST_FAILED ? sstr("  ", 
           Test::glyphs["failed"], " ", TEST_FAILED) : "", TEST_EXCEPTION ? sstr("  ", 
           Test::glyphs["exception"], " ", TEST_EXCEPTION) : "");

  cout << r << "\n" << flush;
  Tests.clear();
  // pool().wait();
  return TEST_FAILED + TEST_EXCEPTION;
};

#endif // TESTS

}// UniQ • Released under GPL 3 licence
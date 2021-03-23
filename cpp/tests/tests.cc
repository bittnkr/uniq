// #include "memwatch.h"
#include "uniq.h"
// #include "Any.h" // simpler std:any 
// #include "Json.h" // Json primitive
// #include "Worker.h" // worker thread
// #include "Pool2.h" // thread pool
// #include "Digit.h" // big digit 
// #include "Number.h" // big number
// #include "Timer.h" // Time counting & benchmarks

int main(){
  // Tests().run();
  log("Running tests ------------------------------");  
  test(test_numtypes, "numtypes");
  test(test_terminal, "terminal");
  test(test_utils, "utils");
  test(test_call, "call");
  test(test_fs, "fs");
  test(test_sha256, "sha256");
  test(test_Time, "Time");
  test(test_Named, "Named");
  test(test_Atomic, "Atomic");
  test(test_Color, "Color");
  test(test_Actor, "Actor");
  test(test_Queue, "Queue");
  test(test_Pool, "Pool");

  // test(test_Any, "Any");

  // test(test_Pool2, "Pool2");
  // test(test_Worker, "Worker");
  // test(test_Event, "Event");
  // test(test_Timer, "Timer");

  // test(test_Json, "Json");
  // test(test_Digit, "Digit");
  // test(test_Number, "Number");

  // test(test_Ticker, "Ticker");
  log(TEST_CHECKS," tests passed------------------------------");

  quick_exit(0); // return 0;
}
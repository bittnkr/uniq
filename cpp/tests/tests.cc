// #include "memwatch.h"
#include "uniq.h"

int main(){
  log("Running tests ------------------------------");

  test(test_numtypes, "numtypes");
  test(test_terminal, "terminal");
  test(test_utils, "utils");
  test(test_call, "call");
  test(test_fs, "fs");
  test(test_sha256, "sha256");
  test(test_Time, "Time");
  test(test_Named, "Named");
  test(test_Atom, "Atom");
  test(test_Color, "Color");
  test(test_Actor, "Actor");
  test(test_Queue, "Queue");
  // test(test_Worker, "Worker");
  test(test_Pool, "Pool");
  // test(test_Ticker, "Ticker");
  // test(test_Digit, "Digit");
  // test(test_Event, "Event");
  // test(test_Timer, "Timer");

  log(TEST_CHECKS," tests passed------------------------------");

  quick_exit(0); // return 0;
}
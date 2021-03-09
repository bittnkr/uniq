#include "uniq.h"
using namespace uniq;

int main(){
  log("Running tests ------------------------------");

  test(test_numtypes, "numtypes");
  test(test_atom, "atom");
  test(test_utils, "utils");
  test(test_terminal, "terminal");
  test(test_actor, "actor");
  test(test_queue, "queue");
  test(test_pool, "pool");
  // test(test_time, "time");

  log(TEST_CHECKS," tests passed------------------------------");

  quick_exit(0); // return 0;
}
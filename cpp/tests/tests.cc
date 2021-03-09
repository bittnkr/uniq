#include "uniq.h"
using namespace uniq;

int main(){
  cout << "Running tests ------------------------------" << endl;

  test(test_numtypes, "numtypes");
  test(test_atom, "atom");
  test(test_utils, "utils");
  test(test_terminal, "terminal");
  test(test_actor, "actor");
  test(test_queue, "queue");
  test(test_pool, "pool");

  cout << TEST_CHECKS << " tests passed" << "------------------------------"<< endl;
  // cout.flush();
  quick_exit(0); // return 0;
}
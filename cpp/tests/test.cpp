#include "uniq.h"

int main(){
  cout << "Running tests ------------------------------" << endl;

  runtest(test_numtypes, "numtypes");
  runtest(test_utils, "utils");
  runtest(uniq::test_queue, "queue");
  runtest(uniq::test_pool, "pool");

  cout << TEST_CHECKS << " tests passed" << "------------------------------"<< endl;
  return 0;
}
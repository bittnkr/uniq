#include "common.h"
#include "queue.h"
#include "pool.h"
#include "numtypes.h"

int main(){
  cout << "Running tests ------------------------------" << endl;

  run_test(test_queue, "queue"); 
  run_test(test_pool, "pool"); 
  run_test(test_numtypes, "numtypes");  

  cout << TEST_CHECKS << " tests passed" << "------------------------------"<< endl;
  return 1;
}

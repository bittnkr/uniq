#include "common.h"
#include "numtypes.h"
#include "utils.h"
#include "boilerplate.h"
#include "queue.h"
#include "pool.h"

int main(){
  using namespace test;
  cout << "Running tests ------------------------------" << endl;

  runtest(test_numtypes, "numtypes");  
  runtest(test_utils, "utils");  
  runtest(test_boilerplate, "boilerplate");  
  runtest(test_queue, "queue"); 
  runtest(test_pool, "pool"); 

  cout << CHECKS << " tests passed" << "------------------------------"<< endl;
  return 1;
}

#include "uniq.h"
#include "boilerplate.cpp"
// #include "mrmw.cpp"

int main(){
  using namespace test;
  cout << "Running tests ------------------------------" << endl;

  runtest(test_numtypes, "numtypes");  
  runtest(test_utils, "utils");  
  runtest(test_boilerplate, "boilerplate");  
  runtest(test_queue, "queue"); 
  // runtest(test_mrmw, "mrmw"); 
  runtest(test_pool, "pool"); 

  cout << TEST_CHECKS << " tests passed" << "------------------------------"<< endl;
  return 0; //quick_exit(0);
}

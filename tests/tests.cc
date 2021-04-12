// #include "backward.hpp"
// namespace backward { backward::SignalHandling sh; }

#define TESTS 1 // unit testing (0: MUTE, 1: SILENT, 2: NORMAL, 3 VERBOSE)
// #define TRACE 0; // function call execution time tracing 

#include "uniq.h"
using namespace uniq;

int main(){
  // set_terminate([]{ log("Exception"); abort(); });
  int r = runTests(); 
  quick_exit(r);
}
// #include "backward.hpp"
// namespace backward { backward::SignalHandling sh; }

#define TESTS 2 // unit testing 0: mute, 1: just bad news, 2: normal
// #define TRACE 0; // execution time tracing 

#include "uniq.h"
using namespace uniq;

int main(){
  // set_terminate([]{ log("Exception"); abort(); });
  int r = runTests(); 
  quick_exit(r);
}
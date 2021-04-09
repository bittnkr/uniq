// #include "backward.hpp"
// namespace backward { backward::SignalHandling sh; }

#define TESTING 1 // Comment to disable tests
#include "uniq.h"

int main(){
  uniq::SILENT_TESTS = 1; // omit success report
  int r = uniq::runTests();
  quick_exit(r);
}
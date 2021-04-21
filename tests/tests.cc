#define TESTS 2 // unit testing 0: mute, 1: just bad news, 2: normal

#include "uniq.h"
using namespace uniq;

int main(){

  int r = runTests(); 

  quick_exit(r);
}
// Hello world for UniQ library
// compile using ./build sayHello
#include "uniq.h"
using namespace uniq;

void sayHello(string msg, int i) { 

  if (i % 100'000 == 0)
    log("Hello ", msg, " #", i);

  if(!i) pool().stop();
}

int main() { 
  log("Say Hello in paralel a million times...");

  pool().start();
  pool().showstats = true;

  for (int i = 1e6; i>=0 ; i--)
    run(sayHello, "World", i);
 
  pool().join();
  log("\n");
  return 0;
}
//UniQ • Released under GPL 3 licence
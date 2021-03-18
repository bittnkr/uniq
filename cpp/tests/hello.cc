// Hello world for uniq library
// compile using ./build hello
#include "uniq.h"

// say hello in paralel a million times
void hello(string s, int counter) {

  if (counter % 100000 == 0)
    log(counter, " Hello ", s);

  if(counter>0) run(hello, s, counter-1);
  else pool.stop();
}

int main() { 
  pool.showstats = true;
  
  run(hello, "World", 1e6);
  
  pool.join();
  
  return 0;
}
//uniq • Released under GPL 3.0
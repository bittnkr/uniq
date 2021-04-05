// Hello world for uniq library
// compile using ./build hello
#include "uniq.h"

void hello(string s, int N) { 

  if (N % 100000 == 0){
    log("Hello ", s, " ", N);

    for (auto i = N-1; i > N-100001; i--){
      run(hello, s, i);
    }
  }

  if(N<=0) pool().stop();
}

int main() { 
  pool().showstats = true;
  
  run(hello, "World", 1e6); // say hello in paralel a million times
  
  pool().join();
  
  return 0;
}
//uniq • Released under GPL 3.0

// void hello(string s, int N) {
//   for(auto i=0; i < N-1; i++)
//     run(hello, s, i);
//   if (N % 100 == 0)
//     log(N, " Hello ", s);
//   if(N==0){
//     pool().showstats = true;
//     pool().stop();
//   }
// }
// int main() { 
//   run(hello, "World", 1000);
//   pool().join();
// }
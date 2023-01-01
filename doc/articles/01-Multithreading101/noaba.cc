#include "std.h"
int main(){
    
  atomic<int> V {0};
 
  thread A([&]{ for(auto i =0; i < 100e6; i++) V++; });
  thread B([&]{ for(auto i =0; i < 100e6; i++) V--; });

  A.join();
  B.join();

  cout << V << "\n";
}

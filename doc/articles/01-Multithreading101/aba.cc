#include <thread>
#include <iostream>
using namespace std;

int main(){
  
  int V = 0;
 
  thread A([&]{ for(auto i =0; i <100e6; i++) V++; });
  thread B([&]{ for(auto i =0; i <100e6; i++) V--; });

  A.join(); B.join();

  cout << V << "\n";
};
// build using c++ aba.cpp -lpthread -std=c++11 -o aba && ./aba

#include "std.h"

int main(int argc, char *argv[]) {

  int pairs = argc>1 ? atoi(argv[1]) : 1;
  cout << "aba test with "<< pairs*2 <<" concurrent threads\n";

  int V = 0;

  vector<thread> pool;
  for (auto i = 0; i < pairs; i++) {
    pool.push_back(thread([&]{ for(auto i =0; i < 100e6; i++) V++; }));
    pool.push_back(thread([&]{ for(auto i =0; i < 100e6; i++) V--; }));
  };

  for (auto &t : pool) t.join(); // Wait termination

  cout << "\nchecksum: "<< V << "\n";
}

// producer/consumer test for UniQ Library
// See test.cpp for use case & README.md for details about the solution
// compile using ./build hello

#include "std.h"
#include "./queue.h"

atomic<long> total{0}; // a checksum, to ensure that all count pushed are poped
// long total{0}; // a checksum, to ensure that all count pushed are poped
int Items = 10'000'000; // how many items will flow trough the queue 
int pairs = 4; // how many producers and consumers pool will be used

const string RST("\033[0m");
string RED(string s) {return "\033[31m"+s+RST;};
string GRN(string s) {return "\033[32m"+s+RST;};

Queue Q; // Q(1); // stress test using a single position queue

void producer(int items) // pushes data into the queue
{
  int v; long sum = 0;

  for (int i = 1; i <= items; i++) {
    Q.push(i);
    sum += 1;//i;
    if(Items<=1000) cout << (GRN(to_string(i))+" ");
  }
  Q.push(-1); // signal termination with -1

  total += sum;
  cout << GRN("\nProduced: "+to_string(sum));
}

void consumer() // takes data from the queue
{
  int v; long sum = 0;

  while ((v = Q.pop()) > 0){
    sum += 1;//v;
    if(Items<=1000) cout << (RED(to_string(v))+" ");
  }

  total -= sum;
  cout << RED("\nConsumed: "+to_string(sum));
}

int main() {
  total = 0;
  cout << "Creating "<< pairs*2 <<" producers & consumers to flow "<< Items <<" items trough the queue.\n";

  vector<thread> pool;
  for (auto i = 0; i < pairs; i++) {
    pool.push_back(thread(consumer));
    pool.push_back(thread(producer, Items / pairs));
  }
  for (auto &t : pool) t.join();

  cout << "\n\nChecksum: "<< size_t(total) <<" (it must be zero)\n";  
  return 0;
}// UniQ • Released under GPL 3 licence

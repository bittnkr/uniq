// basic producer/consumer multi reader multi writer test for UniQ Library
// this is the evolution of nutshell/main.cc using the official queue
// but yet using std:: thdread for an example using run() function see mrmw2.cc
// compile using ./build mrmw

#include "std.h"
#include "uniq.h"
using namespace uniq;

Atomic<long> total(0); // a checksum, to ensure that all count pushed are poped

Queue<int> Q; // using the default 64 positions
// Queue<int> Q(1); // stress test using a single position queue

void producer(int items) // pushes data into the queue
{
  int v; long sum = 0;

  for (int i = 1; i <= items; i++) {
    Q.push(i);
    sum += i;
  }
  Q.push(-1); // signal termination with -1

  total += sum;
  log(GRN, "Produced: ", sum);
}

void consumer() // takes data from the queue
{
  int v; long sum = 0;

  while (Q.pop(v) && v != -1)
    sum += v;

  total -= sum;
  log(RED, "Consumed: ", sum);
}

int main(){
  total = 0;

  long Items = 10'000'000;  // how many items each producer will push into the queue
  int pairs = thread::hardware_concurrency()/2;

  log("Creating ", pairs*2, " producers & consumers to flow ", Items, " items trough the queue.\n");

  vector<thread> pool;
  for (auto i = 0; i < pairs; i++) {
    pool.push_back(thread(consumer));
    pool.push_back(thread(producer, Items));
  }
   for (auto &t : pool) t.join(); // Wait termination

  log("\nChecksum: ", total.value, "\ntasks: ", Q.counter());
  return 0;
}
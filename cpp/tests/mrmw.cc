// basic producer/consumer multi reader multi writer test for uniQ Library
// this test was the first working proof of the ideia
// for a more practical use of the queue look at mrmw2.cc
// compile using ./build mrmw

#include "std.h"
#include "queue.h"
#include "atom.h"

// atomic_long total;   
uniq::atom<long> total=0; // a checksum, to ensure that all count pushed are poped

uniq::Queue<int> Q; // using the default 64 positions
// Q(1); // stress test using a single position queue
// Q(64*1024); // performance using a 64k queue

void producer(int items) // pushes data into the queue
{
  int v; long sum = 0;

  for (int i = 1; i <= items; i++) {
    Q.push(i);
    sum += i;
  }
  Q.push(-1); // signal termination with -1
  total += sum;
  printf("Produced: %ld\n", sum);
}

void consumer() // takes data from the queue
{
  int v; long sum = 0;

  while (Q.pop(v) && v != -1)
    sum += v;

  total -= sum;
  printf("Consumed: %ld\n", sum);
}

int main(){
  total = 0;
  printf("\nChecksum: %ld (it must be zero)\n", total.value);
  long Items = 1000*1000;  // how many items each producer will push into the queue
  int pairs = thread::hardware_concurrency()/2;

  printf("Creating %d producers & %d consumers\n", pairs, pairs);
  printf("to flow %ld items through the queue.\n\n", Items*pairs);

  vector<thread> pool;
  for (auto i = 0; i < pairs; i++) {
    pool.push_back(thread(consumer));
    pool.push_back(thread(producer, Items));
  }
 
  for (auto &t : pool) t.join(); // Wait termination

  printf("\nChecksum: %ld (it must be zero)\n", total.value);
  printf("\ntasks: %d\n", Q.done());
  return 0; // quick_exit(0);
}
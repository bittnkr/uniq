// producer/consumer test for uniQ Library
// See README.md for details
// compile using ./build test

#include "uniq.h"
using namespace uniq;

Queue<int> Q;
atom<long> total = 0;

void producer(int items)  // pushes data into the queue
{
  long sum = 0;
  for (int i = 1; i <= items; i++) {
    Q.push(i);
    sum += i;
  }
  Q.push(-1);
  // pool.atomic([&](){ total += sum; })
  total += sum;
  printf("Produced: %lu\n", sum);
}

atomic<long> consumed(0);
void consumer()  // takes data from the queue
{
  int v; long sum = 0;
  while (Q.pop(v) && v != -1)
    sum += v;
  total -= sum; 
  printf("Consumed: %lu\n", sum);

  if(Q.empty() && total==0) pool.stop();
}

int main() {
  int Workers = 4; // 4 producers & 4 consumers
  int Items = 1e6; // how many items will flow trough the queue

  printf("Creating %d producers & consumers\n", Workers);
  printf("to flow %d items through the queue.\n\n", Items);

  uniq::pool.start(); // todo: remove this

  for (int i = 0; i < Workers; i++) {
    run(consumer);
    run(producer, Items);
  }

  pool.join();
  // uniq::pool.stop();  // todo: remove this

  printf("\nChecksum: %ld (it must be zero)\n", total.value);
  printf("\nmessages: %'d", pool.done());
  quick_exit(0); // return 0;
}
//uniq • Released under GPL 3.0
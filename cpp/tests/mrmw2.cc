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
  logn(GRN,"+");
  // log("Produced: ", sum);
}

atomic<long> consumed(0);
void consumer()  // takes data from the queue
{
  int v; long sum = 0;
  while (Q.pop(v) && v != -1)
    sum += v;
  total -= sum; 
  logn(RED,"-");
  // log("Consumed: ", sum);

  if(Q.empty() && total==0) pool.stop();
}

int main() {
  int Workers = 1000; // 100 producers & 1000 consumers
  int Items = 10000; // each flowing 100k items trough the queue

  pool.showstats = true;

  log("Creating ",Workers," producers & consumers to flow ",Items*Workers," items through the queue.");

  for (int i = 0; i < Workers; i++) {
    run(consumer);
    run(producer, Items);
  }

  pool.join();

  log("\nChecksum: ",total," (it must be zero)");
  log("tasks: ", pool.todo.done() );
  log("messages: ", Q.done() );
  quick_exit(0); // return 0;
}
//uniq • Released under GPL 3.0
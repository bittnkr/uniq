// producer/consumer test for
// See README.md for details
// compile using ./build test

#include "uniq.h"
using namespace uniq;

Queue<int> Q;
Atomic<long> total = 0;

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
  out(GRN,"+");
  // log("Produced: ", sum);
}

atomic<long> consumed(0);
void consumer()  // takes data from the queue
{
  int v; long sum = 0;
  while (Q.pop(v) && v != -1)
    sum += v;
  total -= sum; 
  out(RED,"-");
  // log("Consumed: ", sum);

  if(total<=0 && pool().empty()) 
    pool().stop();
}

int main() {
  int Workers = 1000; // 1000 producers & 1000 consumers
  int Items = 1000; // each flowing 1000 items trough the queue

  pool().showstats = true;

  log("Creating ",Workers," producers & consumers to flow ",Items*Workers," items through the queue.");

  for (int i = 0; i < Workers; i++) {
    run(consumer);
    run(producer, Items);
  }

  pool().join();

  log("\nChecksum: ",total," (it must be zero)");
  log("tasks: ", pool().counter() );
  log("messages: ", Q.counter() );
  return 0;
}
//uniq
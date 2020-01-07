#include <stdio.h>
#include <locale.h>
#include <thread>
#include <vector>
#include <atomic>
#include "uniq.h"

using namespace std;

int Threads = 8;           // 4 producers & 4 consumers
int Items = 10*1000*1000;  // how many items will flow trough the queue 
atomic<int> Total(0);      // a checsum, to ensure that all items pushed are poped

Queue<int64> Q(4); // using the default 64 positions 
// Queue<int64> Q(1); // stress test using a single position queue
// Queue<int64> Q(64*1024); // performance using a 64k queue 


void producer(int items) // this is the producer thread, it pushes data into the queue
{
  int v, sum=0;

  for(int i = 1; i <= items; i++)
    sum += Q.push(1); // push the value 1 to the qeue

  Q.push(-1); // signalize termination with a -1
  Total += sum;
  printf ("Produced: %'9d\n", sum);
}


void consumer() // the consumer thread, takes data from the queue
{
  int v, sum = 0;

  
  while((v = Q.pop()) > 0)
    sum -= v;

  Total += sum;
  printf("Consumed: %'9d\n", -sum);
}

int main()
{
    int pairs = Threads/2;
    
    vector<thread> producers(pairs);
    vector<thread> consumers(pairs);

    setlocale(LC_NUMERIC, "");
    printf("Creating %d producers & %d consumers\n", pairs, pairs);
    printf("to flow %'d items trough the queue.\n\n", Items);

    for(int i=0; i < pairs; i++)
    {
      consumers[i] = thread(consumer);
      producers[i] = thread(producer, Items/pairs);
    }

    // Wait consumers finish the job
    for(int i=0; i < pairs; i++){
      producers[i].join();
      consumers[i].join();
    }
    printf("\nChecksum: %d (it must be zero)\n", int(Total));
}
// Part of uniQ library released under GNU 3.0 
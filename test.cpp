#include <stdio.h>
#include <locale.h>
#include <thread>
#include <vector>
#include <atomic>
#include "uniq.h"

using namespace std;
int Threads = 8;
int Items = 10*1000*1000;
atomic<int> Total(0);

Queue<int64> Q;//(64*1024);

void producer(int items)
{
  int v, sum=0;
  for(int i = 1; i <= items; i++)
    sum += Q.push(1);    
  Q.push(-1); // signalize termination
  Total += sum;
  printf ("Produced: %9d %d\n", sum, Q.flows());
}

void consumer()
{
  int v, sum = 0;
  while((v = Q.pop()) > 0)
    sum -= v;
  Total += sum;
  printf("Consumed: %'9d: \t Tasks: %'d\n", -sum, Q.taskid());
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
    printf("Total.....: %'9d (it must be zero)\n", int(Total));
}
// uniq::send(ping, 10)

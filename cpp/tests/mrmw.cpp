// producer/consumer test for uniQ Library
// compile using ./build mrmw

#include "common.h"
#include "queue.h"

typedef long long Int64;

int Threads = 4;           // 4 producers & 4 consumers
int Items = 10*1000*1000;  // how many items will flow trough the queue 
atomic<Int64> Total(0); // a checksum, to ensure that all count pushed are poped

uniq::Queue<Int64> Q; // using the default 64 positions
// Queue<Int64> Q(1); // stress test using a single position queue
// Queue<Int64> Q(64*1024); // performance using a 64k queue

void producer(int items) // pushes data into the queue
{
  Int64 v, sum = 0;

  for (int i = 1; i <= items; i++) {
    Q.push(i); 
    sum += i;
  }
  Q.push(-1); // signal termination with -1
  Total += sum;
  printf("Produced: %'llu\n", sum);
}

void consumer() // takes data from the queue
{
  Int64 v, sum = 0;

  while ((v = Q.pop()) != -1)
    sum += v;

  Total -= sum;
  printf("Consumed: %'llu\n", sum);
}

void test_mrmw() {
  int pairs = 2;

  vector<thread> producers(pairs);
  vector<thread> consumers(pairs);

  setlocale(LC_NUMERIC, "");
  printf("Creating %d producers & %d consumers\n", pairs, pairs);
  printf("to flow %'d items through the queue.\n\n", Items);

  for (int i = 0; i < pairs; i++) {
    consumers[i] = thread(consumer);
    producers[i] = thread(producer, Items / pairs);
  }

  // Wait consumers finish the job
  for (int i = 0; i < pairs; i++) {
    producers[i].join();
    consumers[i].join();
  }

  CHECK(Total==0);

  printf("\nChecksum: %llu (it must be zero)\n", Int64(Total));
  printf("\ntasks: %d", Q.nextJobId());
}

int main(){ test_mrmw(); return 1; }

// Part of uniQ library released under GNU 3.0
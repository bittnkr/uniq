// basic producer/consumer multi reader multi writer test for uniQ Library
// compile using ./build mrmw

#include "std.h"
using namespace std;
#include "queue.h"

typedef long long Int64;

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

int main(){
  int Items = 1000*1000;  // how many items each producer will push into the queue
  int pairs = thread::hardware_concurrency()/2;

  setlocale(LC_NUMERIC, "");
  printf("Creating %d producers & %d consumers\n", pairs, pairs);
  printf("to flow %'d items through the queue.\n\n", Items*pairs);

  vector<thread> pool;
  for (auto i = 0; i < pairs; i++) {
    pool.push_back(thread(consumer));
    pool.push_back(thread(producer, Items));
  }

  // Wait consumers finish the job
  for (auto i = 0; i < pool.size(); i++) pool[i].join(); // for (auto t : pool) t.join();

  printf("\nChecksum: %llu (it must be zero)\n", Int64(Total));
  printf("\ntasks: %d\n", Q.counter());
}
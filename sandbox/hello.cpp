using namespace std;
#include <pthread.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <atomic>

inline bool CompareAndSwap(int *destination, int currentValue, int newValue) {
  return __sync_bool_compare_and_swap(destination, currentValue, newValue);
}

class Queue {
private:
  int _size, _head, _tail, mask;
  vector<int> buffer;
  vector<int> isfree;

protected:
  int tail() {
    while (full())
      sched_yield();
    return _tail;
  }

  int head() {
    while (empty())
      sched_yield();
    return _head;
  }

public:
  Queue(int size = 32) {
    _size = size;
    _tail = _head = 0;
    mask = _size - 1;
    buffer = vector<int>(_size, 0);
    isfree = vector<int>(_size, 1);
  }
  int size() { return _size; }
  int count() { return _tail - _head; }
  int empty() { return _tail == _head; }
  int full() { return _size == count(); }

  int push(int item) {
    int t;

    do
      t = tail();
    while (!isfree[t & mask] || !CompareAndSwap(&_tail, t, t + 1));

    isfree[t &= mask] = 0;
    buffer[t] = item;
    return t;
  }

  int pop() {
    int h;

    do
      h = head();
    while (isfree[h & mask] || !CompareAndSwap(&_head, h, h + 1));

    int r = buffer[h &= mask];
    isfree[h] = 1;
    return r;
  }
};

Queue Q;
atomic<int> Total(0); // a checsum, to ensure that all items pushed are poped

// this is the producer thread, it pushes data into the queue
void producer(int items) 
{
  int v;

  for (int i = 1; i <= items; i++)
    Q.push(1); // push the value 1 to the qeue

  Q.push(-1); // signalize termination with a -1
  Total += items;
  printf("Produced: %'9d\n", items);
}

void consumer() // the consumer thread, takes data from the queue
{
  int v, sum = 0;

  while ((v = Q.pop()) > 0)
    sum += v;

  Total -= sum;
  printf("Consumed: %'9d\n", sum);
}

int main() {
  int pairs = 4, Items = 10*1000*1000;
  // pthread_t p[pairs], c[pairs];
  vector<thread> producers(pairs);
  vector<thread> consumers(pairs);

  for (int i = 0; i < pairs; i++) {
    consumers[i] = thread(consumer);
    producers[i] = thread(producer, Items / pairs);
  }

  // Wait consumers finish the job
  for (int i = 0; i < pairs; i++) {
    producers[i].join();
    consumers[i].join();
  }
  printf("\nChecksum: %d (it must be zero)\n", int(Total));

  return 0;
}
// Part of uniQ library released under GNU 3.0
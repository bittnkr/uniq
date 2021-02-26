// UniQ - The Lock Free Queue.
#pragma once
namespace uniq {
#include "std.h"
using namespace std;

template <class T>
class Queue {
 protected:
  vector<T> buffer;
  int mask = 1;
  atomic<int> in, out;
  vector<int> isFree;

 public:
  Queue(int size_ = 64) {
    while (mask < size_) mask *= 2;
    buffer = vector<T>(mask, 0);
    isFree = vector<int>(size_, 1);
    mask--;

    out = in = 0;
    running = true;
  }

  int push(T item) {
    int i;
    do {
      i = in;
      if (!running) return 0;
      while (i - out > mask && running) sched_yield();  // if full, wait for space
    } while (!isFree[i & mask] || !in.compare_exchange_weak(i, i + 1));

    buffer[i & mask] = item;
    isFree[i & mask] = 0;
    return i;
  }

  T pop() {
    int o;
    do {
      if (!running) return T{};
      o = out;
      while (o == in && running) sched_yield();  // if empty, wait for item
    } while (isFree[o & mask] || !out.compare_exchange_weak(o, o + 1));

    T r = buffer[o &= mask];
    isFree[o] = 1;
    return r;
  }

  bool running = false;
  int size() { return mask + 1; }
  inline bool full() { return (in - out) > mask; }
  inline bool empty() { return out == in; }
  int counter() { return out - 1; }
  inline void wait(int id) { while(out < id) sched_yield(); }
};

// ================================================== tests
#include "test.h"
void test_queue(){

  uniq::Queue<int> q(1);
  atomic<int> produced(0);
  atomic<int> consumed(0);
  vector<thread> threads;

  for (int i = 0; i < 3; i++) { // 4 producers & 4 consumers

    // producer
    threads.push_back(thread([&](int items){
      for (int i = 1; i <= items; i++) {
        q.push(i);
        produced++;
      };
      q.push(-1);
    }, random() % 1000));

    // consumer
    threads.push_back(thread([&](){
      int v;
      while ((v = q.pop()) != -1) // while (q.pop(v) && v != -1) {
        consumed ++;
    }));
  };

  for (auto i = 0; i < threads.size(); i++)
    threads[i].join();

  CHECK(produced > 0);
  CHECK(produced == consumed);
}
}// uniq • Released under GPL 3.0
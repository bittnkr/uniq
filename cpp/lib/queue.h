// UniQ - The Lock Free Queue.
#pragma once
namespace uniq {
#include "std.h"

#define WAIT(condition) while(!(condition)) { sched_yield(); }

template <typename T> class Queue {
 protected:
  vector<T> buffer;
  vector<char> isfree;
  atomic<int> in, out;
  int mask = 1;

 public:
  Queue(int size_ = 64) {
    while (mask < size_) mask *= 2;
    buffer = vector<T>(mask, 0);
    isfree = vector<char>(mask, 1); // bool produces UB
    out = in = -1; // start in overflow
    mask--; // 01000000 => 00111111
    running = true;
  }

  int push(T item) {
    // assert(allow(item) && "uniq::queue::push(): Not allowed.");
    int i;
    do {
      i = in;
      WAIT(i - out <= mask || !running); // if full, wait for space
      if (!running) return 0;
    } while (!isfree[i & mask] || !in.compare_exchange_weak(i, i + 1) || !i);
    // string s = sstr("i[",sched_getcpu(),"] ",i," p ",(i&mask)," item ",item,"\n"); cout<<s;
    buffer[i & mask] = item;
    isfree[i & mask] = 0;
    return i;
  }

  int pop(T &item) {
    int o;
    do {
      do { o = out; } while (!o && !out.compare_exchange_weak(o,1)); // skip zero
      WAIT(o < in || !running); // if empty, wait for item
      if (!running) return 0;
    } while (isfree[o & mask] || !out.compare_exchange_weak(o, o + 1));
    // string s = sstr("o[",sched_getcpu(),"] ",o," p ",(o&mask)," item ",item,"\n"); cout<<s;
    item = buffer[o & mask];
    isfree[o & mask] = 1;
    return o;
  }

  bool running = false;
  int size() { return mask + 1; }
  inline bool full() { return (in - out) > mask; }
  inline bool empty() { return out == in; }
  int done() { return out-1; }
  inline void wait(int c) { while(out < c) sched_yield(); }
  // virtual bool allow(T item) { return true; }
};

// ======================================================================== test
#include "test.h"
void test_queue(){

  Queue<int> q(1);  
  vector<thread> threads;

  atomic<int> produced(0);
  auto producer = [&produced, &q](int N){
    int i = 0;
    while( ++i <= N && q.push(i) ) 
      produced += i;
    q.push(-1);
  };

  atomic<int> consumed(0);
  auto consumer = [&consumed, &q](){
    int v;
    while (q.pop(v) && v != -1)
      consumed += v;
  };

  for (int i = 0; i < (random()%8); i++) { // random producers & consumers
    threads.push_back(thread(consumer));
    threads.push_back(thread(producer, random()%100000));
  };

  for (auto &t : threads) t.join();

  CHECK(produced > 0);
  CHECK(produced == consumed);
}
}// uniq • Released under GPL 3.0
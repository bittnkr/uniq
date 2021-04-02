//==============================================================================
// Queue • A lock free multi-reader multi-writer circular buffered queue.
//==============================================================================
#pragma once
namespace uniq {
#include "std.h"

void sleep() { sched_yield(); }
void sleep(int ms) {this_thread::sleep_for(chrono::milliseconds(ms));}

#define WAIT(condition) while(!(condition)) { sched_yield(); }

template <typename T> class Queue : public Actor {

  vector<T> buffer;
  vector<char> isfree;
  atomic<int> in, out;
  int mask = 1;

 public:
  Queue(int size_ = 64) {
    while (mask < size_) mask *= 2;
    buffer = vector<T>(mask, 0);
    isfree = vector<char>(mask, 1);
    out = in = -1; // start in overflow
    mask--; // 01000000 => 00111111
    running = true;
  }
  ~Queue(){ 
    // log("~Queue()");
  }

  int push(const T &item, bool wait=true) {
    int i;
    do {
      i = in;

      if((full(i) && !wait) || !running) return 0;
      else WAIT(!full(i));

    } while (!isfree[i & mask] || !in.compare_exchange_weak(i, i + 1) || !i);

    buffer[i & mask] = item;
    isfree[i & mask] = 0;
    return i;
  }

  int pop(T &item, bool wait=true) {
    int o;
    do {
      do { o = out; } while (!o && !out.compare_exchange_weak(o,1)); // skip zero

      if((empty(o) && !wait) || !running) return 0;
      else WAIT(!empty(o));

    } while (isfree[o & mask] || !out.compare_exchange_weak(o, o + 1));

    item = buffer[o & mask];
    isfree[o & mask] = 1;
    return o;
  }

  inline bool full(int i = -1) { 
    if (i<0) i = in; 
    i = (i - out) > mask; 
    if(i) onfull();
    return i;
  }
  virtual void onfull(){ }

  inline bool empty(int o = -1) { 
    if (o<0) o = out; 
    o = o == in;
    if(o) onempty();
    return o; 
  }
  virtual void onempty(){ }

  int size() { return mask + 1; }
  int done() { return out-1; }
  inline void wait(int c) { while(out < c) sched_yield(); }
};

// ======================================================================== test
#include "test.h"
TEST(Queue){
  auto t = CpuTime();
  
  Queue<int> q;
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

  for (int i = 0; i < thread::hardware_concurrency()/2; i++) {
    threads.push_back(thread(consumer));
    threads.push_back(thread(producer, 1000));
  };

  for (auto &t : threads) t.join();

  CHECK(produced != 0);
  CHECK(produced == consumed);
  // log("Queue:", double(CpuTime(t)));
}
}// uniq • Released under GPL 3.0
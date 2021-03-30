// ElasticQueue - A Thread-safe Queue using mutexes and condition variables.
// Used for speed comparison. https://stackoverflow.com/a/16075550/9464885
#pragma once
namespace uniq {
#include "uniq.h"

template <typename T> class ElasticQueue : public Actor {
  queue<T> q;
  mutable mutex m;
  condition_variable c;
  atomic<int> in, out;
  int maxsize=0;
 public:
  
  ElasticQueue(int maxsize = 64) : maxsize(maxsize) {
    out = in = 1;
    Actor();
  }

  int push(T item, bool wait=true) {
    WAIT(q.size() < maxsize); // 13% slower
    lock_guard<mutex> lock(m);
    q.push(item);
    c.notify_one();
    return in++;
  }

  int pop(T &item, bool wait=true) {
    unique_lock<mutex> lock(m);
    c.wait(lock, [&]{ return !q.empty(); } );
    item = q.front();
    q.pop();
    return out++; 
  }

  inline bool full() { return (in-out) >= maxsize; }
  inline bool empty() { return q.empty; }
  inline int size() { return in-out; }
  inline int done() { return out-1; }
};

// ======================================================================== test
#include "test.h"
void test_ElasticQueue(){
  auto t = CpuTime();

  ElasticQueue<int> q;  
  vector<thread> threads;

  atomic<int> produced(0);
  auto producer = [&produced, &q](int N){
    int i = 0;
    while( ++i <= N && q.push(i) ) 
      produced++;
    q.push(-1);
  };

  atomic<int> consumed(0);
  auto consumer = [&consumed, &q](){
    int v;
    while (q.pop(v) && v != -1)
      consumed++;
  };

  for (int i = 0; i < thread::hardware_concurrency()/2; i++) {
    threads.push_back(thread(consumer));
    threads.push_back(thread(producer, 100'000));
  };
  for (auto &t : threads) t.join();

  CHECK(produced > 0);
  CHECK(produced == consumed);
  // log("ElasticQueue:", double(t(CpuTime())));
}
}// uniq • Released under GPL 3.0
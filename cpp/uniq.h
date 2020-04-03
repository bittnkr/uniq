// uniQ The Lock free Queue.
// See test.cpp for use case & README.md for details about the solution
// Released under LGPL 3.0

#pragma once
#include <assert.h>

#include <atomic>
#include <thread>
#include <vector>
using namespace std;

template <class T>
class Queue {
 protected:
  vector<T> buffer;
  int mask;

  atomic<int> in, out;
  vector<int> isFree;

 public:
  Queue(int size_ = 64) : mask(size_) {
    assert(mask && !(mask & (mask - 1)) &&
           "Queue size must be a positive power of 2.");
    buffer = vector<T>(mask, 0);
    isFree = vector<int>(size_, 1);
    mask--;

    out = in = 0;
    stop = 0;
  }

  int stop;

  int push(T item) {
    int i;
    do {
      i = in;
      if (stop) return 0;
      while (i - out > mask && !stop) sched_yield();  // if full, wait for space
    } while (!isFree[i & mask] || !in.compare_exchange_weak(i, i + 1));

    buffer[i & mask] = item;
    isFree[i & mask] = 0;
    return i;
  }

  T pop() {
    int o;
    do {
      if (stop) return T{};
      o = out;
      while (o == in && !stop) sched_yield();  // if empty, wait for item
    } while (isFree[o & mask] || !out.compare_exchange_weak(o, o + 1));

    T r = buffer[o &= mask];
    isFree[o] = 1;
    return r;
  }

  // virtual bool allow(int i) { return i; };
  int size() { return mask + 1; }
  inline bool full() { return (in - out) > mask; }
  inline bool empty() { return out == in; }
  int nextJobId() { return out; };
  inline void wait(int id) { while(out < id) sched_yield(); }
};

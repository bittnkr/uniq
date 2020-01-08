// uniQ The Lock free Queue. 
// See test.cpp for use case & README.md for details about the solution

#pragma once
#include <atomic>
#include <assert.h>
#include <thread>
#include <vector>
using namespace std;

template <class T> class Queue {
protected:
  vector<T> buffer;
  int mask;

  atomic<int> in, out;
  vector<int> isFree;

public:
  Queue(int size_ = 64) : mask(size_) {
    assert(mask && !(mask & (mask - 1)) && "Queue size must be a positive power of 2.");
    buffer = vector<T>(mask, 0);
    isFree = vector<int>(size_, 1);
    mask--;

    out = in = 0;
  }

  int push(T item) {

    int i;
    do {
      i = in;
      while (i - out > mask)
        sched_yield(); // if full, wait for space
    } while (!isFree[i & mask] || !in.compare_exchange_weak(i, i + 1));
     
    buffer[i & mask] = item;
    isFree[i & mask] = 0;
    return i;
  }

  T pop() {
    int o;
    do {
      o = out;
      while (o == in)
        sched_yield(); // if empty, wait for item
    } while (isFree[o & mask] || !out.compare_exchange_weak(o, o + 1));

    T r = buffer[o &= mask];
    isFree[o] = 1;
    return r;
  }
};

// Released under GNU 3.0
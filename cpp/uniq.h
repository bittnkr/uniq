#include "utils.h"
#include <assert.h>
#include <thread>
#include <vector>
#pragma once

using namespace std;

template <class T> class Queue {
protected:
  vector<T> buffer;
  int mask;
  atomic<int> head, tail;

public:
  Queue(int size_ = 64) : mask(size_) {
    assert(mask && !(mask & (mask - 1)) &&
           "Queue size must be a positive power of 2.");
    buffer = vector<T>(mask, 0);
    mask--;
    head = tail = 0;
  }

  virtual T push(T item) {
    assert(item); // do not allow nulls

    int t;
    do {
      t = head;
      while (t - tail > mask)
        sched_yield(); // if full, wait for space
    } while (buffer[t & mask] ||
             !head.compare_exchange_weak(t, t + 1)); // head = t+1); // srsw)

    buffer[t & mask] = item;
    return item;
  }

  virtual T pop() {
    int t;
    do {
      t = tail;
      while (t == head)
        sched_yield(); // if empty, wait for item
    }

    while (!buffer[t & mask] ||
           !tail.compare_exchange_weak(t, t + 1) // tail = t+1 (ons swsr)
    );

    T r = buffer[t &= mask];
    buffer[t] = 0;
    return r;
  }
};

// Part of uniQ library released under GNU 3.0 
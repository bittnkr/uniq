#include "std.h"

// the most basic queue, with minimum dependecies pushing only ints
class Queue { 
private:

  vector<int> buffer;
  int size, mask;
  atomic<int> in,out;

public:

  Queue(int size = 64): size(size)
  {
    buffer = vector<int>(size, 0);
    mask = size - 1;
    in = out = 0;
  }  

  void push(int item) {
    // assert(item != 0);
    int i;
    do {

      i = in;
      while (i - out == size) sched_yield(); // if full, wait for space

    } while (buffer[i & mask] || !in.compare_exchange_weak(i, i + 1));

    buffer[i & mask] = item;
  }

  int pop() {
    int o;
    do {

      o = out;
      while (o == in) sched_yield();  // if empty, wait for item

    } while (!buffer[o & mask] || !out.compare_exchange_weak(o, o + 1));

    int r = buffer[o &= mask];
    buffer[o] = 0;
    return r;
  }
};
// UniQ • Released under GPL 3 licence
#include <assert.h>
#include <thread>
#include <vector>
#include "utils.h"
#pragma once

using namespace std;

template <class T>
class Queue
{
  protected:
    vector<T> buffer;
    int mask;
    atomic<int> input, output;

  public:
    Queue(int size_=64): mask(size_) 
    {
      assert(mask && !(mask & (mask - 1) ) && "Queue size must be a positive power of 2.");
      buffer = vector<T>(mask, 0);
      mask--;
      input = output = 0;
    }

    virtual T push(T item)
    {
        if( !allow(item) ) return (0);
        int t;
        do
        {
            t = input;
            while (t-output > mask) sched_yield(); // if full, wait for space
        }
        while (buffer[t & mask] 
          || !input.compare_exchange_weak(t, t+1) // input = t+1); // srsw
        ); 

        buffer[t & mask] = item;
        return item;
    }

    virtual T pop()
    {
        int t;
        do
        {
            t = output;
            while (t == input) sched_yield(); // if empty, wait for item
        }
        while (!buffer[t & mask] 
         || !output.compare_exchange_weak(t, t+1) //output = t+1 (ons swsr) 
        ); 

        T r = buffer[t &= mask];
        buffer[t] = 0;
        return r;
    }
    virtual bool allow( int i ) { return i; };
    int Size() { return mask+1; }
    bool free() { return (input-output)>mask; }
    bool empty() { return output==input; }
    int taskid() { return output; };
};

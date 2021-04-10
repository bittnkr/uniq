// OpenQueue - A Thread-safe Queue using mutexes and condition variables.
// Used for speed comparison. https://stackoverflow.com/a/16075550/9464885
#pragma once
#include "Set.h"
#include "Actor.h"
namespace uniq {

template <typename T> class OpenQueue: public Set<T>, public Actor {
  queue<T> q;
  mutable mutex m;
  condition_variable notEmpty;
  atomic<int> in, out;
  int maxsize=0;
 public:
  
  OpenQueue(int maxsize = 0) 
    : maxsize(maxsize), Set<T>() { out = in = 1; }

  int push(const T item, bool wait=true) {
    if(maxsize) WAIT(q.size() < maxsize);
    lock_guard<mutex> lock(m);
    q.push(item);
    notEmpty.notify_one();
    return in++;
  }

  int pop(T &item, bool wait=true) {
    if(q.empty( )&& !wait) return 0;
    
    unique_lock<mutex> lock(m);
    notEmpty.wait(lock, [&]{ return !q.empty(); } );
    item = q.front();
    q.pop();
    return out++; 
  }

  inline bool full() { return (in-out) >= maxsize; }
  inline bool empty() { return q.empty(); }
  inline int size() { return in-out; }
  inline int done() { return out-1; }
};

}// uniq • Released under GPL 3.0

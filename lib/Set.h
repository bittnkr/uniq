//==============================================================================
// Set • A common interface for collections
//==============================================================================
#pragma once
#include "call.h"
namespace uniq {

template <typename T> struct Set{
  virtual int push(const T& item, bool wait = true) { return 0; }
  virtual int pop(T& item, bool wait = true) { return 0; }

  virtual int size() { return 0; }
  virtual bool full(){ return 1; }
  virtual bool empty(){ return 1; }

  virtual int first(T &item) { return 0; }
  virtual int last(T &item) { return 0; }
  virtual bool has(const T &item) { return 0; }

  // virtual bool find(T &item, lambda){ }
  // virtual bool each(T &item, lambda) {  }
  // virtual Set<T> map(lambda) {  }
  // virtual Set<T> reduce(lambda) {  }
  // virtual Set<T> select(lambda) {  }

protected:
  virtual void onempty() {}
  virtual void onfull() {}
};


}// uniq • Released under GPL 3.0
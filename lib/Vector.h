//==============================================================================
// Vector • A std:vector implementation of Set<T>
//==============================================================================
#pragma once
#include "Set.h"
#include "Actor.h"
namespace uniq {
template <typename T> struct Vector;

template <typename T> ostream& operator<< ( ostream& os, const Vector<T>& L );

//======================================================================= Vector
template <typename T> class Vector: public Set<T> {
  queue<T> Q;
  mutable mutex m;
  condition_variable notEmpty;
  atomic<int> in, out;
  int maxsize=0;

protected:
  virtual void onempty() override { }
  virtual void onfull() override { }

public:

  Vector(int maxsize = 0) : maxsize(maxsize), Set<T>() { out = in = 1; }

  int push(const T item, bool wait=true) {
    if(maxsize) WAIT(Q.size() < maxsize);
    lock_guard<mutex> lock(m);
    Q.push(item);
    notEmpty.notify_one();
    return in++;
  }

  int pop(T &item, bool wait=true) override {
    if(Q.empty( )&& !wait) return 0;
    unique_lock<mutex> lock(m);
    notEmpty.wait(lock, [&]{ return !Q.empty(); } );
    item = Q.front();
    Q.pop();
    return out++; 
  }

  bool empty() override { return Q.empty(); }
  bool full() override { return 0; }
  int size() override { return Q.size(); }
  
  int first(T &item) override { 
    if(empty()) return 0;
    item = Q.front(); 
    return 1; //v.front()-v.begin(); 
  } 

  int last(T &item) override {  
    if(empty()) return 0;
    item = Q.back(); 
    return Q.size(); }

  // virtual T& get(const int i) { }
  // virtual T& set(const int i, const T &item) { }
  // virtual bool has(const T &item) { }

  // virtual bool find(T &item, lambda){ 
  //   if(!vector.size()) return 0;
  //   auto it = find_if(states.begin(), states.end(), lambda);
  //   int r = distance(states.begin(), it);
  //   return (r == states.size()) ? 0 : r;
  // }
  
  // virtual bool each(T &item, lambda) {  }
  // virtual bool next(T &item) {  }
  // virtual bool prev(T &item) {  }
  // virtual int index(T &item) { }
  // virtual Set<T> map(lambda) {  }
  // virtual Set<T> reduce(lambda) {  }
  // virtual Set<T> select(lambda) {  }
	
  inline operator string() const { 
    stringstream os; 
    os << "[";
    for (auto it = Q.begin(); it != Q.end(); it++) {
      if (it != Q.begin()) os << ",";
      os << *it;
    };
    os << "]";
    return os.str();
  }

  friend ostream& operator << ( ostream& os, const Vector& v ) { return os << string(v); }
};

// ================================================================ TEST(Vector)
TEST(Vector){
  int i = -1;
  Vector<int> v; // out(v);
  CHECK(v.empty()); CHECK(!v.full()); CHECK(!v.first(i)); CHECK(!v.last(i)); CHECK(!v.size());

  v.push(3); // out(v);
  CHECK(!v.empty()); CHECK(v.first(i) && i==3 ); CHECK(v.last(i) && i==3); CHECK(v.size()==1);

  v.push(4); 
  // CHECK(sstr(v)=="[3,4]");
  CHECK(!v.empty()); CHECK(v.first(i) && i==3 ); CHECK(v.last(i) && i==4); CHECK(v.size()==2);

  CHECK(v.pop(i) && i==3); CHECK(v.size()==1); CHECK(v.first(i) && i==4 ); CHECK(v.last(i) && i==4); 

  CHECK(v.pop(i) && i==4); CHECK(v.empty()&&v.size()==0); CHECK(!v.first(i) && i==4 );  CHECK(!v.last(i) && i==4); 

  CHECK(v.empty()); CHECK(!v.pop(i,0) && i==4);
}

// ============================================================ TEST(LockQueue)
TEST(LockQueue){ 
  Vector<int> q(64);  
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

  auto t = CpuTime();

  for (int i = 0; i < thread::hardware_concurrency()/2; i++) {
    threads.push_back(thread(consumer));
    threads.push_back(thread(producer, 100'000));
  };
  for (auto &t : threads) t.join();

  CHECK(produced > 0);
  CHECK(produced == consumed);
  // log("OpenQueue:", double(t(CpuTime())));
}

}// UniQ • Released under GPL3 licence
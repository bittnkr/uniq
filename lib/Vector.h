//==============================================================================
// Vector • A std:vector implementation of Set<T>
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {
template <class T> struct Vector;

template <class T> ostream& operator<< ( ostream& os, const Vector<T>& L );

//======================================================================= Vector
template <class T> class Vector: public Set<T>{
protected:
  virtual void onempty() {}
  virtual void onfull() {}
public:
  vector<T> V;

  bool empty() override { return V.empty(); }
  bool full() override { return 0; }
  int size() override { return V.size(); }

  int first(T &item) override { 
    if(empty()) return 0;
    item = V.front(); 
    return 1; //V.front()-V.begin(); 
  } 
  int last(T &item) override {  
    if(empty()) return 0;
    item = V.back(); 
    return V.size(); }

  int push(const T& item, bool wait = true) override { V.push_back(item); return V.size(); }
  int pop(T& item, bool wait = true) override { 
    if(V.empty() && !wait) return 0;
    WAIT(!V.empty());
    item = *V.begin();
    V.erase(V.begin());
    return 1;
  }

  // virtual T& get(const int i) { }
  // virtual T& set(const int i, const T &item) { }
  // virtual bool has(const T &item) { }
  // virtual bool find(T &item, lambda){ }
  // virtual bool each(T &item, lambda) {  }
  // virtual bool next(T &item) {  }
  // virtual bool prev(T &item) {  }
  // virtual int index(T &item) { }
  // virtual Set<T> map(lambda) {  }
  // virtual Set<T> reduce(lambda) {  }
  // virtual Set<T> select(lambda) {  }
	
  friend ostream& operator << ( ostream& os, const Vector& v ) { 
    os << "[";
    for (auto it = v.V.begin(); it != v.V.end(); it++) {
      if (it != v.V.begin()) os << ",";
      os << *it;
    };
    os << "]";
    return os;
  }
};

// ================================================================ TEST(Vector)
TEST(Vector){
  int i = -1;
  Vector<int> V; // out(V);
  CHECK(V.empty()); CHECK(!V.full()); CHECK(!V.first(i)); CHECK(!V.last(i)); CHECK(!V.size());

  V.push(3); // out(V);
  CHECK(!V.empty()); CHECK(V.first(i) && i==3 ); CHECK(V.last(i) && i==3); CHECK(V.size()==1);

  V.push(4); CHECK(sstr(V)=="[3,4]");
  CHECK(!V.empty()); CHECK(V.first(i) && i==3 ); CHECK(V.last(i) && i==4); CHECK(V.size()==2);

  CHECK(V.pop(i) && i==3); CHECK(V.size()==1); CHECK(V.first(i) && i==4 ); CHECK(V.last(i) && i==4); 

  CHECK(V.pop(i) && i==4); CHECK(V.empty()&&V.size()==0); CHECK(!V.first(i) && i==4 );  CHECK(!V.last(i) && i==4); 

  CHECK(V.empty()); CHECK(!V.pop(i,0) && i==4);
}//*/
}// uniq • Released under GPL 3.0
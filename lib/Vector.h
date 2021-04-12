//==============================================================================
// Vector • A std:vector implementation of Set<T>
//==============================================================================
#pragma once
// #include "UniQ.h"
namespace uniq {
template <class T> struct Vector;

template <class T> ostream& operator<< ( ostream& os, const Vector<T>& L );

//======================================================================= Vector
template <class T> class Vector: public Set<T>{
protected:
  virtual void onempty() override {}
  virtual void onfull() override {}
public:
  vector<T> v;

  bool empty() override { return v.empty(); }
  bool full() override { return 0; }
  int size() override { return v.size(); }

  int first(T &item) override { 
    if(empty()) return 0;
    item = v.front(); 
    return 1; //v.front()-v.begin(); 
  } 

  int last(T &item) override {  
    if(empty()) return 0;
    item = v.back(); 
    return v.size(); }

  int push(const T& item, bool wait = true) override { v.push_back(item); return v.size(); }
  int pop(T& item, bool wait = true) override { 
    if(v.empty() && !wait) return 0;
    WAIT(!v.empty());
    item = v[0];
    v.erase(v.begin());
    return 1;
  }

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
	
  friend ostream& operator << ( ostream& os, const Vector& v ) { 
    os << "[";
    for (auto it = v.v.begin(); it != v.v.end(); it++) {
      if (it != v.v.begin()) os << ",";
      os << *it;
    };
    os << "]";
    return os;
  }
};

// ================================================================ TEST(Vector)
TEST(Vector){
  int i = -1;
  Vector<int> v; // out(v);
  CHECK(v.empty()); CHECK(!v.full()); CHECK(!v.first(i)); CHECK(!v.last(i)); CHECK(!v.size());

  v.push(3); // out(v);
  CHECK(!v.empty()); CHECK(v.first(i) && i==3 ); CHECK(v.last(i) && i==3); CHECK(v.size()==1);

  v.push(4); CHECK(sstr(v)=="[3,4]");
  CHECK(!v.empty()); CHECK(v.first(i) && i==3 ); CHECK(v.last(i) && i==4); CHECK(v.size()==2);

  CHECK(v.pop(i) && i==3); CHECK(v.size()==1); CHECK(v.first(i) && i==4 ); CHECK(v.last(i) && i==4); 

  CHECK(v.pop(i) && i==4); CHECK(v.empty()&&v.size()==0); CHECK(!v.first(i) && i==4 );  CHECK(!v.last(i) && i==4); 

  CHECK(v.empty()); CHECK(!v.pop(i,0) && i==4);
}//*/
}// UniQ • Released under GPL 3 licence
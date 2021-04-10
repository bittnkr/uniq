//==============================================================================
// Id • A class with id
//==============================================================================
#pragma once
// #include "uniq.h"
namespace uniq {
//======================================================================= Id
template <typename T> class Id{ 
  inline static map<string, integer> Ids = {};
  inline static mutex m = {};

public:
  Id(){ 
    lock_guard<mutex> lock(Id::m); // Mutex(Ids);
    auto r = &Ids[ anyType(*this) ];
    id = AtomicAdd(*r,1);
    // log("Id()", group, " ", id);
  };

  integer id;
  inline operator integer() const { return id; }
  
  bool operator<(const Id &other) const { return id < other.id; };
  bool operator>(const Id &other) const { return id > other.id; };
  bool operator==(const Id &other) const { return id == other.id; };
};

// #define ID(x) Id(#x);

/*/========================================================================= test
TEST(Id){
  Id<int> a, b;
  CHECK(b.id > 0);
  CHECK(b > 0);
  CHECK(a < b);

  Id<string> c;
  CHECK(c.id == 1);
} //*///
} // uniq • Released under GPL 3.0

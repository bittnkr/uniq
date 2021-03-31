//==============================================================================
// Id • A class with id
//==============================================================================
#pragma once
namespace uniq {

//======================================================================= Id
class Id {
public:
  inline static Atomic<int> nextId{0};
  int id;
  
  Id(){ id = ++nextId; };

  bool operator<(const Id &other) const { return id < other.id; };
  bool operator>(const Id &other) const { return id > other.id; };
  bool operator==(const Id &other) const { return id == other.id; };
};

//========================================================================= test
#include "test.h"
void test_Id(){
  Id a, b;
  CHECK(b.id > 0);
  CHECK(a.id < b.id);
}
}// uniq • Released under GPL 3.0
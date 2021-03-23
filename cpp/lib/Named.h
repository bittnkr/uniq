//==============================================================================
// Named • A class with name
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

atomic<int> Indent(0);
//======================================================================= Named
struct Named {
  string name;
  Named(){name="";};
  Named(string name_) : name(name_){};
  Named operator = (Named o) { return name = o.name; }
  // used by std::set to compare event names
  bool operator<(const Named &other) const { return name < other.name; };
  bool operator>(const Named &other) const { return name > other.name; };
};

//========================================================================= test
void test_Named(){
  Named alice("Alice"), bob("Bob");
  CHECK(alice.name=="Alice");
  CHECK(alice<bob);
}
}// uniq • Released under GPL 3.0
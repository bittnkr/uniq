//==============================================================================
// Name • A class with name
//==============================================================================
#pragma once
namespace uniq {

//======================================================================= Name
struct Name {
  string name;
  Name(){name="";};
  Name(string name_) : name(name_){};
  Name operator = (Name o) { return name = o.name; }
  // used by std::set to compare event names
  bool operator<(const Name &other) const { return name < other.name; };
  bool operator>(const Name &other) const { return name > other.name; };
  bool operator==(const Name &other) const { return name == other.name; };
};

/*/================================================================== test_Name
#include "test.h"
TEST(Name){
  Name alice("Alice"), bob("Bob");
  CHECK(alice.name=="Alice");
  CHECK(alice<bob);
}//*/
}// uniq • Released under GPL 3.0
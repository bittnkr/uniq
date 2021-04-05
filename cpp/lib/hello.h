//==============================================================================
// Hello • A starting point for C++ classes
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

//======================================================================== Hello
struct Hello: public Named, Id {
  // fields
  string msg;

  // constructors & destructor
  Hello(string name = "somebody") : Named(name), Id("Hello") { };
  ~Hello() { say("goodbye."); };

  // methods
  string say(string s);

  // functor
  string operator()(string s){ return say(s); }
};

// streamable cout << Hello()
ostream& operator<<(ostream& os, Hello& t) { 
  return os << repeat("| ",t.id) << t.name << " say: " << t.msg << "\n"; 
}

// referencing streamable this
string Hello::say(string s) { msg = s; cout << (*this); return s; }

// utility constructor
string hello(string msg) { return Hello()(msg); }

/*/================================================================= TEST(Hello)
TEST(Hello){
  Hello alice("Alice"), bob("Bob");
  CHECK(alice.id < bob.id);
  // alice("Hola!");
  // bob("Olá");
  // hello("Que tal?");
}//*/
}// uniq • Released under GPL 3.0
//==============================================================================
// Hello • A starting point for C++ classes
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

//======================================================================= Hello
struct Hello: public Named, Id {
  Hello(string name = "somebody") : Named(name), Id("Hello") { };
  ~Hello() { say("goodbye."); };
 
  string msg;
  string say(string s);
  string operator()(string s){ return say(s); } // functor
};

// streamable cout << Hello()
ostream& operator<<(ostream& os, Hello& t) { 
  return os << repeat("| ",t.id) << t.name << " say: " << t.msg << "\n"; 
}

// referencing streamable this
string Hello::say(string s) { msg = s; cout << (*this); return s; }

// utility function
string hello(string msg) { return Hello()(msg); }

/*/=================================================================== test_Hello
TEST(Hello){
  CHECK(true);
  // Hello alice("Alice"), bob("Bob");
  // alice("Hola!");
  // bob("Olá");
  // hello("Que tal?"); // somebody
}//*/

}// uniq • Released under GPL 3.0
// int main() { uniq::test_hello(); }
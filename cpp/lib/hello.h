//==============================================================================
// Hello • A starting point for C++ classes
//==============================================================================
#pragma once
#include "std.h"
namespace uniq {

atomic<int> Indent(0);
//======================================================================= Hello
struct Hello { // : public Parent {
  string name;
  int indent=0;

  Hello(string name_ = "somebody") { name=name_; indent = Indent++; };
  ~Hello() { say("goodbye."); Indent--; };

  void say(string msg) {
    cout << string(indent, '|') << name << " say: " << msg << endl;
  };
};

void say(string msg) { return Hello().say(msg); }

//========================================================================= test
#include "test.h"
void test_hello(){
  Hello alice("Alice"), bob("Bob");

  CHECK(bob.indent == alice.indent+1);

  alice.say("Hola!");
  bob.say("Olá");
  say("To be or not?"); // somebody
}
}// uniq • Released under GPL 3.0

// int main() { uniq::test_hello(); }
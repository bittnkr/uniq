//==============================================================================
// A starting point for C++ classes
//==============================================================================
// #pragma once
#include "std.h"
using namespace std;

namespace uniq {

atomic<int> Indent(0);
//======================================================================= Sayer
struct Sayer { // : public Parent {
  string name;
  int indent=0;

  Sayer(string name_ = "somebody") { name=name_; indent = Indent++; };
  ~Sayer() { say("goodbye."); Indent--; };

  void say(string msg) {
    cout << string(indent, '|') << name << " say: " << msg << endl;
  };
};

void say(string msg) { return Sayer().say(msg); }

//========================================================================= test
#include "test.h"
void test_sayer(){
  Sayer alice("Alice"), bob("Bob");

  CHECK(bob.indent == alice.indent+1);

  alice.say("Hola!");
  bob.say("Olá");
  say("To be or not?"); // somebody
}
} // uniq

int main() { uniq::test_sayer(); }
//==============================================================================
// Actor - A starting point for C++ classes 
//==============================================================================
#pragma once
#include "common.h"
namespace test {

atomic<int> Indent(0);
//======================================================================= Actor
struct Actor { // : public Parent {
  string name;
  int indent=0;

  Actor(string name_ = "somebody") { name=name_; indent = Indent++; };
  ~Actor() { say("goodbye."); Indent--; };

  void say(string msg) { 
    // cout << string(indent, '|') << name << " say: " << msg << endl; 
  };
};

void say(string msg) { return Actor().say(msg); }

//========================================================================= test
void test_boilerplate(){
  Actor alice("Alice"), bob("Bob");

  CHECK(bob.indent == alice.indent+1);

  alice.say("Hola!");
  bob.say("Olá");
  say("To be or not?"); // somebody
}
}// namespace test
// int main(int argc, const char *argv[]) { test::test_boilerplate(); }
// part of UniQ library - released under GPL 3.0
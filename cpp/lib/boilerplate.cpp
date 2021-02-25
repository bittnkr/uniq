//==============================================================================
// Sayer - A starting point for C++ classes 
//==============================================================================
#pragma once
#include "uniq.h"
namespace test {

atomic<int> Indent(0);
//======================================================================= Sayer
struct Sayer { // : public Parent {
  string name;
  int indent=0;

  Sayer(string name_ = "somebody") { name=name_; indent = Indent++; };
  ~Sayer() { say("goodbye."); Indent--; };

  void say(string msg) { 
    // cout << string(indent, '|') << name << " say: " << msg << endl; 
  };
};

void say(string msg) { return Sayer().say(msg); }

//========================================================================= test
void test_boilerplate(){
  Sayer alice("Alice"), bob("Bob");

  CHECK(bob.indent == alice.indent+1);

  alice.say("Hola!");
  bob.say("Olá");
  say("To be or not?"); // somebody
}
}// namespace test
// int main(int argc, const char *argv[]) { test::test_boilerplate(); }
// part of UniQ library - released under GPL 3.0
//==============================================================================
// Log • A Logging utility
//==============================================================================
#pragma once
#include "Name.h"
#include "OpenQueue.h"
namespace uniq {

//========================================================================== Log
class Log : public uniq::OpenQueue<string>, public Name{ 
public:
  bool hasColors = 1;
  OpenQueue<string> Q; // Queue of strings waiting to be flushed

  Log(string name="console") : Name(name){ };

  virtual void flush(ostream &stream) { 
    string s;
    while(Q.pop(s,0)) stream << s;
    stream << RST << std::flush;
  };

  void flush() { flush(cout); }
  
  template <typename... Args> 
  string operator()(Args &&... args ){
    string r = sstr(args...);
    Q.push(r);
    return r;
  };

  inline int ready(){ return Q.size(); }

  ~Log() { flush(); };
};

// ostream& operator<<(ostream& os, Log& t) { return os << string(id, '|') << name << " log: " << msg << endl; }
// istream& operator>>(istream& os, Log& t) {  }

template <typename... Args> 
string logn(Args &&... args ){
  static Log out;
  return out(args..., "\n");
}

// string log(double n) { return uniq::log(n); }
// string log(integer n) { return uniq::log(n); }
// string log(uinteger n) { return uniq::log(n); }

TEST(Log){ //=============================================================== Log
  throw exception(); // to see an exception
  // Log say, err("cerr");
  // say("Hola!");
  // err("Olá");
  // log("To be or not?");
}

}// uniq • Released under GPL 3.0

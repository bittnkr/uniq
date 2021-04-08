//==============================================================================
// Log • A Logging utility
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

//========================================================================== Log
class Log : public Actor<string>, public Named{ 
public:
  bool hasColors = 1;
  ElasticQueue<string> Q; // Queue of strings waiting to be flushed

  Log(string name="console") : Named(name){ };

  // ~Log() { flush(); };
  // virtual void beat(){ flush(); };

  virtual void flush() { 
    string s;
    while(Q.pop(s,0)){ 
      cout << s << "\033[0m" << std::flush;
    }
  };
  
  template <typename... Args> 
  string operator()(Args &&... args ){
    string r = sstr(args...);
    Q.push(r);
    return r;
  };
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

//===================================================================== test_Log
TEST(Log){
  Log say, err("cerr");
  say("Hola!");
  err("Olá");
  // log("To be or not?");
}
}// uniq • Released under GPL 3.0

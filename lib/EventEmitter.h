#pragma once
#include "uniq.h"
#include "Name.h"
#include "call.h"
namespace uniq {

struct EventEmitter{

  struct functionCompare{ bool operator()(const voidfunction a, const voidfunction b) const { return &a < &b; }; };
  map<string, set<voidfunction, functionCompare>> subs;

  void listen(const string name, const voidfunction f){ //, Args &&...args) {
    lock_guard<std::mutex> lock(mutex);
    subs[name].insert(f);
  }

  void remove(const string name, const voidfunction f) {
    lock_guard<std::mutex> lock(mutex);
    subs[name].erase(f);
  }

  void emit(string name) { 
    lock_guard<std::mutex> lock(mutex);
    for (auto f : subs[name]) 
      f();
  };
};

TEST(EventEmitter){
  EventEmitter e; 
  Atomic<int> V;

  e.listen("ping", [&]{ V+=2;  e.emit("pong"); /*out(GRN, " ping ", V);*/ });
  e.listen("ping", [&]{ V--; /*out(YEL,"ping ", V);*/ });

  e.listen("pong", [&]{ e.emit(V < 17 ? "ping" : "done"); /*out(RED," pong ", V);*/ });
  e.listen("pong", [&]{ /*out(MAG," pong ",V);*/ });

  e.listen("done", [&]{ CHECK(V == 17); /*out(GRY," done!", V);*/ });

  e.emit("ping"); // start the flow
  WAIT(V==17);
};
}// UniQ • Released under GPL 3 licence
#pragma once
#include "Event.h"
namespace uniq {

struct EventEmitter{
  map<string, Event> events;

  void listen(const string name, const voidfunction f){ //, Args &&...args) {
    events[name].listen(f);
  }

  void remove(const string name, const voidfunction f) {
    events[name].remove(f);
  }

  void emit(string name) { 
    events[name].emit();
  };
};

TEST(EventEmitter){
  EventEmitter e; 
  Atomic<int> V;

  e.listen("ping", [&]{ V+=2;  e.emit("pong"); /*out(GRN, " ping ", V);*/ });
  e.listen("ping", [&]{ V--; /*out(YEL,"ping ", V);*/ });

  e.listen("pong", [&]{ e.emit(V < 17 ? "ping" : "done"); /*out(RED," pong ", V);*/ });
  e.listen("pong", [&]{ /*out(MAG," pong ",V);*/ });

  e.listen("done", [&]{ 
    CHECK(V >= 17); /*out(GRY," done!", V);*/ });

  e.emit("ping"); // start the flow
  WAIT(V>=17);
};

}// UniQ • Released under GPL 3 licence
#pragma once
#include "uniq.h"
#include "Name.h"
#include "call.h"
#include "Event.h"
namespace uniq {

//======================================================================== Event
void Event::listen(const voidfunction f){ //, Args &&...args) {
  lock_guard<std::mutex> lock(mutex);
  subscribers.insert(f);
}

void Event::remove(const voidfunction f) {
  lock_guard<std::mutex> lock(mutex);
  subscribers.erase(f);
}

void Event::emit() { 
  // lock_guard<std::mutex> lock(mutex);
  for (auto f : subscribers) 
    f(); // 
    // run(f);

};

//================================================================== TEST(Event)
TEST(Event){
  Event ping, pong, done; 
  Atomic<int> V;
  // pool().showstats = true;

  ping.listen([&]{ V+=2;  pong.emit(); /*out(GRN, " ping ", V);*/ });
  ping.listen([&]{ V--; /*out(YEL,"ping ", V);*/ });

  pong.listen([&]{ V < 17 ? ping.emit() : done.emit(); /*out(RED," pong ", V);*/ });
  pong.listen([&]{ /*out(MAG," pong ",V);*/ });

  done.listen([&]{ CHECK(V >= 17); /*out(GRY," done!", V);*/ });

  ping.emit(); // start the flow
  WAIT(V==17);
  // pool().join();
};

//================================================================= EventEmitter
void EventEmitter::listen(const string name, const voidfunction f){ //, Args &&...args) {
  events[name].listen(f);
}

void EventEmitter::remove(const string name, const voidfunction f) {
  events[name].remove(f);
}

void EventEmitter::emit(string name) { 
  events[name].emit();
};

//=========================================================== TEST(EventEmitter)
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
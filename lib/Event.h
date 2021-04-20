#pragma once
#include "uniq.h"
#include "Name.h"
#include "call.h"
namespace uniq {

struct Event{

  struct functionCompare{ bool operator()(const voidfunction a, const voidfunction b) const { return &a < &b; }; };
  set<voidfunction, functionCompare> subscribers;

  void listen(const voidfunction f){ //, Args &&...args) {
    lock_guard<std::mutex> lock(mutex);
    subscribers.insert(f);
  }

  void remove(const voidfunction f) {
    lock_guard<std::mutex> lock(mutex);
    subscribers.erase(f);
  }

  void emit() { 
    lock_guard<std::mutex> lock(mutex);
    for (auto f : subscribers) 
      f(); // run(f);
  };
};

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
  // WAIT(V==17);
  // pool().join();
};

}// UniQ • Released under GPL 3 licence
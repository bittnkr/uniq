// Each state can define events that trigger a transition.
// A transition defines how a machine would react to the event, by exiting one state and entering another state.
// A transition can define actions that occur when the transition happens. Actions will typically have side effects.

//==============================================================================
// State is a
// StateMachine or a collection of states
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

struct StateRecord {
  string name;
  voidfunction onenter;
  voidfunction onexit;
};

class State {
 private:
  int current = -1;
  vector<StateRecord> states;

 public:
  State(){};
  ~State(){ exitCurrent(); };


  int id(const string state, bool check_ = 0) {  // the numeric id of the state
    if(!states.size()) return -1;
    auto it = find_if(states.begin(), states.end(), [&](const StateRecord& s) -> bool { return s.name == state; });

    int r = distance(states.begin(), it);
    return (r == states.size()) ? -1 : r;
  };

  int id() { return current; };

  string name(int i){
    check(i >= 0 && i < states.size(), "State[\"", i, "\"] not found");
    return states[i].name;
  };

  string name(){ return states[current].name; }

  // transition
  void on(const string name, voidfunction onenter, voidfunction onexit = nullptr) {
    int i = id(name);
    if (i < 0)
      states.push_back({name, onenter, onexit});
    else {
      states[i].onenter = onenter;
      states[i].onexit = onexit;
    }
    if (current < 0) enter(0);
  }

  void remove(const string name) {
    int i = id(name, 1);
    if(current==i) exitCurrent();
    states.erase(states.begin() + i);
  }

  void exitCurrent(){
    if (current >= 0 && states[current].onexit)
      states[current].onexit();
    current = -1;
  }

  inline void enter(int i) { 
    exitCurrent();
    states[i].onenter();
    current = i;
  }

  // the functor changes the state
  inline void operator()(int i) { enter(i); }
  inline void operator()(string s) { enter(id(s,1)); }

  inline string operator()(){ return states[current].name; }

  // the bracket operator returns true/false if the state match
  inline bool operator[](const int id) { return current == id; }
  inline bool operator[](const string name) {
    int i = id(name);
    check( i >= 0, "State[\"", name, "\"] not found");
    return (current >= 0) && (states[current].name == name); 
  }
  inline bool operator == (int i) { return current==i; }
  inline bool operator == (string s) { return states[current].name == s; }
};

ostream& operator<<(ostream& os, State& t) {
  return os << "[" << t() << ":" << t.id() << "]";
}

}// uniq • Releas under GPL 3.0
// https://kentcdodds.com/blog/implementing-a-simple-state-machine-library-in-javascript?ck_subscriber_id=739354581
//==============================================================================
// State is { data + events }
// StateMachine or a collection of States
//==============================================================================
#pragma once
#include "call.h"
#include "utils.h"
#include "test.h"
namespace uniq {
class StringMachine;

//======================================================================== State
struct State {
  string state;
  voidfunction onenter = nullptr;
  voidfunction onexit = nullptr;
  // vector<State&> transitions;

  // optional<vector<State&>> transitions;
  // State(string state, voidfunction onenter = nullptr, voidfunction onexit = nullptr)
  // : state(state), onenter(onenter), onexit(onexit ){ }

  ~State(){ if(onexit) onexit(); };

  // void exit(State& other) { onexit(); } 
  State& enter(State& other) { other.onexit(); *this=other; onenter(); return *this; } 

  inline bool operator[](const string name) { return state == name; }

  inline bool is(const string name) { return state == name; }
  inline bool operator==(const string name) { return is(name); }
  inline bool operator!=(const string name) { return state != name; }
  // inline bool operator=(const string name) { 

  inline bool is(const State& other) { return other.state == state; }
  inline bool operator==(const State& other) { return is(other); }
  inline bool operator!=(const State& other) { return other.state != state; }

  // inline State& operator()(const string name) { *this = name; return *this; }
  inline State& operator=(State& other){
    // assert(states.count(name));
    if(state != "") onexit();
    this->state = other.state;
    this->onenter = other.onenter;
    this->onexit = other.onexit;
    onenter();
    return *this;
  }

  friend StringMachine;
  friend std::ostream& operator << ( std::ostream& os, const State& s) { return os << s.state; }
};

//================================================================== StringMachine
class StringMachine { // public StateMachine<string>
 private:
  function<void(State&, State&)> onchange;
  map<string, State> states;
  State* current;

  // State& parse(string names=""){ 
  void parse(string names=""){
    vector<string> tr;
    auto tokens = split(names, ' ');
    states.clear();
    for(auto &t : tokens)
      states[t].state = t;
    // foreach(, [&](string const& s){ states[t] = {}; });
  }

 public:
  StringMachine(){};

  StringMachine(const string names) { 
    onchange = nullptr; //[&](State state, State from) { };
    // log(parse(names), this->current->state);
    parse(names); 
  };

  // StringMachine(function<void(StringMachine&)> init) { init(*this); };
    
  ~StringMachine(){ 
    for(auto &s : states) 
      s.second.onexit = nullptr;
  };

  // change events
  State& on(const string name, voidfunction onenter, voidfunction onexit = nullptr) {
    states[name].onenter = onenter;
    if(onexit) states[name].onexit = onexit;
    // assert(states.count(name));    
    // states[name].onenter();
    return states[name];
    log(states[name].state);
  }

  void remove(const string name) { states.erase(name); }

  // inline bool is(const string name) { return *current == states[name];  }
  // inline bool operator[](const string name) { return is(name); }

  inline State& operator[](const string name) { 
    assert(states.count(name));    
    return states[name]; 
  }

  // inline State& operator()(const string name, voidfunction onenter= nullptr, voidfunction onexit = nullptr) { 
  //   return on(name, onenter, onexit); }
  inline State& operator()(const string name) { 
    assert(states.count(name));    
    states[name].onenter();
    return states[name];
  }

  friend ostream& operator << ( ostream& os, const StringMachine& m) { 
    os << "[";
    auto it = m.states.begin(); //std::begin is a free function in C++11
    while(it != m.states.end()) {
      if (it != m.states.begin()) os << ",";
      os << it->second;
      it++;
    }
    return os<<"]";
  }
};

//================================================================= TEST(StringMachine)
TEST(StringMachine) {
  map<string, int> counter;

  StringMachine M("grn red yel"); // M([&](StringMachine& ss){ ss.parse("red->grn->yel->red"); });

  M.on("red", 
    [&]{ out(BLD,RED,"⏺"); counter["red"]++; },  // onenter
    [&]{ out(DIM,RED,"⏺"); counter["red"]--; }); // onexit

  M.on("yel",
    [&]{ out(BLD,YEL,"⏺"); counter["yel"]++; }, 
    [&]{ out(DIM,YEL,"⏺"); counter["yel"]--; });

  M.on("grn",
    [&]{ out(BLD,GRN,"⏺"); counter["grn"]++; }, 
    [&]{ out(DIM,GRN,"⏺"); counter["grn"]--; });

  CHECK(sstr(M)=="[grn,red,yel]");

  State S = M("red");
  CHECK(S["red"] && !S["yel"] && !S["grn"]);
  CHECK(counter["red"] == 1); // side effect of names
  
  S = M["grn"]; // functor call changes the state
  CHECK(S["grn"]); // bracket query the state returning true if match
  CHECK(counter["grn"] == 1); 
  CHECK(counter["red"] == 0); 
  CHECK(!S["red"]);

  S = M["yel"];
  CHECK(counter["yel"] == 1);
  CHECK(S["yel"]);
  CHECK(!S["red"]);

  S = M["red"];
  CHECK(counter["red"] == 1 && counter["yel"] == 0);
  CHECK(S["red"]); CHECK(!S["yel"]);
  CHECK(S=="red");

  CHECK(S!="grn");
  CHECK(sstr(S)=="red"); // ostream operator

  CHECK(counter["red"]==1);
  CHECK(counter["grn"]==0);
  CHECK(counter["yel"]==0); 

  // CHECK_EXCEPTION(S["blue"]); // non existent state raises an exception 
}

}// UniQ • Released under GPL 3 licence
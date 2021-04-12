//==============================================================================
// StringMachine is a state machine
// StringMachine or a collection of States
//==============================================================================
#pragma once
#include "call.h"
#include "utils.h"
#include "test.h"
namespace uniq {

#define VAR(V,init) __typeof(init) V=(init)
#define EACH(I,C) for(VAR(I,(C).begin());I!=(C).end();I++)
// #define foreach(V, lambda) do { for_each(V.begin(), V.end(), lambda) } while(0)

//======================================================================== State
struct State {
  string state;
  voidfunction onenter = [&]{ };
  voidfunction onexit = [&]{ };

  ~State(){ 
    onexit(); };

  // optional<vector<State&>> transitions;

  // void exit(State& other) { onexit(); } 
  State& enter(State& other) { other.onexit(); *this=other; onenter(); return *this; } 

  inline bool operator[](const string name) { return state == name; }

  inline bool operator==(const string name) { return state == name; }
  inline bool operator!=(const string name) { return state != name; }
  // inline bool operator=(const string name) { 

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
};
ostream& operator<<(ostream& os, State& s) { return os << s.state; }

//================================================================== StringMachine
typedef function<void(State, State)> StateEnterFunction;
typedef function<void(State)> StateExitFunction;

class StringMachine { // public StateMachine<string>
 private:
  optional<StateEnterFunction> onenter;
  optional<StateExitFunction> onexit;
  map<string, State> states;
 public:
  StringMachine(){};

  StringMachine(string names) { 
    onenter = [&](State state, State from) { };
    onexit = [&](State from) { };    
    parse(names); 
  };

  StringMachine(function<void(StringMachine&)> init) { init(*this); };
    
  ~StringMachine(){ 
    for(auto [key,s] : states)
      s.onexit=[]{};
    // states.clear(); 
  };

  void parse(string names=""){
    vector<string> tr;
    auto tokens = split(names, ' ');
    states.clear();
    for(auto &t : tokens)
      states[t].state = t;
    // foreach(, [&](string const& s){ states[t] = {}; });
  }

  // change events
  void on(const string name, voidfunction onenter, voidfunction onexit = nullptr) {
    states[name].onenter = onenter;
    if(onexit) states[name].onexit = onexit;
  }

  void remove(const string name) {
    states.erase(name);
  }

  inline State& operator[](const string name) { 
    assert(states.count(name));    
    return states[name]; 
  }

  inline State& operator()(const string name) { 
    assert(states.count(name));    
    states[name].onenter();
    return states[name];
  }
};


//================================================================= TEST(StringMachine)
TEST(StringMachine) {
  map<string, int> counter;

  // StringMachine M("red grn yel red");
  StringMachine M([&](StringMachine& ss){ ss.parse("red grn yel red"); });

  M.on("red", 
    [&]{ out(BLD,RED,"⏺"); counter["red"]++; },  // onenter
    [&]{ out(DIM,RED,"⏺"); counter["red"]--; }); // onexit

  M.on("yel",
    [&]{ out(BLD,YEL,"⏺"); counter["yel"]++; }, 
    [&]{ out(DIM,YEL,"⏺"); counter["yel"]--; });

  M.on("grn",
    [&]{ out(BLD,GRN,"⏺"); counter["grn"]++; }, 
    [&]{ out(DIM,GRN,"⏺"); counter["grn"]--; });

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
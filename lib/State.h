//==============================================================================
// State is { data + events }
// StateMachine or a collection of States
//==============================================================================
#pragma once
#include "call.h"
#include "utils.h"
#include "test.h"
namespace uniq {
template <typename T> class StateMachine;

//======================================================================== State<T>
template <typename T> struct State{
  T state = T {};
  voidfunction onenter = nullptr;
  voidfunction onexit = nullptr;
  int counter = 0;
  // vector<State<T>&> transitions;

  inline bool operator[](const T s) { return state == s; }

  inline bool is(const T s) { return state == s; }
  inline bool operator==(const T s) { return is(s); }
  inline bool operator!=(const T s) { return state != s; }

  inline bool is(const State<T>& other) { return other.state == state; }
  inline bool operator==(const State<T>& other) { return is(other); }
  inline bool operator!=(const State<T>& other) { return other.state != state; }

  friend StateMachine<T>;
  friend std::ostream& operator << ( std::ostream& os, const State<T>& s) { return os << s.state; }
};

//================================================================== StateMachine
template <typename T> 
class StateMachine : public vector<State<T>>{
 protected:
  function<void(State<T>&, State<T>&)> onchange = nullptr;
  map<T, State<T>> states;
  State<T> *current;
 public:
  // StateMachine() { };
  // StateMachine(function<void(StateMachine&)> init) { init(*this); };
    
  ~StateMachine(){ if(current) current->onexit(); };

  State<T>& on(const T s, voidfunction onenter= nullptr, voidfunction onexit = nullptr) {
    if(onenter) states[s].onenter = onenter;
    if(onexit) states[s].onexit = onexit;
    if(current) current->onexit();
    current = &states[s];
    current->onenter();
    current->counter++;
    return *current;
  }
  inline State<T>& operator()(const T s, voidfunction onenter= nullptr, voidfunction onexit = nullptr) { 
    return on(s, onenter, onexit); }

  inline bool is(const T s) { 
    if(!states.count(s)) throw invalid_argument(s);
    return *current == states[s];  
  }

  inline bool operator[](const T s) { return is(s); }

  void remove(const T s) { states.erase(s); }

  friend ostream& operator << ( ostream& os, const StateMachine& m) { 
    os << "[";
    auto it = m.states.begin();
    while(it != m.states.end()) {
      if (it != m.states.begin()) os << ",";
      os << it->second;
      it++;
    }
    return os<<"]";
  }
};

//================================================= StringMachine : StateMachine
class StringMachine: public StateMachine <string> {
public:
  void parse(string names=""){
    vector<string> tr;
    auto tokens = split(names, ' ');
    states.clear();
    for(auto &t : tokens) states[t].state = t;
  }

  StringMachine() : StateMachine<string>() { }

  StringMachine(const string names) : StateMachine<string>() { 
    // log(parse(names), this->current->state);
    parse(names); 
  };
};

//================================================================== TEST(State)
TEST(State) {
  map<string, int> counter;

  StringMachine M("red yel grn"); //to do: M("red->grn->yel->red");

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

  CHECK(M["grn"] && !M["yel"] && !M["red"]);

  auto S = M("red");
  CHECK(S["red"] && !S["yel"] && !S["grn"]);
  CHECK(M["red"] && !M["yel"] && !M["grn"]);
  CHECK(counter["red"] == 1); // side effect of names
  
  S = M("yel");
  CHECK(counter["yel"] == 1);
  CHECK(S["yel"]); CHECK(M["yel"]); 

  M("grn");
  CHECK(S["yel"]); CHECK(M["grn"]);
  
  CHECK(counter["grn"] && !counter["red"]); 

  S = M("red");
  CHECK(counter["red"] && !counter["grn"]);
  CHECK(S["red"]); CHECK(S=="red"); CHECK(S!="grn" && !M["grn"]);
  CHECK(sstr(S)=="red"); // ostream operator

  CHECK(counter["red"]==1);
  CHECK(counter["grn"]==0);
  CHECK(counter["yel"]==0); 

  CHECK_EXCEPTION(M["blue"]); // invalid_argument
}
}// UniQ • Released under GPL 3 licence
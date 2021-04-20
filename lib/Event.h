#pragma once
namespace uniq {

class Event{
  struct functionCompare{ bool operator()(const voidfunction a, const voidfunction b) const { return &a < &b; }; };
  set<voidfunction, functionCompare> subscribers;
public:
  void listen(const voidfunction f);
  void remove(const voidfunction f);
  void emit();
};

class EventEmitter{
  map<string, Event> events;
public:
  void listen(const string name, const voidfunction f); //, Args &&...args) {
  void remove(const string name, const voidfunction f);
  void emit(string name);
};

}
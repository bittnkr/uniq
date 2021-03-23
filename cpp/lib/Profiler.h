#pragma once
#include "uniq.h"
namespace uniq {

struct Profiler;

struct ProfilerTrace : Named { // trace record
  Profiler* session;
  u32 threadId;
  Time time, duration=0;
  ProfilerTrace (const string name, Profiler* session): Named(name), session(session){
    threadId = std::hash<thread::id>{}(this_thread::get_id()); 
  }
};

struct ProfilerProbe : Actor { // time probe
  ProfilerTrace &trace;
  ProfilerProbe(ProfilerTrace& trace) : trace(trace) { trace.time = CpuTime(); }
  ~ProfilerProbe() { trace.duration = trace.time(CpuTime()); }
};

struct Profiler : Named { // Controler

  ~Profiler() { save(); }

  vector<shared_ptr<ProfilerTrace>> traces; // todo: stats in a map with function names
  mutex vectorLock;

  void save() {
    // auto log = log.open(name + "-profiler.json");

    log("{\"otherData\": {}, \"traceEvents\":[");

    for (auto it=traces.begin(); it != traces.end() ;)
    {
      auto p = *it;
      out(" {", 
        "\"cat\":\"", p->session->name,"\"",
        ", \"name\":\"", p->name,"\"",
        ", \"ts\":", integer(p->time.micros()),
        ", \"dur\":", integer(p->duration.micros()),
        ", \"ph\":", "\"X\"",
        ", \"pid\":", 0,
        ", \"tid\":", p->threadId, "}"
      );
      if(++it == traces.end()) break;
      log(",");
    };
    log("\n]}");
    // log.close();
  }

  ProfilerProbe probe(const string name) {
    shared_ptr<ProfilerTrace> t(make_shared<ProfilerTrace>(name, this));
    lock_guard<mutex> lock(vectorLock);
    traces.push_back(t);
    return ProfilerProbe (*t);
  };
};

// simple constructor & session manager
Profiler& profiler(const string session="main"){
  static map<string, Profiler> sessions;
  auto r =  &sessions[session];
  if (r->name != session){ r->name = session; }
  return *r;
}

}// uniq • Released under GPL 3.0
#pragma once
#include "uniq.h"
namespace uniq {

struct Profiler;

struct TimeTrace : Named { // trace record
  Profiler* session;
  u32 threadId;
  Time start, duration=0;
  TimeTrace (const string name, Profiler* session): Named(name), session(session){
    threadId = std::hash<thread::id>{}(this_thread::get_id()); 
  }
};

struct TimeProbe { // update the trace on destruction
  TimeTrace &trace;
  TimeProbe(TimeTrace& trace) : trace(trace) { trace.start = CpuTime(); }
  ~TimeProbe() { trace.duration = trace.start(CpuTime()); }
};

struct Profiler : Named { // Controler

  ~Profiler() { save(); }

  vector<shared_ptr<TimeTrace>> traces; // todo: stats in a map with function names
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
        ", \"ts\":", integer(p->start.micros()),
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

  TimeProbe timeit(const string name) {
    shared_ptr<TimeTrace> t(make_shared<TimeTrace>(name, this));
    lock_guard<mutex> lock(vectorLock);
    traces.push_back(t);
    return TimeProbe (*t);
  };
};

// simple constructor & session manager
Profiler& profiler(const string session="main"){
  static map<string, Profiler> sessions;
  auto r =  &sessions[session];
  if (r->name != session){ r->name = session; }
  return *r;
}

#define timeit() auto _timeit = profiler().timeit(__FUNCTION__);

void test_prof_a() { timeit() usleep(50); }
void test_prof_b() { timeit() usleep(200); }
void test_prof_f() { timeit() usleep(3000); test_prof_a(); }
void test_prof_h() { timeit() usleep(2000); test_prof_a(); test_prof_b(); }
void test_prof_g() { timeit() usleep(1000); test_prof_b(); test_prof_a(); };

TEST(Profiler){
  vector<thread> workers;
  workers.push_back(thread(test_prof_f));
  workers.push_back(thread(test_prof_h));
  workers.push_back(thread(test_prof_g));
  for (auto& w : workers) w.join();

  // run(f); run(g); run(h);
  // join();
  // profiler().save();
  CHECK(profiler().traces.size()==8);
}
}// uniq • Released under GPL 3.0
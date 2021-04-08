//==============================================================================
// Profiler • A collection of TimeTraces
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

//==================================================================== TimeTrace
struct TimeTrace : Named { // trace record
  u32 threadId;
  Time start, duration=0;
  // TimeTrace (TimeTrace& t) { name=t.name; threadId=t.threadId; start=t.start, duration=t.duration; }
  TimeTrace (const string name=""): Named(name){
    start = CpuTime();
    threadId = std::hash<thread::id>{}( this_thread::get_id() ); 
  }
};

//===================================================================== Profiler
struct Profiler : Named { 

  OpenQueue<shared_ptr<TimeTrace>> traces; 

  // ~Profiler() { save(); }

  void save() {
    // auto log = log.open(name + "-profiler.json");

    log("{\"otherData\": {}, \"traceEvents\":[");

    shared_ptr<TimeTrace> t;
    while(traces.pop(t)){
      out(" {", 
        "\"cat\":\"", name,"\"",
        ", \"name\":\"", t->name,"\"",
        ", \"ts\":", integer(t->start.micros()),
        ", \"dur\":", integer(t->duration.micros()),
        ", \"ph\":", "\"X\"",
        ", \"pid\":", 0,
        ", \"tid\":", t->threadId, "}"
      );
      if(!traces.empty()) log(",");
    };
    log("\n]}");
    // log.close();
  }
};

//============================================================ profiler(session)
Profiler& profiler(const string session=""){ // singleton session manager
  static map<string, Profiler> sessions;
  auto r =  &sessions[session];
  if (r->name != session){ r->name = session; }
  return *r;
}

//==================================================================== TimeProbe
struct TimeProbe { // update the trace on release
  TimeTrace &trace;

  TimeProbe(TimeTrace &trace) : trace(trace) { }
  ~TimeProbe() { trace.duration = CpuTime(trace.start); }
};

//====================================================================== probe()
TimeProbe probe(const string name=__FUNCTION__, const string session="") {
  shared_ptr<TimeTrace> trace(make_shared<TimeTrace>(name));
  profiler(session).traces.push(trace);
  return TimeProbe(*trace);
};

#define timeit() auto _probe = probe();

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
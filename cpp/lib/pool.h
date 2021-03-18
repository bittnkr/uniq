#pragma once
#include "uniq.h"
namespace uniq {

// ThreadPool =================================================================
// #include "worker.h"
class ThreadPool : Actor {
 protected:
  vector<thread> workers;
  // vector<uniq::Worker&> workers;

 public:
  Queue<voidfunction> todo;

  ThreadPool(int size = 0) { if(size) start(size); else running = 0; };

  void start(int size = 0) { 
    if (running) return;

    if (!size) size = thread::hardware_concurrency();
    for (auto i = 0; i < size; i++) {
      // workers.push_back(new Worker(todo));
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
    };
    Actor::start();
  }

  ~ThreadPool() { stop(); }

  void stop() override { todo.running = 0; running = 0; }


  void join() { 
    for (auto &w : workers) if(w.joinable()) w.join();
    stop();
  }

  int size() { return workers.size(); }
  void sleep(int ms=0) {if (ms==0) sched_yield(); else this_thread::sleep_for(chrono::milliseconds(ms)); }

  inline void wait(int id) { while(todo.done() < id) sleep(); }

  // void wait(int id) {
  //   for (auto &w : workers) {
  //     while (w.done() < id) sleep(1);
  //   };
  // }

  bool showstats = false;
  
  void worker(int id) {
    int done = 0;
    Time t, total(0);
    voidfunction f;
    while (todo.pop(f)){
      t = procTime();
      f();
      total += t(procTime());
      done++;
    };
    if(showstats) logn("\n", colorcode(id), sstr("worker[", id, "] handled ", done, " messages in ", total));
  };

  template <typename Func, typename... Args>
  inline int run(Func &&f, Args &&...args) {
    if(!running) start();
    // voidfunction vf = [this]()->void { f(args...); }
    voidfunction vf = bind(forward<Func>(f), forward<Args>(args)...);
    return todo.push(vf);
  }

  // template <typename Func>
  // map<string, Func> funcs;
  // mutex mutex;

  // template <typename Func>
  // void addfunc(const string name, Func &&f){
  //   lock_guard<mutex> lock(mutex);
  //   funcs.insert({name, f});
  // };

  // template <typename Func, typename... Args>
  // inline int run(const string name, Args &&...args) {
  //   Func f = funcs[name]; assert(f);
  //   return run(f, args...);
  // }

  // template <typename Func, typename... Args>
  // inline int atomic(Func &&f, Args &&...args) {
  //   voidfunction vf = bind(forward<Func>(f), forward<Args>(args)...);
  //   u64 address = *(long *)(char *)&vf; // https://stackoverflow.com/a/44236212/9464885 
  //   // cout << address; 
  //   address = rehash((u64)address);
  //   // cout << " " << address << "\n";
  //   return workers[address % size()]->push(vf);
  // }
};

ThreadPool pool(0);

// run ========================================================================
template <typename Func, typename... Args>
inline int run(Func&& f, Args&&... args) {
  return pool.run(f, args...);
}

// tests =======================================================================
atomic<int> rounds = 0;
void test_ping(int v);

void test_pong(int v) { if (v) run(test_ping, v - 1); else pool.stop(); }
void test_ping(int v) { run(test_pong, v); rounds++; }

void test_Pool() {
  run(test_ping, 999); // start the flow
  WAIT(rounds==1000);
  CHECK(true);
}
}// uniq • Released under GPL 3.0
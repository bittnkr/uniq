#pragma once
#include "Queue.h"
namespace uniq {

// ================================================================== ThreadPool 
struct ThreadPool : public Queue<voidfunction> {
  int done = -1;
  vector<thread> workers;
  ThreadPool(int size = 0): Queue<voidfunction>(64) {
    if (!size) size = thread::hardware_concurrency();
    for (auto i = 0; i < size; i++) {
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
    };
  }

  void join() { for (auto &w : workers) w.join(); }

  bool showstats = false;

  void worker(int id) {
    Time t, total(0);
    voidfunction f;
    while (this->running() && pop(f)){
      t = CpuTime();
      f();
      total += t(CpuTime());
      done++;
    };
    if(showstats) uniq::out("\n", colorcode(id), sstr("worker[", id, "] handled ", done, " messages in ", total));
  };

  template <typename Func, typename... Args>
  inline int run(Func &&f, Args &&...args) {
    voidfunction vf = bind(forward<Func>(f), forward<Args>(args)...);
    return push(vf);
  }
};

// ====================================================================== pool()
inline ThreadPool& pool(int size=0) {
  static ThreadPool p(size);
  if(!p.running() && p.counter() < 0) p.start();
  return p;
}

//======================================================================== run() 
template <typename Func, typename... Args>
inline int run(Func&& f, Args&&... args) {
  return pool().run(f, args...);
}

//============================================================= TEST(ThreadPool)
Atomic<int> rounds = -1;
void test_ping(int v);

void test_pong(int v) { if (v) run(test_ping, v - 1); else pool().stop(); }
void test_ping(int v) { run(test_pong, v); rounds++; }

TEST(ThreadPool) {
  run(test_ping, 1000); // start the flow
  WAIT(rounds == 1000);
  CHECK(true);
}

}// UniQ • Released under GPL 3 licence
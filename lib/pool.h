// Hey @elonmusk! Why don't you use this in your next rocket?
#pragma once
#include "queue.h" // the queue
namespace uniq {

// ThreadPool =================================================================
// #include "worker.h"
struct ThreadPool : public Queue<voidfunction> {
  int done = -1;
  vector<thread> workers;
  // vector<uniqWorker&> workers;
  // ThreadPool(int queueSize=64, int poolSize = 0): Queue<voidfunction>(queueSize) {
  //   if (!poolSize) poolSize = thread::hardware_concurrency();
  //   for (auto i = 0; i < poolSize; i++) {
  ThreadPool(int size = 0): Queue<voidfunction>(64) {
    if (!size) size = thread::hardware_concurrency();
    for (auto i = 0; i < size; i++) {
      // workers.push_back(new Worker(this));
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
    };
  }

  void join() { for (auto &w : workers) w.join(); }

  void wait() { 
    // log(done, " ", counter());
    while(!empty() && done != counter() ) 
      sleep(); 
  }

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
    // voidfunction vf = [this]()->void { f(args...); }
    voidfunction vf = bind(forward<Func>(f), forward<Args>(args)...);
    return push(vf);
  }

  // int size() { return workers.size(); }
  // void sleep(int ms=0) {if (ms==0) sched_yield(); else this_thread::sleep_for(chrono::milliseconds(ms)); }

  // void wait(int id) {
  //   for (auto &w : workers) {
  //     while (w.done() < id) sleep(1);
  //   };
  // }

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

// ====================================================================== pool()
inline ThreadPool& pool(int size=0) {
  static ThreadPool p(size);
  if(!p.running() && p.counter() < 0) p.start();
  return p;
}

// run ========================================================================
template <typename Func, typename... Args>
inline int run(Func&& f, Args&&... args) {
  return pool().run(f, args...);
}

//=================================================================== TEST(Pool)
Atomic<int> rounds = -1;
void test_ping(int v);

void test_pong(int v) { if (v) run(test_ping, v - 1); else pool().stop(); }
void test_ping(int v) { run(test_pong, v); rounds++; }

TEST(Pool) {
  run(test_ping, 50'000); // start the flow
  WAIT(rounds==50'000)
  // pool().wait();
  // pool().stop();
  // pool().join();
  CHECK(rounds == 50'000);
}//*/
}// UniQ • Released under GPL 3 licence
#pragma once
#include "uniq.h"
namespace uniq {

Queue<voidfunc> todo;

#define WAIT(condition) while(!(condition)) { sched_yield(); }
void sleep(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }

// ThreadPool =================================================================

class ThreadPool {
 protected:
  vector<thread> workers;

 public:
  ThreadPool(int size = 0) {
    if (!size) size = thread::hardware_concurrency();
    for (auto i = 0; i < size; i++) {
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
   }
  };

  ~ThreadPool() {
    for (auto i = 0; i < workers.capacity(); i++) workers[i].join();
  }

  void stop() { todo.running = false; };
  int size() { return workers.size(); }
  int counter() { return todo.counter(); }

  void worker(int color) {
    // while (todo.pop(f)) f();
    int size = 0;
    voidfunc f;
    while (true) {
      f = todo.pop();
      if (!todo.running) break;
      f();
      size++;
    };
    // cout << colorcode(color) + "thread" + to_string(color) + ": " + to_string(size) + "\n";
  };
};

ThreadPool pool;

// run ========================================================================
template <typename Func, typename... Args>
inline void run(Func&& f, Args&&... args) {
  voidfunc job = bind(forward<Func>(f), forward<Args>(args)...);
  todo.push(job);
}
// tests =======================================================================
#include "test.h"
atomic<int> rounds = 0;
void test_ping(int v);

void test_pong(int v) { if (v) uniq::run(test_ping, v - 1); else uniq::pool.stop(); }
void test_ping(int v) { uniq::run(test_pong, v); rounds++; }

void test_pool() {
  uniq::run(test_ping, 999); // start the flow
  WAIT(!todo.running);
  CHECK(rounds == 1000);
}
}// uniq • Released under GPL 3.0
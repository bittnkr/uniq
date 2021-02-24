#pragma once
// namespace uniq {
#include "common.h"
#include "queue.h"

typedef int JobID;
typedef function<void()> Job;

uniq::Queue<Job> Q;

#define wait(condition) while(!(condition)) { sched_yield(); }
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

  void stop() {
    Q.stop = 1;
    sleep(100);  // alow stdout printing
  };

  int size() { return workers.size(); }
  int nextJobId() { return Q.nextJobId(); }

  void worker(int color) {
    // while (Q.pop(f)) f();
    int size = 0;
    Job f;
    while (true) {
      f = Q.pop();
      if (Q.stop) break;
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
  Job job = bind(forward<Func>(f), forward<Args>(args)...);
  Q.push(job);
}
// };// uniq

// tests =======================================================================
namespace test {
  #include "common.h"
  atomic<int> rounds = 0;
  void test_ping(int v);

  void test_pong(int v) { if (v) run(test_ping, v - 1); else pool.stop(); }
  void test_ping(int v) { run(test_pong, v); rounds--; }

  void test_pool() {
    run(test_ping, 999); // start the flow
    wait(Q.stop);
    CHECK(rounds == -1000);
  }
}// Part of the UniQ libray • Released under GNU 3.0

#pragma once
#include "common.h"
#include "queue.h"

typedef int JobID;
typedef function<void()> Job;

Queue<Job> Q;

#define wait(condition) while(!(condition)) { sched_yield(); }
void sleep(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }

// ThreadPool =================================================================

class ThreadPool {
 protected:
  vector<thread> workers;

 public:
  ThreadPool(int count = 0) {
    if (!count) count = maxThreads();
    for (auto i = 0; i < count; i++)
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
  }

  ~ThreadPool() {
    for (auto i = 0; i < workers.capacity(); i++) workers[i].join();
  }

  unsigned int maxThreads() { return thread::hardware_concurrency(); }

  void stop() {
    Q.stop = 1;
    sleep(100);  // alow stdout printing
  };

  int nextJobId() { return Q.nextJobId(); }

  void worker(int color) {
    // while (Q.pop(f)) f();
    int count = 0;
    Job f;
    while (true) {
      f = Q.pop();
      if (Q.stop) break;
      f();
      count++;
    };
    // cout << colorcode(color) + "thread" + to_string(color) + ": " + to_string(count) + "\n";
  };
};

ThreadPool pool;

// run ========================================================================
template <typename Func, typename... Args>
inline void run(Func&& f, Args&&... args) {
  Job job = bind(forward<Func>(f), forward<Args>(args)...);
  Q.push(job);
}

#include <cstddef>
#include <tuple>
#include <utility>

template <typename TupT, size_t... Is>
auto combine(TupT&& tup, index_sequence<Is...>) {
  return std::get<sizeof...(Is)>(tup)(std::get<Is>(forward<TupT>(tup))...);
}

template <typename... Ts>
auto call(Ts&&... ts) {
  return combine(forward_as_tuple(forward<Ts>(ts)...),
                 make_index_sequence<sizeof...(Ts) - 1>{});
}

// tests =======================================================================
atomic<int> rounds = 0;
void test_ping(int v);

void test_pong(int v) { if (v) run(test_ping, v - 1); else pool.stop(); }
void test_ping(int v) { run(test_pong, v); rounds--; }

void test_pool() {
  run(test_ping, 999); // start the flow
  wait(Q.stop);
  CHECK(rounds = -1000);
}
// Part of the uniQ Libray - Released under GNU 3.0 =============================

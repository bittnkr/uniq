#pragma once
#include <functional>
#include <iostream>
#include <thread>

#include "color.h"
#include "uniq.h"
using namespace std;

typedef unsigned long long u64;
typedef unsigned long u32;

typedef int JobID;
typedef function<JobID()> Job;

Queue<Job> Q;

void sleep(int ms) { this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// ThreadPool ================================================

class ThreadPool {
 protected:
  vector<thread> workers;

 public:
  ThreadPool(int count = 0) {
    if (!count) count = maxThreads();
    for (int i = 0; i < count; i++)
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
  }

  ~ThreadPool() {
    // stop();
    for (int i = 0; i < workers.capacity(); i++) workers[i].join();
  }

  unsigned int maxThreads() { return thread::hardware_concurrency(); }

  int stop() {
    sleep(100); // to alow stdout printing
    return Q.stop = 1;
  };

  int nextJobId() { return Q.nextJobId(); }

  void worker(int color) {
    // while (Q.pop(f)) f();
    int res = 0, count = 0;
    Job f;
    while (true) {
      f = Q.pop();
      if (Q.stop) break;
      res = f();
      count++;
      if (res != 0 && res % (int)1e6 == 0) {
        cout << colorcode(color) << res << "\n";
      }
    };

    // cout << colorcode(color) + "thread" + to_string(color) + ": " +
    // to_string(count) + "\n";
  }

  // using task_t = function<T(Args...)>;
  // auto bound = bind(forward<task_t>(task), forward<Args>(args)...);

  // template <typename Functor>
  // void f(Functor functor) {
  //   cout << functor(10) << endl;
  // }
};

ThreadPool pool;

// run ================================================

template <typename Func, typename... Args>
inline JobID run(Func &&f, Args &&... args) {
  Job job = bind(forward<Func>(f), forward<Args>(args)...);
  return Q.push(job);
}

// call using combinator ===============================

#include <cstddef>
#include <tuple>
#include <utility>

template <typename TupT, size_t... Is>
auto combine(TupT &&tup, index_sequence<Is...>) {
  return std::get<sizeof...(Is)>(tup)(std::get<Is>(forward<TupT>(tup))...);
}

template <typename... Ts>
auto call(Ts &&... ts) {
  return combine(forward_as_tuple(forward<Ts>(ts)...),
                 make_index_sequence<sizeof...(Ts) - 1>{});
}

// Released under GNU 3.0

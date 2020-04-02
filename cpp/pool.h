#pragma once
#include <functional>
#include <iostream>
#include <thread>

#include "color.h"
#include "uniq.h"
using namespace std;

typedef int JobID;
typedef function<JobID()> Job;

// ThreadPool ================================================

class ThreadPool {
 protected:
  vector<thread> workers;
  Queue<Job> Q;

 public:
  ThreadPool(int count = 0) {
    if (!count) count = maxThreads() / 2;
    for (int i = 0; i < count; i++)
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
  }

  ~ThreadPool() {
    // stop();
    for (int i = 0; i < workers.capacity(); i++) workers[i].join();
  }

  unsigned int maxThreads() { return thread::hardware_concurrency(); }

  int stop() { return Q.stop = 1; };

  int taskId() { return Q.taskId(); }

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

    cout << colorcode(color) + "thread" + to_string(color) + ": " +
                to_string(count) + "\n";
  }

  // using task_t = function<T(Args...)>;
  // auto bound = bind(forward<task_t>(task), forward<Args>(args)...);

  // template <typename Functor>
  // void f(Functor functor) {
  //   cout << functor(10) << endl;
  // }

  template <typename Func, typename... Args>
  void run(Func &&f, Args &&... args) {
    Job job = bind(forward<Func>(f), forward<Args>(args)...);
    Q.push(job);
  }

  // template <typename Func, typename... Args>
  // JobID wait(Func &&f, Args &&... args) {
  //   Job job = bind(forward<Func>(f), forward<Args>(args)...);
  //   return Q.push(job);
  // }
};

ThreadPool pool;

template <typename Func, typename... Args>
inline void run(Func &&f, Args &&... args) {
  pool.run(f, args...);
}

// Batch ================================================
// class Batch {
//  protected:
//   vector<thread> workers;
//   Queue<Job> Q;

//  public:
//   Batch(int size = 0) {
//     if (!size) size = pool.maxThreads() / 2;
//     for (int i = 0; i < size; i++)
//       workers[i] = thread(&ThreadPool::worker, this, i + 1);
//   }

//   template <typename Functor>
//   void jobDone(Functor functor) {
//     cout << functor(10) << endl;
//   }

//   template <typename Func, typename... Args>
//   void run(Func &&f, Args &&... args) {
//     return pool.run(f, args...);
//   }
// }

// Released under GNU 3.0

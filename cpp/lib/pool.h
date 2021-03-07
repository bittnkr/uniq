#pragma once
#include "uniq.h"
namespace uniq {

typedef function<void()> voidfunction;


// ThreadPool =================================================================
class ThreadPool : Actor {
 protected:
  vector<thread> workers;

 public:
  ThreadPool(int size = 0) { /*start(size);*/ };

  ~ThreadPool() { join(); }

  Queue<voidfunction> todo;

  void start(int size = 0) { 
    if (!size) size = thread::hardware_concurrency();
    for (auto i = 0; i < size; i++) {
      workers.push_back(thread(&ThreadPool::worker, this, i + 1));
    }
  }

  void join() { 
    for (auto &w : workers) w.join();
    todo.running = false;
  };

  void stop() { todo.running = false; };
  int size() { return workers.size(); }
  int done() { return todo.done(); }

  void worker(int id) {
    int done = 0;
    voidfunction f;
    while (todo.pop(f)){
      f();
      done++;
    };
    // log(colorcode(id), sstr("worker[", id, "] ", done));
  };

  template <typename Func, typename... Args>
  inline int run(Func &&f, Args &&...args) {
    // voidfunction vf = [this]()->void { f(args...); }
    voidfunction vf = bind(forward<Func>(f), forward<Args>(args)...);
    return todo.push(vf);
  }
};

ThreadPool pool;

// run ========================================================================
template <typename Func, typename... Args>
inline int run(Func&& f, Args&&... args) {
  return pool.run(f, args...);
  cout<<"done\n";
}
// tests =======================================================================
#include "test.h"
atomic<int> rounds = 0;
void test_ping(int v);

void test_pong(int v) { if (v) run(test_ping, v - 1); else pool.stop(); }
void test_ping(int v) { run(test_pong, v); rounds++; }

void test_pool() {
  pool.start();
  run(test_ping, 999); // start the flow
  pool.join();
  CHECK(rounds == 1000);
}
}// uniq • Released under GPL 3.0
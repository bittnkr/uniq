//==============================================================================
// Worker • A Queue of functions whose enter() runs in its own thread.
//==============================================================================
#pragma once
namespace uniq {

thread_local int TaskID = 0;  // the id of the current TaskID TaskID

class Worker : public Queue<voidfunction>{ //, public Id { 
 private:
  thread thrd;
 public:
  Worker(int queueSize = 1) : Queue<voidfunction>(queueSize) {
    this->beat = [&]{
      voidfunction f;
      while (this->running()) {
        try {
          while ((TaskID = pop(f))) {
            f();
            // counter++;
          }
        } catch (...) {
          handle_exception();
        }
        sleep(1);
      }
    };
    // thrd = thread(this->enter, this);
    thrd = thread(&Worker::loop, this);
    // workers.push_back(thread(&ThreadPool::worker, this, i + 1));
  }

  void loop(){ this->beat(); }

  template <typename Func, typename... Args>
  inline int run(Func&& f, Args&&... args) {
    return Queue::push(bind(forward<Func>(f), forward<Args>(args)...));
  }

  // template <typename Func, typename... Args>
  // inline int chain(int id, Func&& f, Args&&... args) {
  //   auto fchain = [=]{ }
  //   return Queue::push(bind(forward<Func>(f), forward<Args>(args)...));
  // }

  void join() { thrd.join(); }

  Worker(const Worker&) = delete;  // no copy constructor
  Worker& operator=(const Worker& w) { return *this; }

  // cout << colorcode(color) + "thread" + to_string(color) + ": " +
  // to_string(count) + "\n";
};

// int Worker::nextId = 0;
TEST(Worker) {  //=======================================================
  int X = 0;

  auto w = Worker();

  w.run([&](int n) {
    X = n;
    w.stop();
  }, 1);

  w.join();

  CHECK(X == 1);
}
}// UniQ • Released under GPL 3 licence

//==============================================================================
// Worker • A Queue with a threaded beat()
//==============================================================================
#pragma once
namespace uniq {

thread_local int TaskID = 0;

class Worker : public Queue<voidfunction>{
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
          }
        } catch (...) {
          handle_exception();
        }
        sleep(1);
      }
    };
    thrd = thread(&Worker::loop, this);
  }

  void loop(){ this->beat(); }

  template <typename Func, typename... Args>
  inline int run(Func&& f, Args&&... args) {
    return Queue::push(bind(forward<Func>(f), forward<Args>(args)...));
  }

  void join() { thrd.join(); }

  Worker(const Worker&) = delete;  // no copy constructor
  Worker& operator=(const Worker& w) { return *this; }
};

//================================================================= TEST(Worker)
TEST(Worker) {  
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

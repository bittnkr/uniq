//==============================================================================
// Queue • A lock free multi-reader multi-writer circular buffered queue.
//==============================================================================
#pragma once
namespace uniq {

// ======================================================================= Queue
template <typename T> struct Queue: public Set<T>, public Actor {
private:
  vector<T> buffer;
  vector<char> isfree;
  atomic<int> in, out;
  int mask = 1;

 public:
  Queue(int size=1){
    while (mask < size) mask *= 2;
    buffer = vector<T>(mask, 0);
    isfree = vector<char>(mask, 1);
    out = in = -1; // start in overflow
    mask--; // 01000000 => 00111111
  } 

  // ~Queue(){  stop(); }
  // void stop() override { 
  //   running = false; }

  int push(const T &item, bool wait=true) override {
    int i;
    do {
      i = in;
// Base<std::vector<InterfaceType> >::myOption = 10;
      if((isfull(i) && !wait) || !this->running()) return 0;
      else WAIT(!isfull(i) || !this->running());

    } while (!isfree[i & mask] || !in.compare_exchange_weak(i, i + 1) || !i);

    buffer[i & mask] = item;
    isfree[i & mask] = 0;
    return i;
  }

  int pop(T &item, bool wait=true) override {
    int o;
    do {
      do { o = out; } while (!o && !out.compare_exchange_weak(o,1)); // skip zero

      if((isempty(o) && !wait) || !this->running()) return 0;
      else WAIT(!isempty(o) || !this->running());

    } while (isfree[o & mask] || !out.compare_exchange_weak(o, o + 1));

    item = buffer[o & mask];
    isfree[o & mask] = 1;
    return o;
  }

  bool full() override { return isfull(-1); }
  bool empty() override { return isempty(-1); }

  int size() override { return in-out; }
  int counter() { return out-1; }
  // inline void wait(int c) { while(out < c) sched_yield(); }

  using Actor::start;
  using Actor::stop;
private:
  inline bool isfull(int i = -1) { 
    if (i<0) i = in; 
    i = (i - out) > mask; 
    if(i) this->onfull();
    return i;
  }

  inline bool isempty(int o = -1) { 
    if (o<0) o = out; 
    o = o == in;
    if(o) this->onempty();
    return o; 
  }

  const T& first() { return buffer[in & mask]; }
  const T& last() { return buffer[out & mask]; }
};

}// uniq • Released under GPL 3.0
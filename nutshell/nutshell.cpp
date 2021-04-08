class Queue {
protected:
  vector<int> buffer;
  int size, mask;
  int in, out;
public:
  Queue(int size_ = 64): size(size_) // :size(RoundPowerOf2(size_))
  {
    buffer = vector<int>(size, 0);
    mask = size - 1;
    in = out = 0;
  }  

  void push(int item) {
    assert(item);
    int i;
    do {
      i = in;
      while (i - out == size)
        sched_yield(); // if full, wait for space
    } while (buffer[i & mask] || !compare_and_swap(&in, i, i + 1)); // CWS instruction
    buffer[i & mask] = item;
  }

  int pop() {
    int o;
    do {
      o = out;
      while (o == in)
        sched_yield(); // if empty, wait for item
    } while (!buffer[o & mask] || !compare_and_swap(&out, o, o + 1));
  }
  
  int r = buffer[o &= mask];
  buffer[o] = 0;
  return r;
};
// Released under GPL3, as part of the uniQ project
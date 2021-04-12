int buffer = 0;
int in = 0, out = 0;

void push(int item) {
  assert(item);
  int i;
  do {
    i = in;
    wait(i != out);
  } while (buffer || !compareAndSwap(&in, i, i + 1));

  buffer = item;
  return i;
}

int pop(int &item) {
  int o;
  do {
    o = out;
    wait(o <= in);
  } while (!buffer || !compareAndSwap(&out, o, o + 1));

  int item = buffer;
  buffer = 0;
  return o;
};
}// UniQ • Released under GPL 3 licence
#include <iostream>

#include "pool.h"
using namespace std;

int ping(int v);

int pong(int v) {
  if (v % 1000 == 0) {
    run(ping, v);
  };
  return -1;
}

int ping(int v) {
  for (size_t i = 1; i <= 1000; i++) {
    pool.run(pong, v - i);  // each ping run 1000 pongs
  };
  if (v == 0) pool.stop();
  return v;
}

int main(int argc, char *argv[]) {
  run(ping, 10e6);  // start the flow
  cout << "\ntasks: " << pool.taskId() << "\n";
}
// pingpong test for uniQ Library
// Each ping call 1000 pongs, the last pong call ping(N-1) until zero
// compile using ./build pingpong

#include <iostream>

#include "pool.h"

int ping(int v);

int pong(int v) {
  if (v % 1000 == 0) {
    run(ping, v);
  };
  return -1;
}

int ping(int v) {
  for (size_t i = 1; i <= 1000; i++) {
    run(pong, v - i);  // each ping run 1000 pongs
  };
  if (v == 0) pool.stop();
  return v;
}

int main(int argc, char *argv[]) {
  run(ping, 10e6);  // start the flow
  cout << "\ntasks: " << pool.nextJobId() << "\n";
}
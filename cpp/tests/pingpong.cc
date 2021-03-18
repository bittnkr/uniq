// pingpong test for uniQ Library
// Each ping call 1000 pongs, the last pong call ping(N-1) until zero
// compile using ./build pingpong
#include "uniq.h"
using namespace uniq;

void ping(int v);

void pong(int v) {
  if (v % 10 == 0) {
    run(ping, v);
  };
}

void ping(int v) {
  for (int i = 1; i <= 10; i++) {
    run(pong, v - i);  // each ping run 1000 pongs
  };

  if (v % 100000 == 0) log("count:",v);

  if (v == 0) {
    log("\ntasks: ", pool.todo.done());
    pool.stop();
  };
}

int main() {
  pool.start();
  pool.showstats = true;
  
  run(ping, 1e6); // start the flow

  pool.join();
  
  quick_exit(0); // return 0;  
}
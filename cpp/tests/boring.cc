// pingpong test for uniQ Library
// Each ping call 1000 pongs, the last pong call ping(N-1) until zero
// compile using ./build pingpong
#include "uniq.h"
using namespace uniq;

void ping(int v);

void pong(int v) {
  if (v % 100000 == 0) printf("togo : %d\n", v);
  run(ping, v-1);
}

void ping(int v) {
  if (v > 0) {run(pong, v); return;};
  pool.stop();
  cout << "\ndone: " << pool.done() << "\n";
}

int main(int argc, char* argv[]) {
  pool.start();
  run(ping, 1e6);  // start the flow
}
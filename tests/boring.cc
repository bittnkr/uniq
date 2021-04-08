// boring ping-pong. ping call pong, pong call ping(-1) until zero
// compile using ./build boring
#include "uniq.h"
using namespace uniq;

void ping(int v);

void pong(int v) {
  if (v % 100000 == 0) printf("togo : %d\n", v);
  run(ping, v-1);
}

void ping(int v) {
  if (v > 0) {run(pong, v); return;};
  pool().stop();
}

int main(int argc, char* argv[]) {
  pool().showstats = true;
  Time t;

  run(ping, 1e6);  // start the flow
  
  pool().join();
  log("\n\ntotal done: ", pool().counter(), " tasks in ", t());
  quick_exit(0); // return 0;
}

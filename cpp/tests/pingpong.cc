// pingpong test for uniQ Library
// Each ping call 1000 pongs, the last pong call ping(N-1) until zero
// compile using ./build pingpong
#include "uniq.h"
using namespace uniq;

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

  if (v % 100000 == 0) cout << "count:" << v <<"\n";

  if (v == 0) {
    cout << "\ntasks: " << pool.done() << "\n";
    pool.stop();
  };
  return v;
}

int main(int argc, char* argv[]) {
  run(ping, 1e6); // start the flow
}
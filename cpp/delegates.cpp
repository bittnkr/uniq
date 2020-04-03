// delegation tests

// https://stackoverflow.com/a/40701904/9464885

#include <cstddef>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>

#include "color.h"
#include "pool.h"

using namespace std;

// passing a fuction call forward
u64 spiral(u64 n, u64 min, u64 max) {
  printf("\nspiral(n:%llu, min:%llu, max:%llu)\n", n, min, max);
  return n;
}

template <typename Func, typename... Args>
int run2(Func&& f, Args&&... args) {
  Job job = bind(forward<Func>(f), forward<Args>(args)...);
  return Q.push(job);
}

// combine with variadic & callback
template <typename TupT, size_t... Is>
auto combine3(TupT&& tup, index_sequence<Is...>) {
  return get<sizeof...(Is)>(tup)(get<Is>(forward<TupT>(tup))...);
}

template <typename... Ts>
auto run3(Ts&&... ts) {
  return combine3(forward_as_tuple(forward<Ts>(ts)...),
                  make_index_sequence<sizeof...(Ts) - 1>{});
}

// // template <typename Func, size_t... Ts2>
// template <typename Func, typename... Args>
// auto run4(Func&& f, Args&&... args) {
//   return combine3(forward_as_tuple(forward<Args>(args)...),
//                   make_index_sequence<sizeof...(Args) - 1>{});
// }

int main() {
  printf("Press ctrl+c to stop.\n");

  u64 result = 0;
  run(spiral, 1, 2, 3);
  run2(spiral, 1, 2, 3);

  //  calling a lambda as first param
  auto lambda = [&](u64 a, u64 b, u64 c) {
    ostringstream ss;
    printf("\nrun2(lambda,%llu,%llu,%llu)\n", a, b, c);
    return 1;
  };
  run2(lambda, 1, 2, 3);

  // lambda as last parameter
  run3(4, 4, 4, lambda);

  // immediate lambda as last parameter
  run3(5, 5, 5, [&](u64 a, u64 b, u64 c) {
    return printf("\nrun2(lambda,%llu,%llu,%llu)\n", a, b, c);
  });

  // run(spiral, 1, 2, [&](u64 res) { result = res; });
  //   // run(n, b, b + blockSize, [&](u64 res) {
  // run(spiral2, 1, 2, 3);
  pool.stop();
}
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
int spiral(int n, int min, int max) {
  printf("\nspiral(n:%d, min:%d, max:%d)\n", n, min, max);
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

// template <typename Func, size_t... Ts2>
template <typename TupT, size_t... Is>
int combine4(TupT&& tup, index_sequence<Is...>) {
  return get<sizeof...(Is)>(tup)(get<Is>(forward<TupT>(tup))...);
}

template <typename Func, typename... Args>
int run4(Func&& f, Args&&... args) {
  // Job job = bind(forward<Func>(f), forward<Args>(args)...);
  // return Q.push(job);

  // args.pop_back();
  // f(&args...);
  // Job job = bind(forward<Func>(f), forward<Args>(args)...);
  // job();
  auto iseq = make_index_sequence<sizeof...(Args) - 1> {};
  int cb = combine4(forward_as_tuple(forward<Args>(args)...), iseq);

  // return Q.push(job);
  return 0;
}

// Some different ways to delegate a function

int main() {
  printf("Press ctrl+c to stop.\n");

  int result = 0;
  run(spiral, 1, 2, 3);
  run2(spiral, 1, 2, 3);

  //  calling a lambda as first param
  auto lambda = [&](int a, int b, int c) {
    ostringstream ss;
    printf("\nrun2(lambda,%d,%d,%d)\n", a, b, c);
    return 1;
  };
  run2(lambda, 1, 2, 3);

  // lambda as last parameter
  run3(3, 3, 3, lambda);

  // immediate lambda as last parameter
  run3(4, 4, 4, [&](int a, int b, int c) {
    return printf("\nrun2(lambda,%d,%d,%d)\n", a, b, c);
  });

  // run4(spiral, 1, 2, 3);
  run4(spiral, 5, 5, 5, lambda);

  // run4(spiral, 7, 7, 7, [&](int res) { result = res; });
  // run(n, b, b + blockSize, [&](int res) {
  pool.stop();
}
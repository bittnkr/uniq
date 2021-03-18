#pragma once
#include "uniq.h"
namespace uniq {

typedef function<void()> voidfunction;
typedef function<string()> stringfunction;

// stores a function call to make a posterior call
template <class Func, class... Args>
auto call(Func&& func, Args&&... args) {
  // return bind(forward<Func>(f), forward<Args>(args)...);
  auto f = forward<Func>(func);
  auto r = bind(f, forward<Args>(args)...);
  return r;
}

void test_call() {
  auto echo = [](string s) { return s; };
  auto f = call(echo, "Hello");
  CHECK(f() == "Hello");
};

}  // namespace uniq

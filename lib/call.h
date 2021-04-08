#pragma once
namespace uniq {
#include "std.h"

typedef void(*voidfunc)();
typedef function<void()> voidfunction;
typedef function<any()> anyfunction;
typedef function<string()> stringfunction;

#define ASSERT(expr) assert(expr);
#define ASSERT_INVOCABLE(F, A) static_assert(__is_invocable<typename decay<F>::type, typename decay<A>::type...>::value, "Actor() arguments must be invocable after conversion to rvalues");
#define FWD(...) forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

// stores a function call to make a posterior call
template <class Func, class... Args>
auto call(Func&& func, Args&&... args) {
  // return bind(forward<Func>(f), forward<Args>(args)...);
  auto f = forward<Func>(func);
  auto r = bind(f, forward<Args>(args)...);
  return r;
}
}  // namespace uniq
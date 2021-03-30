#include "uniq.h"

// say("Hello", 123)
template <typename... Args>
  string say(Args&&... args) {
    ostringstream ss;
    (ss << ... << args);
    cout << ss.str() << flush;
    return ss.str();
  }

// template <typename Func, typename... Args>
// using VariadicFunc = Func (*)(Args...);

// run(say, "Hello", 123)
template <typename Func, typename... Args>
  auto run(Func&& f, Args&&... a) {
    // return f(args...); // this works

    // save and apply
    auto tuple = forward_as_tuple(a...); // or tie(a...);
    log(anyType(tuple));
    return apply(f, tuple);
  }

// template <typename... Args>
// using Event = std::function<void(Args...)>;

// Lazy(say, "Hello", 123)()
template <typename Func, typename... Args>
class Lazy {
  Lazy(Func&& f, Args&&... a) {
    auto tup = forward_as_tuple(a...); // or tie(a...);
    log(anyType(tup));
    args = tup;
    func = f;
  }

  Func&& func;
  tuple<Args&&...> args;
  
  auto operator()(Args&&... moreargs) {
    return func(args..., moreargs...); 
    // return apply(func, args, moreargs...); 
    // return apply([&](Args&&... a, Args&&... b) { func(a..., b...); }, args, moreargs...);
  }
};

template<class T> // []<T>(T arg):T { return arg }
T echo(T arg) {return arg;}

string test_echo(string s) { return s; }

int main() {
  auto s = say("say ", "hello\n"); // cout << s;

  cout <<  run(test_echo, "run test_echo\n");

  // All lines bellow dont compile with error of 
  // no instance of ... matches the argument list 

  // Lazy(test_echo)
  // auto f = Lazy(test_echo, "Lazy"); cout << f(" test_echo\n");

  // BANG! run(function with variadic params)
  // cout << run(say, "run", "say\n"); 

  // The dream: Lazy call of a variadic tempĺate
  // auto hello = Lazy(say, "Lazy "); 
  // hello("hello: ", 123, "\n"); // should printout Lazy hello: 123
}

// https://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments
// https://stackoverflow.com/questions/19620818/stdtuple-as-class-member
// https://en.cppreference.com/w/cpp/experimental/apply
// https://www.fluentcpp.com/2020/10/16/tie-make_tuple-forward_as_tuple-how-to-build-a-tuple-in-cpp/
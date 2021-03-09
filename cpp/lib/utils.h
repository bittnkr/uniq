#pragma once
#include "std.h"
namespace uniq {

string format(const string s, ...) {
  va_list ap;
  char* fp = NULL;
  va_start(ap, s);
  vasprintf(&fp, s.c_str(), ap);
  va_end(ap);
  unique_ptr<char[]> formatted(fp);
  return string(formatted.get());
}

inline string exception_message() // https://stackoverflow.com/a/3641809/9464885
{
  try { throw; }// rethrow_exception(eptr); }
  catch (const exception &e) { return e.what()   ; }
  catch (const string    &e) { return e          ; }
  catch (const char      *e) { return e          ; }
  catch (const int        i) { return to_string(i); }
  catch (const long       l) { return to_string(l); }
  catch (...)                { return "unknown exception"; }
}

// float random 0..1
float rnd(){ return rand() / (RAND_MAX + 1.); };

// int random(max)
size_t rnd(size_t max = 0){ return max ? rand() % max : rand(); }

// sign of a number -1 +1
int sign(long v) { return v >= 0 ? 1 : -1; }
int sign(double v) { return v >= 0 ? 1 : -1; }

template <typename... Args> 
string sstr(Args &&... args )
{
    ostringstream ss;
    ( (ss << std::dec) << ... << args );
    return ss.str();
}

template <typename... Args> 
void logn(Args &&... args )
{
  cout << sstr(args...) << std::flush;
}

template <typename... Args> 
void log(Args &&... args )
{
  logn(args...,"\n");
}

// any ===================================================================

// #include <cxxabi.h>
// string demangle(const char *mangled)
// { // returns the type description from typeid(v).name()
//   int status;
//   unique_ptr<char[], void (*)(void *)> result(
//       abi::__cxa_demangle(mangled, 0, 0, &status), free);
//   return result.get() ? string(result.get()) : "error occurred";
// }

// template<typename T> inline T anyto(const any& v)
// {
//   if(v.type() == typeid(T))
//     return any_cast<T>(v);
// }

// string anyType(const any v, bool demangled = 0)
// {
//    return v.type().name();
//   // string t = v.type().name();
//   // string t2 = sstr("(", t, ") => ", demangle(t.c_str()), ") ");
//   // return demangled ? t2 : t;
// }

// string anyString(const any v)
// {
//   string r = "unknown", t = anyType(v);
//   if (t == typeid(const char *).name())
//     r = string(anyto<const char *>(v)); // "PKc"
//   else if (t == typeid(int).name())
//     r = to_string(int(anyto<int>(v)));
//   else if (t == typeid(bool).name())
//     r = to_string(anyto<bool>(v));
//   else if (t == typeid(string).name())
//     r = anyto<string>(v);
//   return r;
// }

inline string trim(const string &s)
{
   auto wsfront=find_if_not(s.begin(),s.end(),[](int c){return isspace(c);});
   auto wsback=find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base();
   return (wsback<=wsfront ? string() : string(wsfront,wsback));
}

string tolower(const string s){
  string r = s;
  for(auto &c : r){ c = std::tolower(c); }
  return r;
}

string join(const vector<string>& v, string delimiter=" ") {
  if (v.size()==0) return "";
  string r = v[0];
  for (auto i = 1; i < v.size(); i++)
    r += delimiter + v[i];
  return r;
}

string repeat(string s, int n) {
  ostringstream os;
  for(int i = 0; i < n; i++) os << s;
  return os.str();
}

string replace( string const & in, string const & from, string const & to )
{
  return regex_replace( in, regex(from), to);
}

// Tests =======================================================================
#include "test.h"
void test_utils() {
  CHECK(sstr("a",1) == "a1");
  CHECK(trim(" a b\t\n") == "a b" );
  CHECK(tolower("ABC") == "abc" );
  CHECK(format("%d-%s",1,"a") == "1-a" );
  CHECK(join({"a","b","c"},"-") == "a-b-c" );
  CHECK(repeat("12",3) == "121212");
  CHECK(replace("a:=1",":=","=") == "a=1");
};
}// uniq • Released under GPL 3.0
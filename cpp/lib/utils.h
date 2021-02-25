#pragma once
namespace uniq {
#include "uniq.h"

// inline string format(const string fmt_str, ...) {  // https://stackoverflow.com/a/8098080/9464885
//   int final_n, n = ((int)fmt_str.size()) * 2;      /* Reserve two times as much as the length of the fmt_str */
//   unique_ptr<char[]> formatted;
//   va_list ap;
//   while (1) {
//     formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
//     strcpy(&formatted[0], fmt_str.c_str());
//     va_start(ap, fmt_str);
//     final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
//     va_end(ap);
//     if (final_n < 0 || final_n >= n)
//       n += abs(final_n - n + 1);
//     else
//       break;
//   }
//   return string(formatted.get());
// }

string format(const string fmt_str, ...) {
  va_list ap;
  char* fp = NULL;
  va_start(ap, fmt_str);
  vasprintf(&fp, fmt_str.c_str(), ap);
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

float rnd(){ // float random()
  return rand() / (RAND_MAX + 1.);
};

int rnd(int max = 0){ // int(max) random()
  return max ? rand() % max : rand();
}

int sign(long v) { return v >= 0 ? 1 : -1; }
int sign(double v) { return v >= 0 ? 1 : -1; }

////SSTR & sstr ===================================================================
#define SSTR( x ) static_cast< ostringstream & >( ( ostringstream() << dec << x ) ).str()

template < typename... Args >
string sstr( Args &&... args )
{
    ostringstream ss;  
    ( (ss << std::dec) << ... << args ); // fold expression
    return ss.str();
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
}// uniq

// Tests =======================================================================
namespace test{
  using namespace uniq;
  void test_utils() {
    CHECK(trim(" a\t\n") == "a" );
    CHECK(tolower("ABC") == "abc" );
    CHECK(format("%d",1) == "1" );
  };
} // Part of the UniQ libray • Released under GNU 3.0

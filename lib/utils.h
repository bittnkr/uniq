#pragma once
#include "std.h"
#include "numtypes.h"
namespace uniq {

string exception_message() // https://stackoverflow.com/a/3641809/9464885
{
  try { throw; }// rethrow_exception(eptr); }
  catch (const exception &e) { return e.what()   ; }
  catch (const string    &e) { return e          ; }
  catch (const char      *e) { return e          ; }
  catch (const int        i) { return to_string(i); }
  catch (const long       l) { return to_string(l); }
  catch (...)                { return "unknown exception"; }
}

void handle_exception(){
  cerr << exception_message() << "\n";  
}

template <typename... Args> 
string sstr(Args &&... args )
{
    ostringstream ss;
    ( (ss << std::dec) << ... << args );
    return ss.str();
}

template <typename... Args> 
void check(bool expr, Args &&... args ){
  if(!expr) throw invalid_argument(sstr(args...));
}

string format(const string s, ...) {
  va_list ap;
  char* fp = NULL;
  va_start(ap, s);
  int r = vasprintf(&fp, s.c_str(), ap);
  va_end(ap);
  unique_ptr<char[]> formatted(fp);
  return string(formatted.get());
}

// float random 0..1
float rnd(){ return rand() / (RAND_MAX + 1.); };

// int random(max)
size_t rnd(size_t max = 0){ return max ? rand() % max : rand(); }

// sign of a number -1 +1
int sign(long v) { return v >= 0 ? 1 : -1; }
int sign(double v) { return v >= 0 ? 1 : -1; }

double round(double v, int places) { 
  u64 p = pow(10, places);
  v = ::round(v * p); 
  return v/p; 
} 

inline string trim(const string &s)
{
   auto wsfront=find_if_not(s.begin(),s.end(),[](int c){return isspace(c);});
   auto wsback=find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base();
   return (wsback<=wsfront ? string() : string(wsfront,wsback));
}

string tolower(const string s){
  string r = s;
  for(auto &c : r){ c = ::tolower(c); }
  return r;
}

string join(const vector<string>& v, string delimiter=" ") {
  if (v.size()==0) return "";
  string r = v[0];
  for (auto i = 1; i < v.size(); i++)
    r += delimiter + v[i];
  return r;
}

vector<string> split(string text, char delim) {
    string line;
    vector<string> vec;
    stringstream ss(text);
    while(getline(ss, line, delim)) {
        vec.push_back(line);
    }
    return vec;
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

u64 hash(string s)
{
   u64 h = 37;
   for(auto c : s){ h = (h * 54059) ^ (c * 76963); }
   return h;
}

inline i64 pause(i64 count = 1) {  while(--count > 0) __asm__( "pause;" ); return 0;}

// any ===================================================================
#include <cxxabi.h>
string demangle(const char *mangled)
{ // returns the type description from typeid(v).name()
  int status;
  unique_ptr<char[], void (*)(void *)> result(abi::__cxa_demangle(mangled, 0, 0, &status), free);
  return string(result.get());
}

string anyType(const any v)
{
  string t = v.type().name();
  t = demangle(t.c_str());
  t = replace(t,"","");
  t = replace(t,"__cxx11::","");
  t = replace(t,"basic_string<char, char_traits<char>, allocator<char> >","string");
  t = replace(t,"char const .*? ","char");
  // t = replace(t,"\\w+",  ORA+"$0"+GRY);  
  // t = GRY+t;
  return t;
}

// template<typename T> inline T anyto(const any& v)
// {
//   if(v.type() == typeid(T))
//     return any_cast<T>(v);
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

}// uniq • Released under GPL 3.0
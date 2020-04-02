// http://web.theurbanpenguin.com/adding-color-to-your-output-from-c/
// [0;31m	Red
// [0;32m	Green
// [0;33m	Yellow
// [0;34m	Blue
// [0;35m	Magenta
// [0;36m	Cyan
// [0m	Reset

#pragma once
#include <stdarg.h>
#include <stdio.h>

#include <memory>
#include <string>
using namespace std;

// template <typename... Args>
// std::string format(const char* fmt, Args... args) {
//   size_t size = snprintf(nullptr, 0, fmt, args...);
//   std::string buf;
//   buf.reserve(size + 1);
//   buf.resize(size);
//   snprintf(&buf[0], size + 1, fmt, args...);
//   return buf;
// }

void red(const std::string s, int bold = 0) {
  printf("\033[%d;31m%s\033[0m", bold, &s[0]);
}
void green(int bold = 0) { printf("\033[%d;32m", bold); }
void yellow(int bold = 0) { printf("\033[%d;33m", bold); }
void blue(int bold = 0) { printf("\033[%d;34m", bold); }
void magenta(int bold = 0) { printf("\033[%d;35m", bold); }
void cyan(int bold = 0) { printf("\033[%d;36m", bold); }
void reset() { printf("\033[0m"); }

std::string format(const std::string fmt_str, ...) {
  va_list ap;
  char* fp = NULL;
  va_start(ap, fmt_str);
  vasprintf(&fp, fmt_str.c_str(), ap);
  va_end(ap);
  std::unique_ptr<char[]> formatted(fp);
  return std::string(formatted.get());
}

std::string colorcode(int code) { return format("\033[0%dm", 30 + code); }

std::string red3(const std::string s, ...) {
  return string("\033[0;31m") + s + string("\033[0m");
};

// template <typename... Args>
// string red2(const char* fmt, Args... args) {
//   return string("\033[0;31m") + format(fmt,args...) + string("\033[0m");
// };

// string red2(string s, int bold = 0) { return format("\033[0;31m%s\033[0m");
// };
string red2(string s) { return format("\033[0;31m%s\033[0m", s); };

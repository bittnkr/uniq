#pragma once
#include "common.h"

void red(int bold = 0) { printf("\033[%d;31m", bold); }
void green(int bold = 0) { printf("\033[%d;32m", bold); }
void yellow(int bold = 0) { printf("\033[%d;33m", bold); }
void blue(int bold = 0) { printf("\033[%d;34m", bold); }
void magenta(int bold = 0) { printf("\033[%d;35m", bold); }
void cyan(int bold = 0) { printf("\033[%d;36m", bold); }
void reset() { printf("\033[0m"); }

// string colorcode(int code) { return format("\033[0%dm", 30 + code); }
// void red(const string s, int bold = 0) {return format("\033[%d;31m%s\033[0m", bold, &s[0]);}

string red3(const string s, ...) {
  return string("\033[0;31m") + s + string("\033[0m");
};

// template <typename... Args>
// string red2(const char* fmt, Args... args) {
//   return string("\033[0;31m") + format(fmt,args...) + string("\033[0m");
// };

// string red2(string s, int bold = 0) { return format("\033[0;31m%s\033[0m");
// };
// string red2(string s) { return format("\033[0;31m%s\033[0m", s); };

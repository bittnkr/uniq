#include "std.h"
#include "utils.h"

//print a colored progress bar using ANSI escape codes 
string progressbar(int width, float progress){
  int w = width*progress;
  string s = ORA(to_string(int(progress * 100))+"% "+GRY("[") + GRN(string(w,'=')+(progress<1?+">":"")) +GRY(string(width-w,'.')+"]\r"));
  return s;
}

int main(){
  int width = 70;
  float progress = 0.0;
  while (progress <= 1.01) {
      auto s = progressbar(width, progress);
      cout << s; cout.flush();
      progress += 0.01;
      this_thread::sleep_for(chrono::milliseconds(50));
  }
  cout << endl;
  return 0;
}
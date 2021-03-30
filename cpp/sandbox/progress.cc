#include "uniq.h"

//print a colored progress bar using ANSI escape codes 
string progressbar(int width, float progress){
  int w = width*progress;
  string s = sstr(GRY, "[", GRN, string(w, '='), (progress < 1 ? +">" : ""), GRY, string(width - w, '.'), "] ",ORA, to_string(int(progress * 100)), "% ", "\r");
  return s;
}

int main(){
  int width = 70;
  float progress = 0.0;
  while (progress <= 1.01) {
      out(progressbar(width, progress));
      progress += 0.1;
      usleep(100000);
      // this_thread::sleep_for(chrono::milliseconds(100));
  }
  log("");
  return 0;
}
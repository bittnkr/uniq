#include "uniq.h"

//print a colored progress bar using ANSI escape codes
string progressbar(int width, float progress) {
  int w = width * progress;
  string s = sstr(GRY, "[", GRN, string(w, '='), (progress < 1 ? +">" : ""), GRY, string(width - w, '.'), "] ",ORA, to_string(int(progress * 100)), "% ", "\r");
  return s;
}

int main() {
  log("Processor speed");
  int width = 100;//;
  while (true) {
    float progress = stod(readFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"))
                   / stod(readFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"));
    log(progressbar(width, progress));
    sleep(1);
  }
  return 0;
}
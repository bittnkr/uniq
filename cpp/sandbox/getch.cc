#include "uniq.h"

int main() {
  log("Press any key to repeat or q to quit...");   
  while (1) {
    char c = getch();
    if (c == 'q') exit(0);
    log(c, " was pressed.");
  }
}
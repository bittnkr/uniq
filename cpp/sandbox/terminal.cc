// #pragma once
#include "std.h"
#include "terminal.h"
int main(int argc, const char *argv[]) { 
  cout << BLD(" bold ") 
  << ITA(GRY("gray "))  
  << UND(GRN(" green ")) 
  << RED(" red ") 
  << DIM(BLD(BLINK(ORA(" blink"))))
  << endl;
};
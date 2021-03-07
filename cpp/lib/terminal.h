#pragma once
#include "utils.h"
namespace uniq {

/* COLORS */
#define RST  "\033[0m"
// #define CODE(n) "\033["+to_string(n)+"m" + s + RST

string BLD(string s) { return "\033[1m"+ s + RST; }
string DIM(string s) { return "\033[2m"+ s + RST; }
string ITA(string s) { return "\033[3m"+ s + RST; }
string UND(string s) { return "\033[4m"+ s + RST; }
string BLINK(string s) { return "\033[5m"+ s + RST; }

string BLK(string s) { return "\033[30m"+ s + RST; }
string RED(string s) { return "\033[31m"+ s + RST; }
string GRN(string s) { return "\033[32m"+ s + RST; }
string YEL(string s) { return "\033[33m"+ s + RST; }
string BLU(string s) { return "\033[34m"+ s + RST; }
string MAG(string s) { return "\033[35m"+ s + RST; }
string CYN(string s) { return "\033[36m"+ s + RST; }
string WHT(string s) { return "\033[37m"+ s + RST; }
string GRY(string s) { return "\033[90m"+ s + RST; }
string ORA(string s) { return "\033[38;5;202m"+ s + RST; }

string BGBLK(string s) { return "\033[40m"+ s + RST; }
string BGRED(string s) { return "\033[41m"+ s + RST; }
string BGGRN(string s) { return "\033[42m"+ s + RST; }
string BGYEL(string s) { return "\033[43m"+ s + RST; }
string BGBLU(string s) { return "\033[44m"+ s + RST; }
string BGMAG(string s) { return "\033[45m"+ s + RST; }
string BGCYN(string s) { return "\033[46m"+ s + RST; }
string BGWHT(string s) { return "\033[47m"+ s + RST; }

string BGGRY(string s) { return "\033[90m"+ s + RST; }
string BGORA(string s) { return "\033[38;5;202m"+ s + RST; }

string CLR(){ return "\033[2J"; } //Clear the screen.
// Move the cursor to row r, column c. indexed starting at 1.
string MOV(int r=1,int c=1){ return "\033["+to_string(r)+";"+to_string(c)+"H"; } 
string HIDE(){ return "\033[?25l"; } // Hide the cursor.
string DEL(){ return "\033[K"; } // Delete everything from the cursor to the end of the line.
string HOME(){ return "\033[H"; } // Move the cursor to the upper-left corner of the screen.

// black red red+ green green+ orange yellow blue blue+ magenta magenta+ cyan cyan+ gray white
vector<int> RAINBOW{20,21,1,22,2,23,3,24,4,25,5,26,6,27,7};
string colorcode(int i, vector<int> pallete = RAINBOW) {
  i = pallete[i % (pallete.size()-1)]; 
  return sstr("\033[", (i<10? "01":"22"), ";3", to_string(i%10),"m");
}

// Tests =======================================================================
#include "test.h"
void test_terminal() {
  // for (auto i = 0; i < 20; i++)
  //   log(colorcode(i),i," ",replace(colorcode(i),"\033",""),RST);
  CHECK(colorcode(1)=="\033[22;31m");
  CHECK(colorcode(13)=="\033[22;37m");
}
}// uniq • Released under GPL 3.0

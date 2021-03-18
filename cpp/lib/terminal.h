#pragma once
#include "uniq.h"
namespace uniq {

/* COLORS */
const string RST("\033[0m");

const string BLD("\033[1m");
const string DIM("\033[2m");
const string ITA("\033[3m");
const string UND("\033[4m");
const string BLI("\033[5m");

const string BLK("\033[30m");
const string RED("\033[31m");
const string GRN("\033[32m");
const string YEL("\033[33m");
const string BLU("\033[34m");
const string MAG("\033[35m");
const string CYN("\033[36m");
const string WHT("\033[37m");
const string GRY("\033[90m");
const string ORA("\033[38;5;202m");

const string BGBLK("\033[40m");
const string BGRED("\033[41m");
const string BGGRN("\033[42m");
const string BGYEL("\033[43m");
const string BGBLU("\033[44m");
const string BGMAG("\033[45m");
const string BGCYN("\033[46m");
const string BGWHT("\033[47m");

const string BGGRY("\033[90m");
const string BGORA("\033[38;5;202m");

const string CLR("\033[2J"); //Clear the screen.
const string HIDE("\033[?2"); // Hide the cursor.
const string DEL("\033[K"); // Delete everything from the cursor to the end of the line.
const string HOME("\033[H"); // Move the cursor to the upper-left corner of the screen.

// Move the cursor to row r, column c. indexed starting at 1.
string MOV(int r=1,int c=1){ return "\033["+to_string(r)+";"+to_string(c)+"H"; } 

// black red red+ green green+ orange yellow blue blue+ magenta magenta+ cyan cyan+ gray white
vector<int> RAINBOW{20,21,1,22,2,23,3,24,4,25,5,26,6,27,7};
string colorcode(int i, vector<int> pallete = RAINBOW) {
  i = pallete[i % (pallete.size()-1)]; 
  return string("\033[")+(i<10? "01":"22")+";3"+to_string(i%10)+"m";
}

// Tests =======================================================================
void test_terminal() {
  CHECK(RED+"X"+RST == "\033[31mX\033[0m");
 
  CHECK(colorcode(1)=="\033[22;31m");
  CHECK(colorcode(13)=="\033[22;37m");
  // for (auto i = 0; i < 20; i++)
  //   log(colorcode(i),i," ",replace(colorcode(i),"\033",""),RST);
}
}// uniq • Released under GPL 3.0

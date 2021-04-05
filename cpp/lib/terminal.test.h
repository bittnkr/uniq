// Tests =======================================================================
#include "test.h"
#include "terminal.h"
using namespace uniq;
TEST(terminal) {
  CHECK(RED+"X"+RST == "\033[31mX\033[0m");
 
  CHECK(colorcode(1)=="\033[22;31m");
  CHECK(colorcode(13)=="\033[22;37m");
  // for (auto i = 0; i < 20; i++)
  //   log(colorcode(i),i," ",replace(colorcode(i),"\033",""),RST);
}

#pragma once
#include "numtypes.h"
namespace uniq {

union Color {
  u32 value;
  u8 array[4];
  struct P {
    u8 r, g, b, a;
  } parts;

  Color() { value = random(); }
  Color(u32 v) { value = v; }
  Color(u8 r, u8 g, u8 b, u8 a = 255) {
    value = b + (g<<8) + (r<<16) + (a<<24);
    // parts.r = r;
    // parts.g = g;
    // parts.b = b;
    // parts.a = a;
  };
};

Color rgba(u8 r, u8 g, u8 b, u8 a=255) { return Color(r, g, b, a); }
Color rgba(u32 v) { return Color(v); }
Color rgba() { return Color(); }

Color rgb(u8 r, u8 g, u8 b) { return Color(r, g, b); };
Color rgb(u32 v) { Color c = Color(v); c.parts.a = 255; return c; }
Color rgb() { Color c = Color(); c.parts.a = 255; return c; };

void test_Color() { // ================================================= tests 
  Color c = rgb();
  CHECK(c.value > 0);
  CHECK(c.parts.r == c.array[0]);
  CHECK(c.parts.g == c.array[1]);
  CHECK(c.parts.b == c.array[2]);
  CHECK(c.parts.a == c.array[3]);
  CHECK(c.parts.a == 255);
  CHECK(sizeof(Color) == sizeof(u32));
};

}// uniq • Released under GPL 3.0

// https://www.ncbi.nlm.nih.gov/IEB/ToolBox/CPP_DOC/lxr/source/include/gui/utils/rgba_color.hpp

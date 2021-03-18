#pragma once
#include "uniq.h"

// read a file to a string
// https://stackoverflow.com/a/28344440/9464885
string readFile(char const *filename) {
  ifstream ifs(filename);
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

// another way to read a file to a string
string readFile2(char const *filename) {
  ifstream ifs(filename);
  stringstream s;
  s << ifs.rdbuf();
  return s.str();
}

// #define DOCTEST_CONFIG_DISABLE
void test_fs() {
  string thisFile = readFile("fs.h");
  string thatFile = readFile2("fs.h");
  CHECK(thisFile == thatFile);
}
#pragma once
#include "std.h"

// read a file to a string
// https://stackoverflow.com/a/28344440/9464885
string readFile(const string filename) {
  ifstream ifs(filename);
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

// another way to read a file to a string
string readFile2(const string filename) {
  ifstream ifs(filename);
  stringstream s;
  s << ifs.rdbuf();
  return s.str();
}

// inline bool fileExists(const string& filename){ filesystem::exists("helloworld.txt"); }
// void writeFile(const string filename, const string data, ios_base::openmode mode=ios_base::app) { 
//   ofstream fs;
//   fs.open(filename, mode); // append instead of overwrite
//   fs << data; 
// }
// void appendFile(const string filename, const string data) { writeFile(filename, data, ios_base::app); }

TEST(fs) {
  string thisFile = readFile("fs.h");
  string thatFile = readFile2("fs.h");
  CHECK(thisFile == thatFile);
}
#pragma once
// namespace uniq{
// #include <x86intrin.h>
#include "std.h" // all std headers
#include "unistd.h" 
// };
#include "test.h" // unit testing

#include "numtypes.h" // basic numeric types
#include "terminal.h" // terminal printing with colors
#include "utils.h" // sstr, format, join, repeat, trim ...
#include "call.h" // lazy call & lambda utilities
#include "fs.h" // filesystem utilities readFile, saveFile() ...
#include "sha256.h" // sha256 cryptographic function

#include "Time.h" // time primitive
#include "Color.h" // color primitive
#include "Atomic.h" // basic atomic types
#include "Named.h" // class with name
#include "Actor.h" // common parent for active classes
#include "Benchmark.h" // execution time analisys
#include "queue.h" // the queue
#include "pool.h" // thread pool

using namespace uniq;
//uniq • Released under GPL 3.0
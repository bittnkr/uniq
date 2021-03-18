#pragma once
#include <x86intrin.h>
#include "std.h" // all std headers
#include "test.h" // unit testing

#include "numtypes.h" // basic numeric types
#include "terminal.h" // terminal printing with colors
#include "utils.h" // sstr, format, join, repeat, trim ...
#include "call.h" // A lazy call & lambda utilities
#include "fs.h" // Filesystem utilities readFile, saveFile() ...
#include "sha256.h" // Filesystem utilities readFile, saveFile() ...

#include "Time.h" // Simpler Time primitive
#include "Color.h" // basic atomic types
#include "atom.h" // basic atomic types
#include "Named.h" // A class with name
#include "Actor.h" // a common parent for active classes
#include "queue.h" // the queue
#include "pool.h" // thread pool
// #include "Timer.h" // Time counting & benchmarks
// #include "Worker.h" // worker thread
// #include "Atom.h" // basic atomic types
// #include "Digit.h"
// #include "Number.h"
// #include "Json.h"

using namespace uniq;
//uniq • Released under GPL 3.0
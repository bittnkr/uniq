#pragma once
// namespace uniq{
// #include <x86intrin.h>
#include "std.h" // all std headers
#include "unistd.h" // posix functions like usleep
// };

#include "test.h" // unit testing

#include "numtypes.h" // basic numeric types
#include "terminal.h" // terminal printing with colors
#include "utils.h" // sstr, format, join, repeat, trim ...
#include "call.h" // lazy call & lambda utilities
#include "fs.h" // filesystem utilities readFile, saveFile() ...
#include "sha256.h" // sha256 cryptographic function

// #include "hello.h" // A Boilerplate for classes
#include "Time.h" // time primitive
#include "Color.h" // color primitive
#include "Atomic.h" // basic atomic types
#include "Named.h" // class with name
#include "Id.h" // class with growing id
#include "Node.h" // parent/children node using shared_ptr
#include "Actor.h" // common parent for active classes
#include "queue.h" // the queue
#include "pool.h" // thread pool
#include "ElasticQueue.h" // a locking queue with elastic storage
// #include "Profiler.h" // execution time recording
// #include "Benchmark.h" // speed tests

using namespace uniq;
//uniq • Released under GPL 3.0
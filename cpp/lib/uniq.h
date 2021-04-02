#pragma once
#include "std.h" // all std headers
#include "unistd.h" // posix functions like usleep

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
#include "Id.h" // incremental id
#include "Node.h" // parent/children node using shared_ptr
#include "Actor.h" // common parent for active classes
#include "queue.h" // the queue
#include "pool.h" // thread pool
#include "ElasticQueue.h" // a locking queue with elastic storage
#include "Profiler.h" // execution time recording
#include "Benchmark.h" // speed tests
#include "Log.h" // simple logger
// #include "Json.h" // Json primitive
// #include "Worker.h" // worker thread
// #include "WorkerPool.h" // A Worker with helpers
// #include "Digit.h" // big digit 
// #include "Number.h" // big number
// #include "Timer.h" // Hi-precision timer
// #include "Any.h" // simpler std:any 
// #include "Event.h" // event emitter

using namespace uniq;
//uniq • Released under GPL 3.0
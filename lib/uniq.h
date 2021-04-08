#pragma once
#include "std.h" // all std headers
#include "unistd.h" // posix functions like usleep
#include "numtypes.h" // basic numeric types
#include "call.h" // lazy call & lambda utilities
#include "utils.h" // rnd, format, join, repeat, trim ...
#include "terminal.h" // terminal printing with colors

#include "Named.h" // class with name
#include "Time.h" // time primitive
#include "Atomic.h" // basic atomic types
#include "Actor.h" // common parent for active classes
#include "queue.h" // the queue
#include "OpenQueue.h" // a locking queue with elastic storage
#include "Log.h" // simple logger
#include "test.h" // unit testing
#include "test.cc" // dependable tests

#include "Id.h" // incremental id
#include "Profiler.h" // execution time recording
#include "Benchmark.h" // speed tests
#include "Node.h" // parent/children node using shared_ptr
#include "State.h" // state machine 

#include "Worker.h" // worker thread
#include "pool.h" // thread pool
// #include "model.h" // UniQ classes mockup
// #include "Lazy.h" // lazy call
// #include "WorkerPool.h" // A Worker with helpers
// #include "Json.h" // Json primitive
// #include "Digit.h" // big digit 
// #include "Number.h" // big number
// #include "Timer.h" // hi-precision timer
// #include "Any.h" // simpler std:any 
// #include "Event.h" // event emitter
#include "Color.h" // color primitive
#include "sha256.h" // cryptographic function
#include "fs.h" // filesystem utilities readFile, saveFile() ...

//uniq • Released under GPL 3.0
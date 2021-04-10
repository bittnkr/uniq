#include "std.h"
#include "unistd.h" // posix functions
#include "utils.h"
#include "terminal.h"
#include "numtypes.h"
#include "Name.h"
#include "OpenQueue.h"
#include "Time.h"

#define TESTS 2 // unit testing (0: MUTE, 1: SILENT, 2: NORMAL, 3 VERBOSE)
#include "test.h"

#define TRACE 1; // function call execution time tracing 
#include "Profiler.h"
using namespace uniq;

void a() {TT usleep(50'000); }
void b() {TT usleep(80'000); }
void f() {TT usleep(200'000); a(); }
void h() {TT usleep(200'000); a(); b(); }
void g() {TT usleep(200'000); b(); a(); }

int main(){ TT
    vector<thread> workers;

    workers.push_back(thread(f));
    workers.push_back(thread(h));
    workers.push_back(thread(g));

    for (auto& w : workers) w.join();

    // run(f); run(g); run(h);
    // join();
    profiler().save();
}
// adapted from https://gist.github.com/maronsson/073840bf94e4d6df94c5f294a6e96e03
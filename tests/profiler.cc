#include "uniq.h"
#include "unistd.h" // posix functions

#include "Profiler.h"
// #define timeit() auto _p = profiler().probe(__FUNCTION__);

void a() { timeit() usleep(50'000); }
void b() { timeit() usleep(80'000); }
void f() { timeit() usleep(200'000); a(); }
void h() { timeit() usleep(200'000); a(); b(); }
void g() { timeit() usleep(200'000); b(); a();};

int main()
{
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
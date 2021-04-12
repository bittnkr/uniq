//==============================================================================
// Primitive Time as double. Seconds with fractional sub-seconds
//==============================================================================
#pragma once
#include "utils.h"
namespace uniq {

struct Time {
  double time;  // double seconds with fractional nanos

  Time();
  Time(Time &t) { time = t.time; }
  Time operator = (Time &t) { return time = t.time; }

  Time(double t) { time = t; }
  Time operator = (double t) { return time = t; }
  inline operator double() const { return time; }

  const string fmt(string fmtString) { // as put_time
    time_t secs = time;
    struct tm* timeinfo = localtime(&secs);
    std::ostringstream os;
    os << put_time(timeinfo, fmtString.c_str());
    return os.str();
  }

  const string str() { 
    string s;
    if (time < MICRO) s = sstr(nanos(), "ns");
    else if (time < MILI) s = sstr(micros(), "µs");
    else if (time < 1) s = sstr(milis(), "ms");
    else if (time < 60) s = sstr(seconds(), "s");
    else if (time < 60*60) s = fmt("M:%S");
    else if (time < 60*60*24) s = fmt("%H:%M:%S");
    else s = fmt("%a %b %e %H:%M:%S %Y");
    return s;
  }

  const string ctime() {
    time_t t = time;
    return trim(::ctime(&t));
  }  // for reference

  inline bool operator == (Time t) { return fabs(time - t.time) < NANO; }
  inline bool operator != (Time t) { return fabs(time - t.time) >= NANO; }
  inline bool operator  < (Time t) { return time < t.time; }
  inline bool operator  > (Time t) { return time > t.time; }
  inline bool operator <= (Time t) { return time <= t.time; }
  inline bool operator >= (Time t) { return time >= t.time; }

  inline bool operator == (double t) { return fabs(time - t) < NANO; }
  inline bool operator != (double t) { return fabs(time - t) >= NANO; }
  inline bool operator <  (double t) { return time < t; }
  inline bool operator >  (double t) { return time > t; }
  inline bool operator <= (double t) { return time <= t; }
  inline bool operator >= (double t) { return time >= t; }

  void operator+=(Time t) { time += t.time; }
  void operator-=(Time t) { time -= t.time; }
  void operator*=(Time t) { time *= t.time; }
  void operator/=(Time t) { time /= t.time; }

  Time operator+(Time t) { return Time(time + t.time); }
  Time operator-(Time t) { return Time(time - t.time); }
  Time operator*(Time t) { return Time(time * t.time); }
  Time operator/(Time t) { return Time(time / t.time); }

  Time operator+(double t) { return Time(time + t); }
  Time operator-(double t) { return Time(time - t); }
  Time operator*(double t) { return Time(time * t); }
  Time operator/(double t) { return Time(time / t); }

  void operator--() { time = time - 1; }
  void operator++() { time = time + 1; }

  double nanos(int places = 0) { return round(time * GIGA, places); }
  double micros(int places = 0) { return round(time * MEGA, places); }
  double milis(int places = 0) { return round(time * KILO, places); }
  double seconds(int places = 0) { return round(time, places); }
  double minutes(int places = 0) { return round(seconds() / 60, places); }
  double hours(int places = 0) { return round(minutes() / 60, places); }
  double days(int places = 0) { return round(hours() / 24, places); }
  double weeks(int places = 0) { return round(days() / 7, places); }
  double months(int places = 0) { return round(days() / 30, places); }
  double years(int places = 0) { return round(months() / 12, places); }

  Time operator()() {
    Time v, r = v.time - time;
    time = v;
    return r;
  }

  Time operator()(double t) {
    if (t == 0) return double(Time()) - time;
    if (t < 0) return double(Time()) - time + t;
    Time r(t - time);
    time = t;
    return r;
  }
};

ostream& operator<<(ostream& os, Time& t) { return os << t.str(); }

// SystemTime from clock_gettime
Time SystemTime(int source=CLOCK_REALTIME){ 
    timespec t;
    clock_gettime(source, &t);
    return NANO * t.tv_nsec + t.tv_sec;
};

Time::Time() { time = SystemTime(); }

const Time START_TIME();

// ClockTime from clock();
Time ClockTime() { return MICRO * ::clock(); }

// // ChronoTime from std::chrono
// Time ChronoTime() { 
//   return NANO * chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
// };

// CpuTime from rdtscp;
#include <x86intrin.h>
inline u64 ticks() { u32 i; return __rdtscp(&i);} // 11ns
inline u64 ticks(u64 prev) { return ticks()-prev;}

const u64 START_TICKS = ticks();

const double CLOCK_CYCLE = NANO / 2.4; // todo: dynamic update

inline Time CpuTime() { return (ticks()-START_TICKS) * CLOCK_CYCLE; } // 13ns 
inline Time CpuTime(Time prev) { return CpuTime()-prev;}


}// UniQ • Released under GPL 3 licence
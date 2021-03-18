//==============================================================================
// Primitive Time as double. Seconds with fractional sub-seconds
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

typedef double (*TimeUpdate)(double);
inline double systemTime(double source);
TimeUpdate getTime = &systemTime;

struct Time {
  double value;  // double seconds with fractional nanos

  Time(double t) { value = t; }
  Time operator=(double t) { return value = t; }
  inline operator double() const { return value; }

  Time(int secs_, int nanos_) { value = NANO * nanos_ + secs_; }

  // copy constructor
  Time(Time &t) { value = t.value; }
  Time operator = (Time &t) { return value = t.value; }

  Time() { value = getTime(CLOCK_MONOTONIC); }

  const string fmt(string fmtString) { // as put_time
    time_t secs = value;
    struct tm* timeinfo = localtime(&secs);
    std::ostringstream os;
    os << put_time(timeinfo, fmtString.c_str());
    return os.str();
  }

  const string str() { 
    string s;
    if (value < MICRO) s = sstr(nanos(), " ns");
    else if (value < MILI) s = sstr(micros(), " µs");
    else if (value < 1) s = sstr(milis(), " ms");
    else if (value < 60) s = sstr(seconds(), " s");
    else s = fmt("%a %b %e %H:%M:%S %Y");
    return s;
  }

  const string ctime() {
    time_t t = value;
    return trim(::ctime(&t));
  }  // for reference

  inline bool operator==(Time t) { return fabs(value - t.value) < NANO; }
  inline bool operator!=(Time t) { return fabs(value - t.value) >= NANO; }
  inline bool operator<(Time t) { return value < t.value; }
  inline bool operator>(Time t) { return value > t.value; }
  inline bool operator<=(Time t) { return value <= t.value; }
  inline bool operator>=(Time t) { return value >= t.value; }

  void operator+=(Time t) { value += t.value; }
  void operator-=(Time t) { value -= t.value; }
  void operator*=(Time t) { value *= t.value; }
  void operator/=(Time t) { value /= t.value; }

  Time operator+(Time t) { return Time(value + t.value); }
  Time operator-(Time t) { return Time(value - t.value); }
  Time operator*(Time t) { return Time(value * t.value); }
  Time operator/(Time t) { return Time(value / t.value); }

  void operator--() { value = value - 1; }
  void operator++() { value = value + 1; }

  double nanos(int places = 1) { return round(value * GIGA, places); }
  double micros(int places = 1) { return round(value * MEGA, places); }
  double milis(int places = 1) { return round(value * KILO, places); }
  double seconds(int places = 1) { return round(value, places); }
  double minutes(int places = 1) { return round(seconds() / 60, places); }
  double hours(int places = 1) { return round(minutes() / 60, places); }
  double days(int places = 1) { return round(hours() / 24, places); }
  double weeks(int places = 1) { return round(days() / 7, places); }
  double months(int places = 1) { return round(days() / 30, places); }
  double years(int places = 1) { return round(months() / 12, places); }

  Time operator()() {
    Time v, r = v.value - value;
    value = v;
    return r;
  }

  Time operator()(double t) {
    if (t == 0) return double(Time()) - value;
    if (t < 0) return double(Time()) - value + t;
    Time r(t - value);
    value = t;
    return r;
  }
};

ostream& operator<<(ostream& os, Time& t) { return os << t.str(); }

// from clock_gettime
inline double systemTime(double source = CLOCK_MONOTONIC) {
  timespec ts;
  clock_gettime(int(source), &ts);
  double r = NANO * ts.tv_nsec + ts.tv_sec;
  return r;
}

// from ::clock();
inline double clockTime(double dummy = 0) {
  return double(::clock()) * MICRO;
}

// from std::chrono
inline double chronoTime(double dummy = 0) {
  return NANO * chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

// from rdtscp;
#include <x86intrin.h>
double CLOCK_CYCLE = NANO / 2.4;
inline u64 ticks() { u32 i; return __rdtscp(&i);}  // 11ns
inline double procTime() {return ticks() * CLOCK_CYCLE;} // 13ns (ticks()+2ns of double multiplication)


// tests =======================================================================
void test_Time() {
  CHECK(sizeof(Time) == sizeof(double));

  CHECK(u64(CLOCKS_PER_SEC) == MEGA);

  auto t = Time();
  CHECK(t.str() == t.ctime());
}

}// uniq • Released under GPL 3.0
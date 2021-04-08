//==============================================================================
// Primitive Time as double. Seconds with fractional sub-seconds
//==============================================================================
#pragma once
namespace uniq {

struct Time {
  double value;  // double seconds with fractional nanos

  Time();
  Time(Time &t) { value = t.value; }
  Time operator = (Time &t) { return value = t.value; }

  Time(double t) { value = t; }
  Time operator = (double t) { return value = t; }
  inline operator double() const { return value; }

  const string fmt(string fmtString) { // as put_time
    time_t secs = value;
    struct tm* timeinfo = localtime(&secs);
    std::ostringstream os;
    os << put_time(timeinfo, fmtString.c_str());
    return os.str();
  }

  const string str() { 
    string s;
    if (value < MICRO) s = sstr(nanos(), "ns");
    else if (value < MILI) s = sstr(micros(), "µs");
    else if (value < 1) s = sstr(milis(), "ms");
    else if (value < 60) s = sstr(seconds(), "s");
    else if (value < 60*60) s = fmt("M:%S");
    else if (value < 60*60*24) s = fmt("%H:%M:%S");
    else s = fmt("%a %b %e %H:%M:%S %Y");
    return s;
  }

  const string ctime() {
    time_t t = value;
    return trim(::ctime(&t));
  }  // for reference

  inline bool operator == (Time t) { return fabs(value - t.value) < NANO; }
  inline bool operator != (Time t) { return fabs(value - t.value) >= NANO; }
  inline bool operator  < (Time t) { return value < t.value; }
  inline bool operator  > (Time t) { return value > t.value; }
  inline bool operator <= (Time t) { return value <= t.value; }
  inline bool operator >= (Time t) { return value >= t.value; }

  inline bool operator == (double t) { return fabs(value - t) < NANO; }
  inline bool operator != (double t) { return fabs(value - t) >= NANO; }
  inline bool operator <  (double t) { return value < t; }
  inline bool operator >  (double t) { return value > t; }
  inline bool operator <= (double t) { return value <= t; }
  inline bool operator >= (double t) { return value >= t; }

  void operator+=(Time t) { value += t.value; }
  void operator-=(Time t) { value -= t.value; }
  void operator*=(Time t) { value *= t.value; }
  void operator/=(Time t) { value /= t.value; }

  Time operator+(Time t) { return Time(value + t.value); }
  Time operator-(Time t) { return Time(value - t.value); }
  Time operator*(Time t) { return Time(value * t.value); }
  Time operator/(Time t) { return Time(value / t.value); }

  Time operator+(double t) { return Time(value + t); }
  Time operator-(double t) { return Time(value - t); }
  Time operator*(double t) { return Time(value * t); }
  Time operator/(double t) { return Time(value / t); }

  void operator--() { value = value - 1; }
  void operator++() { value = value + 1; }

  double nanos(int places = 0) { return round(value * GIGA, places); }
  double micros(int places = 0) { return round(value * MEGA, places); }
  double milis(int places = 0) { return round(value * KILO, places); }
  double seconds(int places = 0) { return round(value, places); }
  double minutes(int places = 0) { return round(seconds() / 60, places); }
  double hours(int places = 0) { return round(minutes() / 60, places); }
  double days(int places = 0) { return round(hours() / 24, places); }
  double weeks(int places = 0) { return round(days() / 7, places); }
  double months(int places = 0) { return round(days() / 30, places); }
  double years(int places = 0) { return round(months() / 12, places); }

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

// SystemTime from clock_gettime
Time SystemTime(int source=CLOCK_REALTIME){ 
    timespec t;
    clock_gettime(source, &t);
    return NANO * t.tv_nsec + t.tv_sec;
};

Time::Time() { value = SystemTime(); }

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


}// uniq • Released under GPL 3.0
#include "std.h"
namespace uniq {

#define CST __ATOMIC_SEQ_CST

template<class T> inline T AtomicAdd(T& a, const T b) { return __atomic_fetch_add(&a, b, CST); }
template<class T> inline T AtomicSub(T& a, const T b) { return __atomic_fetch_sub(&a, b, CST); }
template<class T> inline T AtomicXCH(T& a, const T b) { return __atomic_exchange(&a, b); }

// template<class T> inline bool AtomicCAS(T& value, T& old, const T new_) { 
//   return __atomic_compare_exchange(&value, &old, &new_, false, CST, CST); }

template<class T>
struct Atomic {
  T value;
  Atomic() { value = T {}; }
  Atomic(const Atomic&);
  constexpr Atomic(T v){ value=v; }

  Atomic& operator = (const Atomic&);
  T operator = (T v) { __atomic_store(&value, &v, CST); return v; }

  inline T add(const T v) { return AtomicAdd(value, v); }
  inline T sub(const T v) { return AtomicSub(value, v); }
  inline T xch(const T v) { return AtomicXCH(value, v); }
  // inline bool CAS(T old, T v) { return AtomicCAS(&value, &old, &v, false, CST, CST); }
 
  inline bool swapLower(T v) {
    int old;
    do { 
      old = value; 
      if(old <= v) return 0;
    } while (!CAS(old, v));
    return 1;
  }

  inline bool swapGreater(T v) {
    int old;
    do { 
      old = value; 
      if(old >= v) return 0;
    } while (!CAS(old, v));
    return 1;
  }

  void operator += (T v) { add(v); }
  void operator -= (T v) { sub(v); }
  
  void operator  *= (T v) { T r; do { r = value*v; } while(!CAS(value, r)); value = r; }
  void operator  /= (T v) { T r; do { r = value/v; } while(!CAS(value, r)); value = r; }
  void operator  %= (T v) { T r; do { r = value%v; } while(!CAS(value, r)); value = r; }
  
  void operator &= (T v) { __atomic_fetch_and(&value, v, CST); }
  void operator |= (T v) { __atomic_fetch_or(&value, v, CST); }
  void operator ^= (T v) { __atomic_fetch_xor(&value, v, CST); }
  // T nand(T v) { return __atomic_fetch_nand(&value, v, CST); }
 
 	T operator++(int){ T v = add(1); return v;}; // v++
 	T operator--(int){ T v = sub(1); return v;}; // v--

  T& operator++(){ add(1); return value; }; // ++v
  T& operator--(){ sub(1); return value; }; // --v

  operator T() const { T v; __atomic_load(&value, &v, CST); return v; }
};

}// UniQ • Released under GPL 3 licence
#include "std.h"
namespace uniq {

#define CST __ATOMIC_SEQ_CST

template<class T>
struct Atomic {
  T value;
  Atomic() { }
  Atomic(const Atomic&);
  constexpr Atomic(T v){ value=v; }

  Atomic& operator = (const Atomic&);
  T operator = (T v) { __atomic_store(&value, &v, CST); return v; }

  inline T add(const T v) { return __atomic_fetch_add(&value, v, CST); }
  inline T sub(const T v) { return __atomic_fetch_sub(&value, v, CST); }
  inline T xch(T v) {  return __atomic_exchange(&value, v, CST); }
  inline bool CAS(T old, T new_) {  return __atomic_compare_exchange(&value, &old, &new_, false, CST, CST); }

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

//========================================================================= test
#include "test.h"
void test_Atomic(){
  Atomic<int> i(1); CHECK(i == 1);

  i = i+1; CHECK(i == 2);
  i = i-1; CHECK(i == 1);
  i |= 3;  CHECK(i == 3);
  i &= 1;  CHECK(i == 1);

  CHECK(i.CAS(1,2) && i == 2);
  CHECK(!i.CAS(3,1) && i == 2);

  CHECK(i++ == 2 && i == 3);
  CHECK(i-- == 3 && i == 2);
  CHECK(++i == 3);
  CHECK(--i == 2);
}

}// uniq • Released under GPL 3.0
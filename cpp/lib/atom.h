#include "std.h"
namespace uniq {

#define CST __ATOMIC_SEQ_CST

template<class T>
struct atom {
  T value;
  atom() { }
  atom(const atom&);
  constexpr atom(T v){ value=v; }

  atom& operator = (const atom&);
  T operator = (T v) { __atomic_store(&value, &v, CST); return v; }

  T exchange(T v) {  return __atomic_exchange(&value, v, CST); }
  bool cas(T old, T new_) {  return __atomic_compare_exchange(&value, &old, &new_, false, CST, CST); }

  void operator += (T v) { __atomic_fetch_add(&value, v, CST); }
  void operator -= (T v) { __atomic_fetch_sub(&value, v, CST); }
  void operator &= (T v) { __atomic_fetch_and(&value, v, CST); }
  void operator |= (T v) { __atomic_fetch_or(&value, v, CST); }
  void operator ^= (T v) { __atomic_fetch_xor(&value, v, CST); }
  // T nand(T v) { return __atomic_fetch_nand(&value, v, CST); }
 
 	T operator++(int){ T v = value; value += 1; return v;}; // v++
 	T operator--(int){ T v = value; value -= 1; return v;}; // v--
  T& operator++(){ value++; return value; }; // ++v
  T& operator--(){ value--; return value; }; // --v

  operator T() const { T v; __atomic_load(&value, &v, CST); return v; }
};

//========================================================================= test
#include "test.h"
void test_atom(){
  atom<int> i(1); CHECK(i == 1);

  i = i+1; CHECK(i == 2);
  i = i-1; CHECK(i == 1);
  i |= 3;  CHECK(i == 3);
  i &= 1;  CHECK(i == 1);

  CHECK(i.cas(1,2) && i == 2);
  CHECK(!i.cas(3,1) && i == 2);

  CHECK(i++ == 2 && i == 3);
  CHECK(i-- == 3 && i == 2);
  CHECK(++i == 3);
  CHECK(--i == 2);
}

}// uniq • Released under GPL 3.0
// https://raw.githubusercontent.com/jasonzliang/scalableMLAssignments/7ac6b37d9c8044b57f0a43c146ea7a941928c9eb/hw5/Galois-2.2.1/include/Galois/c%2B%2B11-compat/atomic.h
// int main() { uniq::test_hello(); }
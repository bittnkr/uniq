#pragma once
typedef unsigned char       u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef __uint128_t       u128;

typedef union { u16 n; struct{  u8 hi;  u8 lo; }; u8 bytes[sizeof( u16)];} U16;
typedef union { u32 n; struct{ u16 hi; u16 lo; }; u8 bytes[sizeof( u32)]; struct{  u8 a;  u8 b;  u8 c;  u8 d;};} U32;
typedef union { u64 n; struct{ u32 hi; u32 lo; }; u8 bytes[sizeof( u64)]; struct{ u16 a; u16 b; u16 c; u16 d;};} U64;
typedef union {u128 n; struct{ u64 hi; u64 lo; }; u8 bytes[sizeof(u128)]; struct{ u32 a; u32 b; u32 c; u32 d;};} U128;

typedef char         i8;
typedef short       i16;
typedef int         i32;
typedef long long   i64;
typedef __int128_t i128;

struct{int h:1; int v:7;} U7;
struct{int h:2; int v:6;} U6;
struct{int h:3; int v:6;} U5;
struct{int h:4; int v:4;} U4;
struct{int h:5; int v:3;} U3;
struct{int h:6; int v:2;} U2;
struct{int h:7; int v:1;} U1;

struct{int s:1; /*h: 0*/ int v:7;} I7;
struct{int s:1; int h:1; int v:6;} I6;
struct{int s:1; int h:2; int v:6;} I5;
struct{int s:1; int h:3; int v:4;} I4;
struct{int s:1; int h:4; int v:3;} I3;
struct{int s:1; int h:5; int v:2;} I2;
struct{int s:1; int h:6; int v:1;} I1;

// floats base
typedef float        f32;
typedef double       f64;
typedef __float128  f128;

// float unions
typedef union { f32 n; struct { u32 mantisa: 23;  u8 exponent: 8; u8 s:1;} parts;}  F32;
typedef union { f64 n; struct { u64 mantisa: 52; u16 exponent:11; u8 s:1;} parts;}  F64;
typedef union {f128 n; struct {u128 mantisa:112; u16 exponent:15; u8 s:1;} parts;} F128;

// platform dependent integer types
#if __WORDSIZE == 64
  typedef i64 integer;
  typedef u64 uinteger;
  typedef i128 ioverflow;
  typedef u128 uoverflow;
#else // __WORDSIZE == 32
  typedef i32 integer;
  typedef u32 uinteger;
  typedef i64 ioverflow;
  typedef u64 uoverflow;
#endif

#define BITS(n) sizeof(n)*CHAR_BIT

u32 rehash(u32 n){
  U32 r, s = {~n};
  r.a = s.b ^ s.c ^ s.d;
  r.b = s.c ^ s.d ^ s.a;
  r.c = s.d ^ s.a ^ s.b;
  r.d = s.a ^ s.b ^ s.c;
  return r.n;
};

u64 rehash(u64 n){
  U64 r, s = {~n};
  r.a = s.b ^ s.c ^ s.d;
  r.b = s.c ^ s.d ^ s.a;
  r.c = s.d ^ s.a ^ s.b;
  r.d = s.a ^ s.b ^ s.c;
  return r.n;
};

const u64 KILO = 1e3;  const double MILI = 1e-3; 
const u64 MEGA = 1e6;  const double MICRO = 1e-6; 
const u64 GIGA = 1e9;  const double NANO = 1e-9; 
// uniq • Released under GNU 3.0

#pragma once
typedef unsigned char       u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef __uint128_t       u128;

struct{  u8 hi;  u8 lo; }  U16;
struct{ u16 hi; u16 lo; }  U32;
struct{ u32 hi; u32 lo; }  U64;
struct{ u64 hi; u64 lo; } U128;

typedef char         i8;
typedef short       i16;
typedef int         i32;
typedef long long   i64;
typedef __int128_t i128;

struct{  i8 hi;  u8 lo; }  I16;
struct{ i16 hi; u16 lo; }  I32;
struct{ i32 hi; u32 lo; }  I64;
struct{ i64 hi; u64 lo; } I128;


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
typedef union { f32 value; struct { u32 mantisa: 23;  u8 exponent: 8; u8 s:1;} parts;}  F32;
typedef union { f64 value; struct { u64 mantisa: 52; u16 exponent:11; u8 s:1;} parts;}  F64;
typedef union {f128 value; struct {u128 mantisa:112; u16 exponent:15; u8 s:1;} parts;} F128;

// platform dependent integer types
#define BITS __WORDSIZE

#if BITS == 64
  typedef i64 integer;
  typedef u64 uinteger;
  typedef i128 ioverflow;
  typedef u128 uoverflow;
#else // BITS == 32
  typedef i32 integer;
  typedef u32 uinteger;
  typedef i64 ioverflow;
  typedef u64 uoverflow;
#endif

// tests ======================================================================  
#include "test.h"
namespace test{
  void test_numtypes(){ 
    CHECK(sizeof(integer) == BITS/8);
  };
}
// part of UniQ library - released under GPL 3.0

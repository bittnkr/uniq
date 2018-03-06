#include <stdio.h>      /* printf */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */
#include <bits/stdc++.h>
#pragma once

typedef long long int64;

clock_t Clock = 0;

clock_t ticks(){
  return clock() - Clock;
}

float startTimer(const char* msg="")
{
  if(msg) printf(msg);
  Clock = clock();
  return (float) ticks()/(CLOCKS_PER_SEC);
}

float timer()
{
  return ((float)ticks())/(CLOCKS_PER_SEC);
}

float printTimer(const char* msg="")
{
  float r = timer();
  if(msg!="") printf("%s: %.3fs\n", msg, r);
  return r;
}

int64 roundsPerSec(int64 done)
{
  return round((float)done/timer());
}

int64 test_utils()
{
  printf("clockSpeed():%d", CLOCKS_PER_SEC);
}

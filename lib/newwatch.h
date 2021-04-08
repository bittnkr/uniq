#include "uniq.h"
void* operator new(size_t size)
{
  void* r= malloc(size);
  log("malloc(", size, ")", r);
  return r;
}
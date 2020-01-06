# uniQ

## Introduction

Imagine a long row of people wanting to cross a bridge or a ferry... 

This ferry only alow one car/person to cross at any time.

The gate is open, the car get in, the crossing happens and after the delivery, the ferry get back to take another.

This operation takes time and has a price.

In computers programming the "transport" problem is about transfering data between different process/actors of the system.

When two processors live in the same memory space, the faster approch is just allow the cores to share the memory instead of transfering the data.

But... 

## How to allow the memory sharing, before processess starts messing with each other?

The common solution is the use of Semaphores, Mutexes and Critical Sessions to create locks, making the resource unavailable while some operation is running. 

But... this pattern **lock/use/release** has a problem: While a operation does not finish, another cannot start. 

In modern operational systems, the act of transfering the processing lines between cores is called thread switching. An expensive operation that sometimes takes thousand of processor cycles.

## The seminal question

What is the most efficient way to share a memory space between multiple threads without any kind of locking between them? 

## The circular buffer

Here starts our quest for the lock-free queue.

...and our question gets a little more specific:

How construct a circular buffer that receive and delivery data without any kind of locking, allowing actors to enter or exit the flow at limit zero time?

With only two threads the solution is known, and called two-thread consensus. But no absolute algorithm was known to solve the 3-thread consensus, yet.

The Lock free Queue is one those hard problems in computer science. In a Stackoverflow [question][2] we see comments like:

> I've made a particular study of lock-free data structures in the last couple of years. 
> I've read most of the papers in the field (there's only about fourty or so - although only about ten or fifteen are any real use :-)

> AFAIK, a lock-free circular buffer has not been invented.

In [another S.O. question][3] someone said: 

> Lock-free queues are unicorns.

Searching the literature, I found no more encouraging words: The book [The Art of Multiprocessor Programming][4] asserts that the construction of a lock free queue is impossible:

> Corollary 5.4.1. It is impossible to construct a wait-free implementation of a queue, stack, priority queue, set, or list from a set of atomic registers. 

> Although FIFO queues solve two-thread consensus, they cannot solve 3-thread consensus. (pg. 107)

## The breaktrough

This paper/repository brings a solution to the 3-thread consensus.  

Here I present a **minimum/absolute** way to create a MRMW (multi-read/multi-write) circular queue, allowing input and output for N threads without any kind of locking. Using only 2 atomic registers.

I believe this is the definitive solution to the ABA problem and race conditions in multi-threaded programming. 

Allowing a really free-flow comunication between multiple processor cores. 

To better understand the solution, take a look [under the hoods][5]. If you like to put your hands dirt and dive right into de code, start at [test.cpp and queue.h][6].

[1]: https://en.wikipedia.org/wiki/Queue_(abstract_data_type) 
[2]: https://stackoverflow.com/a/890269/9464885
[3]: https://stackoverflow.com/questions/6089029/lock-free-queue#comment7056198_6089029
[4]: https://www.amazon.com.br/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376
[5]: https://github.com/bittnkr/uniq/blob/master/doc/under-the-hoods.md
[6]:https://github.com/bittnkr/uniq/blob/master/cpp
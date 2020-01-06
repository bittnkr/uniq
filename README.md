# The Lock Free Queue


This repository brings a solution to the 3-thread consensus.  

I believe this is the definitive solution to the ABA problem and race conditions in multi-threaded programming. 

Allowing a really free-flow comunication between multiple processor cores. 

## The breakthrough

<<<<<<< HEAD
The Lock free [Queue][1] is one those hard problems in computer science. In a Stackoverflow [question][2] we see comments like:
=======
The Lock free Queue is one those hard problems in computer science. In a Stackoverflow [question][2] we see comments like:
>>>>>>> dfd94af922eae24200db644a41c770ed8c1432f8

> I've made a particular study of lock-free data structures in the last couple of years. I've read most of the papers in the field (there's only about fourty or so - although only about ten or fifteen are any real use :-)

> **AFAIK, a lock-free circular buffer has not been invented.**

In [another S.O. question][3] someone said: 

> Lock-free queues are unicorns.

Searching the literature, I found no more encouraging words: The book [The Art of Multiprocessor Programming][4] asserts that the construction of a lock free queue is impossible:

> Corollary 5.4.1. It is impossible to construct a wait-free implementation of a queue, stack, priority queue, set, or list from a set of atomic registers. 

> Although FIFO queues solve two-thread consensus, they cannot solve 3-thread consensus. (pg. 107)

Here I present a **minimum/absolute** way to create a MRMW (multi-read/multi-write) circular queue, allowing input and output for N threads without locking. Using only 2 atomic registers.

## Testing

To better understand the solution, take a look [under the hoods][5]. If you like to put your hands dirt and dive right into de code, start at [test.cpp and queue.h][6]. 

We have implementations in C# and pascal too.

<<<<<<< HEAD
To test this on your computer:
```
git clone https://github.com/bittnkr/uniq
cd uniq/cpp
sh build.sh
```
=======
To better understand the solution, take a look [under the hoods][5]. If you like to put your hands dirt and dive right into de code, start at [test.cpp and queue.h][6].
>>>>>>> dfd94af922eae24200db644a41c770ed8c1432f8

[1]: https://en.wikipedia.org/wiki/Queue_(abstract_data_type) 
[2]: https://stackoverflow.com/a/890269/9464885
[3]: https://stackoverflow.com/questions/6089029/lock-free-queue#comment7056198_6089029
[4]: https://www.amazon.com.br/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376
[5]: https://github.com/bittnkr/uniq/blob/master/doc/under-the-hoods.md
[6]:https://github.com/bittnkr/uniq/blob/master/cpp
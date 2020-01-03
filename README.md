# uniQ

A system wide single queue to solve the synchronicity impedance between all processor cores and threads.

## About lock free queues

This repository contains a proof of concept of a lock-free [queue][1]. 

It is in early stages, but this implementation, resolves the ABA problem in multi thread programs. 

Lock free Queues are an incredible hard problem in computer science. Here a [SO question][2] with a broader view of the problem. Where we can see comments as 

> I've made a particular study of lock-free data structures in the last couple of years. 
> I've read most of the papers in the field (there's only about fourty or so - although only about ten or fifteen are any real use :-)
> AFAIK, a lock-free circular buffer has not been invented.

The lock free Queue was declared impossible in the book [The Art of Multiprocessor Programming. (page 107)][3] 
> Corollary 5.4.1. It is impossible to construct a wait-free implementation of a queue, 
> stack, priority queue, set, or list from a set of atomic registers. 
> Although FIFO queues solve two-thread consensus, they cannot solve 3-thread consensus.

The authors of the book, affirm the imposibilitiy (until now) of creating a MRMW (multi-read/multi-write) queue, wich permits input for N threads/cores in a linear way, without any kind of locking. Using only 2 atomic registers.

This repository holds a proof that it is possible to create such structure. 

Look at test.cpp and queue.h for better understanding.

 [1]: https://en.wikipedia.org/wiki/Queue_(abstract_data_type)
 [2]: https://stackoverflow.com/questions/871234/circular-lock-free-buffer
 [3]: https://www.amazon.com.br/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376
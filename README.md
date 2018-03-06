# uniq
## A lock free queue

This repository contains proof of concept of a new way of thinking paralelism and concurrency.  A simple solution for a very complex problem. A single queue solve the synchronicity impedance between all processor cores and threads.

The lock free Queue was declared impossible in the book The Art of Multiprocessor Programming. (page 107)
> Corollary 5.4.1. It is impossible to construct a wait-free implementation of a queue, 
> stack, priority queue, set, or list from a set of atomic registers. 
> Although FIFO queues solve two-thread consensus, they cannot solve 3-thread consensus.

This repository contains a proof of the existence of such construct that permits input for N threads/cores in a linear way, without any kind of locking. Using only 2 atomic registers.

Look at test.cpp and queue.h for better understanding.

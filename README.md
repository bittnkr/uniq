# The Lock Free [Queue][1]

In a [question in Stackoverflow][2] about lock-free queues, the most upvoted answer (jan/20) is:

> I've made a particular study of lock-free data structures in the last couple of years. I've read most of the papers in the field (there's only about fourty or so - although only about ten or fifteen are any real use :-)

> **AFAIK, a lock-free circular buffer has not been invented.**

In [another S.O. question][3] someone said: 

> Lock-free queues are unicorns.

Searching the literature, we found no more encouraging words: The book [The Art of Multiprocessor Programming][4] asserts that the construction of a wait-free queue is impossible:

> Corollary 5.4.1. **It is impossible to construct a wait-free implementation of a queue**, stack, priority queue, set, or list **from a set of atomic registers**. 

> Although FIFO queues solve two-thread consensus, **they cannot solve 3-thread consensus**. (pg. 107)

## The Discovery

After years of investigation and lot of tests, I finally destiled a bare minimum solution to this problem, which I'm pretty sure that it is wait-free.
I believe that is the smallest/simplest solution ever found. 
  
In this paper/repository, I did my best to bring only the essential to the compreension of the problem and its solution. Focusing on what really matters. And yet giving some code ready to be used.


Here is some verified facts and features:

* N threads (tested up to 512).
* N buffer size (minimum 1 seat).
* Constant cost per operation O(1).
* Only 2 atomic variables.
* No locks or mutexes.
* Freely preempted.
* Zero checksum.

Follow a compreensive description of the algorithm.

If you like to put your hands dirt and dive right into de code, start at [test.cpp and queue.h][6]. (We have implementations in **C#**, **JavaScript** and **Pascal** too.)

# Under the Hood

A **bare minimum** solution to the 3-thread consensus, implemented as a MRMW (multi-read/multi-write) circular buffer. In the context of a multi-threaded producer/consumer testcase.

For the sake of simplicity, in this docs I used a simplified JavaScript pseudocode, familiar for anyone using C-like languages. For the real thing, refer to source code.

## The Queue object

```cpp
class Queue(size) {
  data = Array(size); 
  in = 0, out = 0
  mask = size-1
   
  push(item) // send the item & return the id of the job 
  pop() // get the next item.
}
```

## Properties

`data` Is a common array (non atomic). Updated without locking. Secured by `in` & `out`. 

`size` must be a power of 2 (1, 2, 4, 8, 16...) The buffer will be indexed by binary mask (`data[t & mask]`) limiting the memory access inside. **At minimum, a single bit**.

`in` Holds a the ID of the next element. Always ahead or equal `out`.

`out` Is the ID of the last element (next to be removed). Never greater than `in`

Both are **atomic variables**, always incremented. 

No boundary check is needed, because `mask` and the way integer overflow happens. 

## Methods

Here, the heart of the solution, where we solve the 3-thread consensus.

### Inserting data into the queue. The `push()` operation.

```cpp
push(item) 
{
  let i
  do {
    i = in // get a local copy the next id

    while (i - out == size) sleep() // if full, wait for space
  
  } while ( (data[i & mask]) || (CompareAndSwap(in, i+1, i) != i) )
  // if the seat is lost or CAS failed, try again. 

  data[i & mask] = item // now is safe to update the buffer using local i
  return i // id of the job 
}
```

*  If the thread is preempted at any point between `i = in` and `CompareAndSwap(input, i+1, i)`, on return the CAS will fail and the loop go to the next seat. Without any kind of locking.

* I think that `(data[i & mask]) ||` should not be really neeed, but my computer hangs without it. And it prevents the use of the expensive CAS instruction. 

* Here I check for nullability of the content. But in the C++ implementation it was replaced by an isFree boolean array.

### Removing data from the queue. The `pop()` operation

```cpp
pop() 
{
  let o
  do {
    o = out // id of the next candidate

    while (o == in) sleep() // if empty, wait for items

  } while ( !(data[o & mask]) || CompareAndSwap(out, o+1, o) != o )
  // if the candidate is gone or CAS failed, try again. 

  o &= mask // round to fit the buffer
  int r = data[o] // save the return
  buffer[o] = 0 // release the seat
  return r
}
```

Both methods have two nested `while()` loops: 

* First we get the next seat/candidates with `i = in` and `o = out`

* Then we check if the buffer is full or empty, `sleep()`ing until state change. 

* If the seat/candidate is available, try increment the atomic register `CompareAndSwap(out, o+1, o)`. 

* If the CAS fail (`!=o`), go to the next seat. 

## Notes

* The load operation used by `data[h] = 0` and `!(data[o & mask])` are naturaly atomic. 

* The last thing done by `pop()` is release the seat. 

* The flow happens without any kind of lock.

## States

If `in == out` the Queue is empty, 

If `in-out == size` the Queue is full. 

In these cases the queue do not lock, but make a voluntary preemption calling the `sleep()` function.

## Testing - The producer/consumer pattern

Now, with our Queue defined, its time to put it on fire... 

Lets create two groups of threads: one producing and another consuming data.

The first type of thread is the **`producer`**, it will put a bunch of numbers into the queue `Q`. 

```JavaScript
producer() 
{
  for(let i=1; i <= N; i++)
    Q.push(i) 
    
  Q.push(-1) // end of job

  Total += N
  log("Produced:", N)
}
```

Here we push -1 into the queue to signal the termination of the job. Also can be implemented with external flags.

Now, the work of the **`consumer`** is remove elements out of the queue, until receiving a termination.

```cpp
consumer()
{
  let value, sum = 0

  do { 
    value = Q.pop()
    sum += value
  } while( value != -1 ) 

  Total -= sum 
  log("Consumed:", sum)
}
```

Producers increment ``Total`` and consumers decrement it. At the end, it must be zero, proofing that there is no leaks.

## Running the horses

Lets start our threads and check if what we get out the queue its equal to what we pushed into. 

```JavaScript
let producers = []
  , consumers = []

for(int i=0; i < 4; i++)
{
  producers[i] = Thread(producer)
  consumers[i] = Thread(consumer)
}

wait(producers, consumers)

log("Total: %d", Total)
```

* Here we create 8 threads to flow data. For sake of simplicity, with the same number of producers and consumers, but it works equally in asymmetric conditions. 
* I've tested with at most 512 threads on an old Windows XP machine, (where I got the better performace). 

This is the output I got from the [C++ implementation][6], flowing 10M items:

```
Creating 4 producers & 4 consumers
Flowing 10.000.000 items trough the queue.

Produced: 2.500.000
Consumed: 1.349.413
Produced: 2.500.000
Consumed: 1.359.293
Consumed: 3.876.642
Produced: 2.500.000
Consumed: 3.414.652
Produced: 2.500.000

Total: 0

real    0m0,581s
```

Note that **producers** always pushed the same amount of items (2.5M), but **consumers** get different quantities, ranging from 1.3 to 3.8M. This is the expected behaviour. 

Follow our expected `Total:0` proofing that all produced was consumed. 

Then the time took by the operation: **581 ms**. A throughput of **17.2 M flow/s**. (Enough for an old Dell M6300 Core duo).

I made a series of benchmarks, varying the buffer size and the number of threads. Follow the results.

## Benchmarks - Measuring the flow

![Throughput x Buffer Size](https://i.stack.imgur.com/TgkKs.png)

With default buffer size (64), varying the number of threads:
 
![Throughput x Threads](https://i.stack.imgur.com/laMSX.png)

* The cost per operation with 2 threads is the same for 512.
I think this is the definitive proof lock-freedom.

Comments, benchmarks and use cases are welcome.

---

Code released under GNU 3.0 and docs under Creative Commons (CC BY-SA 3.0) licenses. 

[1]: https://en.wikipedia.org/wiki/Queue_(abstract_data_type) 
[2]: https://stackoverflow.com/a/890269/9464885
[3]: https://stackoverflow.com/questions/6089029/lock-free-queue#comment7056198_6089029
[4]: https://www.amazon.com.br/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376
[6]:https://github.com/bittnkr/uniq/blob/master/cpp
[7]: https://en.wikipedia.org/wiki/Curry%E2%80%93Howard_correspondence

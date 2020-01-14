# The Lock Free [Queue][1]

In a [question about in Stackoverflow][2] , the most upvoted answer is:

> I've made a particular study of lock-free data structures in the last couple of years. I've read most of the papers in the field (there's only about fourty or so - although only about ten or fifteen are any real use :-)

> AFAIK, a lock-free circular buffer has not been invented.

In [another S.O. question][3] someone said: 

> Lock-free queues are unicorns.

Searching the literature, we found no more encouraging words: The book [The Art of Multiprocessor Programming][4] asserts that the construction of a wait-free queue is impossible:

> Corollary 5.4.1. **It is impossible to construct a wait-free implementation of a queue**, stack, priority queue, set, or list **from a set of atomic registers**. 

> Although FIFO queues solve two-thread consensus, **they cannot solve 3-thread consensus**. (pg. 107)

## Not knowing that was impossible...

Long story short, after years of investigation and lot of tests, I finally destiled a bare minimum solution to this problem, which I'm pretty sure that it is wait-free. (If you can refute, please do it.)
  
In this paper/repository, I did my best to bring only the essential to the compreension of the problem and its solution. Focusing on what really matters. 

Don't let its simplicity fools you. This is the result of years of work, and I believe that is the smallest/simpler solution ever found. 

After releasing the idea, I received some objections about the asuredness of my "claims". The most common is: 

**How can you asure that it is really lock-free?**

## The proof is in the puding

The [Curry-Howard Correspondence][7] says: 
> a proof is a program, and the formula it proves is the type for the program.

A fancy way to say: **The program is the proof of itself**. 

Here is some verified facts and features of this program/formula:

* N threads
* N buffer size (minimum 1)
* 2 atomic variables
* No locks or mutexes.
* Freely preempted.
* O(1): constant cost per operation.
* zero checksum.

If you like to put your hands dirt and dive right into de code, start at [test.cpp and queue.h][6]. (We have implementations in C# and pascal too.)

Follow a compreensive description of the algorithm.

# Under the hoods

A **bare minimum** solution to the 3-thread consensus, implemented as a MRMW (multi-read/multi-write) circular buffer. In the context of a multi-threaded producer/consumer testcase.

For the sake of simplicity, I used a simplified JavaScript pseudocode, familiar for anyone using C-like languages. For the real thing, refer to source code.

## The Queue object 

```cpp
class Queue(size) {

  data = Array(size); 
  in = 0, out = 0
  mask = size-1
   
  push(item) // send the item & return an id.

  pop() // get the next item.
}
```

## Properties

`data` Is a common array (non atomic). Updated without locking. Secured by `in` & `out`. 

`size` must be a power of 2 (1, 2, 4, 8, 16...) The buffer will be indexed by binary mask (`data[t & mask]`) limiting the memory access inside. **At minimum, a single bit**.

`in` Holds a the ID of the next element. Always ahead or equal `out`.

`out` Is the ID of the last element (Next to be removed). Never greater than `in`

Both are **atomic variables**, always incremented. 

No boundary check is needed, because `mask` and the way integer overflow happens. 

## Methods

Here, the heart of the solution, where we solve the 3-thread consensus.

### Inserting data into the queue. The `push()` method.

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

// released under GNU 3.0 Licence 
```

*  If the thread is preempted at any point between `i = in` and `CompareAndSwap(input, i+1, i)`, on return the CAS will fail and the loop go to the next seat. Without any kind of locking.

* I think `(data[i & mask]) ||` can be safely removed, but I got better benchmarks with it, preventing the use of the CAS instruction.

### Removing data from the queue. The `pop()`

```cpp
pop() 
{
  let o
  do {
    o = out // id of the next candidate

    while (o == in) sleep() // if empty, wait for items

  } while ( !(data[o & mask]) || CompareAndSwap(output, o+1, o) != o )
  // if the candidate is gone or CAS failed, try again. 

  o &= mask // round to fit the buffer
  int r = data[o] // save the return
  buffer[o] = 0 // release the seat
  return r
}

// released under GNU 3.0 Licence 
```

Both methods have two nested `while()` loops: 

* First we get the next seat/candidates with `i = in` and `o = out`

* Then we check if the buffer is full or empty, `sleep()`ing until state change. 

* If the seat/candidate is available, go increment the atomic register `CompareAndSwap(output, o+1, o)`. 

* If the CAS fail `!=o`, go to the next seat. 

## Notes

* The load operation used by `data[h] = 0` and `!(data[o & mask])` are naturaly atomic. 

* The last thing done by pop() is release the seat. 

* The flow happens without any kind of lock, at cost of a CAS instruction.

## States

If `in == out` the Queue is empty, 

If `in-out == size` the Queue is full. 

In these cases the queue do not lock, but make a voluntary preemption calling the `sleep()` function.

## Testing - The producer/consumer pattern

Now, with our Queue defined, its time to put it on fire... 

Lets create two groups of threads: one producing and another consuming data.

The first is the **`producer`**, it put a bunch of numbers into the queue `Q`. 

```JavaScript
producer() 
{
  for(int i=1; i <= N; i++)
    Q.push(1) // or i 
    
  Q.push(i)

  Total += N
  log("Produced:", N)
}
```

The (-1) signal the termination of the job. Also can be implemented with external flags.

Now, the work of our **`consumer`**: remove elements out of the queue, until receiving a termination.

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

Producers increment ``Total`` and consumers decrement. At the end, **it must be zero**. The proof that there is no leaks.

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

* Here we create 8 threads to flow data. For sake of simplicity, the same number of producers and consumers, but it works equally in asymmetric conditions. 

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

Follow our expected `Total:0` proofing that all the produced was consumed. 

Then the time took by the operation: **581 ms**. A throughput of 17.2 M/s. (Enough for an old Dell M6300 Core duo).

I made a series of benchmarks, varying the buffer size and the number of threads. Follow the results.

## Benchmarks - Measuring the flow

![Throughput x Buffer Size](https://i.stack.imgur.com/TgkKs.png)

With default buffer size, varying the number of threads.
 
![Throughput x Threads](https://i.stack.imgur.com/laMSX.png)

* **O(1)** The cost per operation for 2 threads is the same for 512

Comments, benchmarks and use cases are welcome.

---

Code released under GNU 3.0 License and docs under Creative Commons (CC BY-SA 3.0). 

[1]: https://en.wikipedia.org/wiki/Queue_(abstract_data_type) 
[2]: https://stackoverflow.com/a/890269/9464885
[3]: https://stackoverflow.com/questions/6089029/lock-free-queue#comment7056198_6089029
[4]: https://www.amazon.com.br/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376
[6]:https://github.com/bittnkr/uniq/blob/master/cpp
[7]: https://en.wikipedia.org/wiki/Curry%E2%80%93Howard_correspondence

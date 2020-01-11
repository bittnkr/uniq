# A Lock Free Queue

The solution to the 3-thread consensus.  

## The problem

Lock free [Queue][1] is one those hard problems in computer science. 

In a [question in Stackoverflow][2], the most upvoted answer is:

> I've made a particular study of lock-free data structures in the last couple of years. I've read most of the papers in the field (there's only about fourty or so - although only about ten or fifteen are any real use :-)

> **AFAIK, a lock-free circular buffer has not been invented.**

In [another S.O. question][3] someone said: 

> Lock-free queues are unicorns.

Searching the literature, we found no more encouraging words: The book [The Art of Multiprocessor Programming][4] asserts that the construction of a wait-free queue is impossible:

> Corollary 5.4.1. **It is impossible to construct a wait-free implementation of a queue**, stack, priority queue, set, or list **from a set of atomic registers**. 

> Although FIFO queues solve two-thread consensus, **they cannot solve 3-thread consensus**. (pg. 107)

## The Breakthrough

Here I show a **minimum/absolute** solution to the 3-thread consensus, implemented as a MRMW (multi-read/multi-write) circular queue. Using **2 atomic registers**.

If you like to put your hands dirt and dive right into de code, start at [test.cpp and queue.h][6]. (We have implementations in C# and pascal too.)

Follow a detailed description of the algorithm. 

# Under the hoods

The internals and a testcase.

For the sake of simplicity, I used a simplified JavaScript pseudocode, familiar for anyone using C-like languages. Only the essential, for the real thing, refer to source code. Now the starting point of our quest:

## The Queue object 

```JavaScript
class Queue(size) {

  input = 0, output = 0  
  buffer = Array(size)

  push(item) // send the item & return an id.

  pop() // get the next item.
}
```

## Properties

`input` Holds a the ID of the last element. 

`output` The ID of the next element. 

Both are **atomic registers**, always incremented. No boundary check is needed, because the way integer overflow happens.

`buffer` A common **Not atomic** array capable of holding integers or data.  Will be updated without any kind of locking. Ensured by the way that `input` & `output` are updated.

The buffer `size` must be a power of 2 (1, 2, 4, 8, 16...)To be indexed by binary mask (`data[t & mask]`) and limit the memory access inside the buffer. **At minimum, a single bit**.

## States

If `input == output` the Queue is empty, 

If `input-output == size` the Queue is full. 

## Methods

Here is the heart of the solution, where we solve the 3-thread consensus. (Its so small that is incredible that nobody had figured it yet).

### Inserting data into the queue. The `push()` method.

```JavaScript
push(item) 
{
  let i
  do {
    i = in // get a local copy the next id

    while (i - out == size) sleep() // if full, wait for space
  
  } while ( (data[i & mask]) || (CompareAndSwap(input, i+1, i) != i) )
  // if the seat is lost or CAS failed, try again. 

  data[i & mask] = item // update the buffer using local i
  return i // id of the job 
}

// released under GNU 3.0 Licence 
```

### Removing data from the queue. The `pop()`

```JavaScript
pop() 
{
  let o
  do {
    o = out // id of the next candidate

    while (o == input) sleep() // if empty, wait for items

  } while ( !(data[o & mask]) || CompareAndSwap(head, o+1, o) != o )
  // if the candidate is gone or CAS failed, try again. 

  h &= mask // round to fit the buffer
  int r = data[h] // save the return
  data[h] = 0 // set the seat free
  return r
}

// released under GNU 3.0 Licence 
```

In both methods, we have two nested `while()` loops. 

* When the first loop starts, look to the next candidates with `i = input` and `o = output`

* Then we check if the buffer is full or empty, `sleep()`ing until state change.

* if the seat is ok, try increment the atomic register.

If CompareAndSwap() fail, go to the next seat, restarting the loop. 

## Some notes

* The load operation used by `data[h] = 0` and `!(data[o & mask])` are naturaly atomic. 

* The last thing done by pop() is release the seat. 

* The flow continues without locks at cost of a single CAS instruction.

Now, with our Queue defined, its time to put it on fire... 

## Testing - The producer/consumer pattern

Lets create two groups of threads, one producing and another consuming data.

The first is the **`producer`**, it just put a bunch of numbers into the queue `Q`. 

```JavaScript
function producer() 
{
  for(int i=0; i < N; i++)
    Q.push(1) 
    
  Q.push(-1)

  Total += N
  log("Produced:", N)
}
```

To achieve a mininum working example, we pushed just the number 1. But `random()` works equally.

The -1 signal the termination of the job. Also can be implemented with external flags.

Now, the work of our **`consumer`** thread is to remove elements out of the queue, until receiving a termination.

```JavaScript
function consumer()
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

Producers increment ``Total`` and consumers decrement it. At the end, **it must be zero**. This is the proof that there is no leaks.

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

Here we create 8 threads to flow data. For sake of simplicity, the same number of producers and consumers, but it works equally in asymmetric conditions. 

After creating the Threads, we `wait` for termination.

This is the output of the [C++ implementation][6], flowing 10M items:

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

Follow the proof that all items produced was consumed. The expected `Total: 0`. 

Then the time took by the operation: **581 ms**. A throughput of 17.2 M/s. (Enough for an old Dell M6300 Core duo).

## Benchmarks - Measuring the flow

![Throughput x Buffer Size](https://i.stack.imgur.com/TgkKs.png)

Now, with default buffer size (64), varying the number of threads.
 
![Throughput x Threads](https://i.stack.imgur.com/laMSX.png)

The number of threads does not affect the overall performance of the system. **The flow for 2 threads is the same for 512**. 

It's lock-free dude!

## Downloading

To get fully functional C++, C# and pascal implementations just clone the repository:

```
git clone https://github.com/bittnkr/uniq
cd uniq/cpp
sh ./build.sh
```

This is a work in progress. Comments and benchmarks and use cases  are welcome.

---

Code released under **GNU 3.0** License and docs under Creative Commons (CC BY-SA 3.0). 

To use this invention in a hardware or closed source software, a special permission is needed from the author. (bittnkr@gmail.com) 

[1]: https://en.wikipedia.org/wiki/Queue_(abstract_data_type) 
[2]: https://stackoverflow.com/a/890269/9464885
[3]: https://stackoverflow.com/questions/6089029/lock-free-queue#comment7056198_6089029
[4]: https://www.amazon.com.br/Art-Multiprocessor-Programming-Revised-Reprint/dp/0123973376
[6]:https://github.com/bittnkr/uniq/blob/master/cpp
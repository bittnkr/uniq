# Under the hoods

A lock free circular buffer.

For the sake of simplicity in the docs **I'm using a JavaScript like pseudocode**, familiar for anyone using JS, C, C++, C# or similar languages. For the real thing, refer to source code.

## The Queue object 

This is the interface for the Queue object and starting point of our quest. 

```JavaScript
class Queue(size) {

  var head = 0, tail = 0
    , buffer = Array(size)

  push(item) // input an item and return an id.

  pop() // remove & return the next item.
}
```

## Properties

`tail` Where we register the new elements. Holds a the ID of last inserted element. 

`head` from where elements get out. Register the next element to be removed.

`tail & head` Are **atomic registers** of simple integers. Topped by the ``size`` of the buffer. **At minimum, a single bit**.

`buffer` Any contigous space of memory capable of holding integers or data. Don't need any kind of locking to be read or written. This is ensured by the atomic registers `tail` & `head`. 

`size` **must be a power of two**. (At minimum 1).

## Methods
Here is the heart of the solution. Where we solve the 3-thread consensus. (Its so simple that is incredible that nobody had figured it yet.)

The most important elements are the 2 atomic registers `tail` & `head`, aka **input & output**. When `head == tail`, the Queue is empty, when `tail-head == size` the Queue is full. 

These are 2 integer numbers that are always incremented. 

Note we wrap the values using the `mask` variable, so we dont need any boundary check because of the way the Integer Overflow happens. The only condition is that the buffer size must be a power of 2.

### Inserting data onto the queue. The `push()` method.

```JavaScript
push(item) 
{
  assert(item, "Queue.push(item): Item can't be null");

  var t;
  do {
    t = tail
    while (t - head == size) sleep(0) // if full, wait for space

  } while ( (data[t & mask]) || (CompareExchange(tail, t+1, t) != t) )

  data[t & mask] = item // now is safe to update the buffer
}
// Code released under GNU 3.0 Licence 
```

### Removing data from the queue. The `pop()` method 
```JavaScript
pop() 
{
  var h
  do {

    h = head
    while (h == tail) sleep() // if empty, wait for items

  } while ( !(data[h & mask]) || CompareExchange(head, h+1, h) != h )

  h &= mask
  int r = data[h]
  data[h] = 0 // release the seat
  return r
}
// Code released under GNU 3.0 Licence 
```
In these two methods is where the real action happens, and where lies the inovation.

Basically we have 2 nested while loops. 

When the first loop starts, we get a copy the value Atomic register on `h = head`, then we check if the buffer is full or empty, in this case, we must release the Thread line to permit other actor remove or add data.

In the last while condition, we check if the buffer has something (on push) or is empty (on pop), and then we try to update the atomic register.

Note that there is no locking here, just the update of the atomic registers. 

If any of these operations fail, the loop just restarted getting a value for the next seat.
 
## Testing - The producer/consumer pattern

Now, with the our Queue defined, its time to put it on fire... 

Here we create two groups of threads, one group producing and another consuming data.

The first thread type is called the **producer**, it pushes a bunch of numbers into the queue `Q`. 

```JavaScript
function producer() 
{
  for(int i=0; i < ITEMS; i++)
    Q.push(1) 
    
  Q.push(-1)

  Total += ITEMS
  log("Produced:', ITEMS)
}
```

To achieve **mininum working example** and **maximum benchmarks**, we allways pushed the number 1, but a positive `random()` must work equally.

We signalize the termination of the job with a special constant (-1). Also can be implemented with external flags.

Now, the work of our **consumer** is remove elements out of the queue, until receiving a termination value. At minimum:

```JavaScript
function consumer()
{
  var value, sum = 0

  do { 
    value = Q.pop()
    sum += value
  } while( value > 0 ) 

  Total -= sum

  log("Consumed:", items)
}
```

Now its a matter of starting the threads and check if what we get out the queue its equal to what we pushed into. Time to fry the processors :D

```JavaScript
var producers = []
  , consumers = []

for(int i=0; i < 4; i++)
{
  producers[i] = Thread(producer)
  consumers[i] = Thread(consumer)
}

wait(producers, consumers)

log("Sum: %d", Total)
```

Here we create 8 of threads to flow data (4 produceres & 4 consumers). 

For simplicity, whe create the same number of the producers and consumers, but it works equally in assimetric conditions. 

After creating the Threads, we call the ``wait`` function to wait for the termination of the jobs.

Producers incremented the variable ``Total`` and consumers decrement it. At the end, the ``Total`` value **must** be zero. This is the proof that there is no leaks.

The results I get flowing 10M items through the test.cpp:

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
Note that **producers** allways pushed the same amount of items (2.5M), but the **consumers** consumed different quantities, (ranging from 1.3 to 3.8M). 

This is the normal behaviour. The only condition we have, is the sum of items consumed must be equal the produced. This was ensured by the global variable Total.

The last line is the time took by the operation, in my computer, 581 ms to flow 10 million itens. A throughput of 17.2 M Items/s, wich is a pretty impressive for an old Dell M6300 Core duo.

## Testing
To get a fully functional C++ implementation just clone this repository:

```
git clone https://github.com/bittnkr/uniq
cd uniq/cpp
sh build.sh
```

## Benchmarks

Here we have the measurements varing the buffer size. 

![Throughput x Buffer Size](https://i.stack.imgur.com/TgkKs.png)

Fixing the buffer size on the default size (64 positions) and varying the number of Threads.
 
![Throughput x Threads](https://i.stack.imgur.com/laMSX.png)

Note how the number of threads does not affect the overall performance of the system. The flow for 2 threads is almost the same for 512. (It's lock-free dude!)

---
This is a work in progress. Your comments and benchmarks are wellcome. 

Code released under **GNU 3.0** and docs under Creative Commons License (CC BY-SA 3.0)
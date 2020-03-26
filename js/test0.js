// This is a early attemp to migrate uniQ lib to JavaScript. 
// SINGLE THREADED WITH Promises
// See README.md for details - Released under GNU 3.0

const AtomicInt32 = () => new Int32Array(new SharedArrayBuffer(Int32Array.BYTES_PER_ELEMENT))
  , AtomicInt32Array = (length) => new Int32Array(new SharedArrayBuffer(Int32Array.BYTES_PER_ELEMENT * length))
  , sleep = () => new Promise(resolve => setTimeout(resolve, 0))
// , assert = require('assert')

function Queue(length = 64) {
  // assert(length && !(length & (length - 1)), "Queue size must be a positive power of 2.");

  var buffer = AtomicInt32Array(length)
    , output = AtomicInt32()
    , input = AtomicInt32()
    , mask = length - 1 // bitmask, used for wrapping

  async function push(item) {
    // assert(typeof item == 'number' && item !== 0)
    return new Promise(async resolve => {
      let i;
      do {
        i = input[0];
        while (i - output[0] > mask)
          await sleep() // Atomics.wait(input, 0, i) // if full, wait for space

      } while (buffer[i & mask] || (Atomics.compareExchange(input, 0, i, i + 1) !== i))

      buffer[i & mask] = item
      return resolve(i)
    })
  }

  async function pop() {
    return new Promise(async resolve => {
      let o;
      do {
        o = output[0];
        while (o == input[0])
          await sleep() // Atomics.wait(output, 0, o)
      } while (!buffer[o & mask] || Atomics.compareExchange(output, 0, o, o + 1) !== o)

      let r = buffer[o &= mask]
      buffer[o &= mask] = 0;
      return resolve(r);
    })
  }
  return { push, pop }
}

const Threads = 4
  , Items = 1e7 // how many items will flow trough the queue 
  , Total = AtomicInt32()
  , Q = Queue(1)

async function producer(count) // the producer thread pushes data into the queue
{
  return new Promise(async resolve => {
    let sum = 0
    for (let i = 1; i <= count; i++) {
      await Q.push(i); // push the value 1 to the qeue
      sum += i
    }
    Q.push(-1); // signalize termination with a -1
    Atomics.add(Total, 0, sum);
    resolve(Total)
    console.log("Produced:", sum);
  })
}

async function consumer() // the consumer thread, takes data from the queue
{
  return new Promise(async resolve => {
    var v, sum = 0;
    while ((v = await Q.pop()) > 0)
      sum += v;

    Atomics.add(Total, 0, -sum);
    console.log("Consumed:", sum);
    resolve(sum)
  })
}

async function main() {
  var pairs = Threads / 2
    , count = Items / pairs

  console.log("Creating %d producers & %d consumers", pairs, pairs);
  console.log("to flow %d items trough the queue.", Items);

  var producers = [], consumers = [];

  for (var i = 0; i < pairs; i++) {
    producers[i] = producer(count)
    consumers[i] = consumer()
  }

  await Promise.all(consumers)

  console.log("\nChecksum: %d (it must be zero)\n", Total);
}

main()
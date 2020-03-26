// This is a early attemp to migrate uniQ lib to JavaScript. 
// See README.md for details - Released under GNU 3.0

const sleep = () => new Promise(resolve => setImmediate(resolve))
  , assert = require('assert')

function Queue(buffer = 64) {
  // in JS, we use a single buffer to hold data with input & output at the end
  // the parameter buffer can be the size fora a new buffer or a SharedArrayBuffer
  if (typeof buffer == 'number')
    buffer = new SharedArrayBuffer(Int32Array.BYTES_PER_ELEMENT * (buffer + 2))

  var data = new Int32Array(buffer)
    , length = data.length - 2
    , mask = length - 1
    , input = length
    , output = length + 1

  assert(length && !(length & (length - 1)), "Queue size must be a positive power of 2.");

  async function push(item) {
    assert(typeof item == 'number' && item !== 0)
    return new Promise(async resolve => {
      let i = 0;
      do {
        i = data[input];
        while (i - data[output] > mask) // if full, wait for space
          await sleep() // Atomics.wait(input, 0, i) 
      } while (data[i & mask] || (Atomics.compareExchange(data, input, i, i + 1) !== i))

      data[i & mask] = item
      return resolve(i)
    })
  }

  async function pop() {
    return new Promise(async resolve => {
      let o;
      do {
        o = data[output]
        while (o == data[input]) // if empty, wait for items
          await sleep() // Atomics.wait(output, 0, o)
      } while (!data[o & mask] || Atomics.compareExchange(data, output, o, o + 1) !== o)

      let r = data[o &= mask]
      data[o &= mask] = 0
      return resolve(r);
    })
  }
  return { buffer, push, pop }
}
module.exports = { Queue }
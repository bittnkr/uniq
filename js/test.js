const { Queue } = require('./uniq.js')
var { Worker, isMainThread, parentPort, workerData } = require('worker_threads');

var Threads = 4   // 2 producers & 2 consumers
  , Items = 10 * 1000 * 1000  // how many items will flow trough the queue 
  , Total = new SharedArrayBuffer(Int32Array.BYTES_PER_ELEMENT)

const Q = Queue(1024); // using the default 64 positions 

async function main() {
  var pairs = Threads / 2;

  console.log("Creating %d producers & %d consumers", pairs, pairs);
  console.log("to flow %d items trough the queue.", Items);

  var producers = [], consumers = [];

  for (var i = 0; i < pairs; i++) {
    producers[i] = new Worker('./js/producer.js', { workerData: { buffer: Q.buffer, Total, count: Items / pairs } })
    consumers[i] = new Worker('./js/consumer.js', { workerData: { buffer: Q.buffer, Total } })
  }

  // await Promise.all(consumers)
  // console.log("\nChecksum: %d (it must be zero)\n", total);
}

main()

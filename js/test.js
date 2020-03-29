const { Queue } = require('./uniq.js')
  , { Worker } = require('worker_threads');

(async function main() {
  const Q = Queue()
    , Threads = 2  // 2 producers & 2 consumers, totaling 4 threads
    , Items = 10 * 1000 * 1000  // how many items will flow trough the queue 
    , Total = new SharedArrayBuffer(Int32Array.BYTES_PER_ELEMENT)
    , producers = [], consumers = []

  console.log("Creating %d producers & %d consumers", Threads, Threads);
  console.log("to flow %d items trough the queue.", Items);

  for (var i = 0; i < Threads; i++) {
    producers[i] = new Worker('./js/producer.js', { workerData: { buffer: Q.buffer, Total, count: Items / Threads } })
    consumers[i] = new Worker('./js/consumer.js', { workerData: { buffer: Q.buffer, Total } })
  }

  // await Promise.all(consumers)
  // console.log("\nChecksum: %d (it must be zero)\n", total);
})()

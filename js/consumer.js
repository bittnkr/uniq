const { Queue } = require('./uniq.js')
const { parentPort, workerData } = require('worker_threads');
const { buffer, Total } = workerData
const Q = Queue(buffer)

function consumer() // the consumer thread, takes data from the queue
{
  var total = new Int32Array(Total)
  return new Promise(async resolve => {
    var v, sum = 0;

    while ((v = await Q.pop()) > 0)
      sum += v;

    Atomics.add(total, 0, -sum);
    console.log("Consumed:", sum);
    resolve(sum)
  })
}

consumer()
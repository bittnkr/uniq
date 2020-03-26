const { Queue } = require('./uniq.js')
const { parentPort, workerData } = require('worker_threads');
const { buffer, count, Total } = workerData
const Q = Queue(buffer)

function producer() // the producer thread pushes data into the queue
{
  var total = new Int32Array(Total)
  return new Promise(async resolve => {
    let sum = 0

    for (let i = 1; i <= count; i++) {
      await Q.push(1); // push the value 1 to the qeue
      sum += 1
    }

    Q.push(-1); // signalize termination with a -1
    Atomics.add(total, 0, sum);
    resolve(sum)
    console.log("Produced:", sum);
  })
}

producer()
importScripts('../uniq.js')

onmessage = async ({ data }) => {

  let Q = new Queue(data.buffer)
    , sum = 0

  console.log('uniq worker:', data.messageCount)

  for (var i = 0; i < data.messageCount; i++) {
    sum += await Q.pop();
    // console.log('pop sum:', sum)
  }
  postMessage(sum);
}
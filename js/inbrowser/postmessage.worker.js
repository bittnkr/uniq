var messageCount, sum = 0;

onmessage = ({ data }) => {
  if (!messageCount) messageCount = data
  else {
    messageCount--
    sum += data
    if (messageCount == 0)
      postMessage(sum);
  }
}
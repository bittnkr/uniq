import { Queue } from './queue';

console.log("Hello, world!");

// TEST(Queue)
export function testQueue(): void {
  const q = new Queue<i32>(64);
  const threads: Array<Thread> = new Array<Thread>();

  const produced = new AtomicI32(0);
  const producer = (n: i32): void => {
    let i = 0;
    while (++i <= n && q.push(i)) {
      produced.store(produced.load() + i);
    }
    q.push(-1);
  };

  const consumed = new AtomicI32(0);
  const consumer = (): void => {
    let v: i32;
    while (q.pop(v) && v != -1) {
      consumed.store(consumed.load() + v);
    }
  };

  const t = CpuTime();

  for (let i = 0; i < coreCount() / 2; i++) {
    threads.push(spawnThread(consumer));
    threads.push(spawnThread(() => producer(100000)));
  }

  for (let i = 0; i < threads.length; i++) threads[i].join();

  assert(produced.load() != 0);
  assert(produced.load() == consumed.load());
  // log("Queue:", double(CpuTime(t)));
}

// UniQ • Released under GPL 3 licence

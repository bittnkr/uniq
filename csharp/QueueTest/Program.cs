using System;
using System.Threading;
using System.Diagnostics;

namespace QueueTest
{
    public class Queue
    {
        protected int[] data;
        protected int head, tail;
        protected int mask;

        public int size { get; private set; }
        public int count { get { return tail - head; } }

        public static int RoundPowerOf2(int v)
        {
            v--;
            v |= v >> 1;
            v |= v >> 2;
            v |= v >> 4;
            v |= v >> 8;
            v |= v >> 16;
            return ++v;
        }

        public Queue(int size = 64)
        {
            this.size = RoundPowerOf2(size);
            mask = this.size - 1;
            tail = head = 0;
            Array.Resize(ref data, this.size);
        }
    
        public virtual void push(int item)
        {
            Debug.Assert(item != 0, "Queue.push(item): item can't be null");
            int t;
            do {
                t = tail;
                while (t - head == size) { };// { Thread.Sleep(0); }
            } while ( (data[t & mask]!=0) || (Interlocked.CompareExchange(ref tail, t+1, t) != t) );
            data[t & mask] = item;
        }

        public virtual int pop()
        {
            int h;
            do {
                h = head;
                while (h == tail) { };// { Thread.Sleep(0); }
            } while ( (data[h & mask]==0) || (Interlocked.CompareExchange(ref head, h+1, h) != h) );

            h &= mask;
            int r = data[h];
            data[h] = 0;
            return r;
        }
    };


    /*
     * FastQueue
    */
    public class FastQueue: Queue
    {
        public FastQueue(int size=64): base(size) {}

        public override void push(int item)
        {
            Debug.Assert(item != 0, "FastQueue.push(item): item can't be null");
            while (tail - head == size) { };// { Thread.Sleep(0); }
            data[tail & mask] = item;
            tail++;
        }

        public override int pop()
        {
            while (tail == head) { };//{ Thread.Sleep(0); }
            int r = data[head & mask];
            head++;
            return r;
        }
    }

    class Program
    {
        static void Main()
        {
            var q = new Queue();

            var producer = new Thread(
                delegate() {
                    int o, total = 0;
                    for (int i = 0; i < 10*1000*1000; i++) {
                        o = (i & 0xF) + 1;
                        q.push(o);
                        total += o;
                    }
                    q.push(-1); // end of the job

                    Console.Write("produced: {0:d}\n", total);
                }
            );

            var consumer = new Thread(delegate()
            {
                int o, total = 0;
                do{
                    o = q.pop();
                    total += o;
                } while (o > 0);
                Console.Write("consumed: {0:d}\n", total + 1);
            });

            DateTime ticks = DateTime.Now;
            producer.Start();
            consumer.Start();
 
            consumer.Join();
            Console.WriteLine("time: {0}", (DateTime.Now-ticks).ToString());
            Console.ReadKey();
        }
    }
}
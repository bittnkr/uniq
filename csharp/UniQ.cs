/**
* A Lock-free Queue
*/ 

using System;
using System.Threading;
using System.Diagnostics;

namespace UniQ
{
  public class Queue
  {
    protected int[] data;
    protected int head, tail;
    protected int mask;

    public int size { get; private set; }
    public int count { get { return tail - head; } }

    public Queue(int size = 8)
    {
      this.size = size;
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
        while (t - head == size){ };// { Thread.Sleep(0); };
      } while ( (data[t & mask]!=0) || (Interlocked.CompareExchange(ref tail, t+1, t) != t) );
      data[t & mask] = item;
    }

    public virtual int pop()
    {
      int h;
      do {
        h = head;
        while (h == tail){ };// { Thread.Sleep(0); };
      } while ( (data[h & mask]==0) || (Interlocked.CompareExchange(ref head, h+1, h) != h) );

      h &= mask;
      int r = data[h];
      data[h] = 0;
      return r;
    }
  };
};

// Part of uniQ library released under GNU 3.0 
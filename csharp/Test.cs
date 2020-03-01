/**
* A producer/consumer test for the UniQ library
* Part of uniQ library released under GNU 3.0 
*/ 

using System;
using System.Threading;
using System.Diagnostics;
using UniQ;

namespace QueueTest
{
  class Program
  {
    public static Queue Q = new Queue();
    public static long Total;

    public static void producer () 
    {
      long total = 0;
      for (int i = 1; i <= 10*1000*1000; i++) {
        Q.push(i);
        total+=i;
      }
      Q.push(-1); // end of the job
      Console.Write("produced: {0:d}\n", total);
      Interlocked.Add(ref Total, total);
    }

    public static void consumer () 
    {
      int o;
      long total = 0;
      do{
        total += (o = Q.pop());
      } while (o != -1);

      Interlocked.Add(ref Total, -total-1);
      Console.Write("consumed: {0:d}\n", total);
    }

    static void Main()
    {
      var CORES = 4; // In .NET the the best performance is achived with one Thread per core 

      Thread[] producers = new Thread[CORES], consumers = new Thread[CORES];
      DateTime ticks = DateTime.Now;

      Total = 0;
      for(int i=0; i < CORES/2; i++)
      {
        var p = new Thread(producer); p.Start();
        var c = new Thread(consumer); c.Start();

        producers[i] = p; 
        consumers[i] = c; 
      }

      for(int i=0; i < CORES/2; i++)
        consumers[i].Join();

      Console.WriteLine("Total:{0} (it must be zero)\n\ntime: {1}", Total, (DateTime.Now-ticks).ToString());
      // Console.ReadKey();
    }
  }
}
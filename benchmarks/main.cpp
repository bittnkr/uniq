#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>
#include <vector>

using namespace std;

int getTicks()
{
    timeval pairs;
    gettimeofday(&pairs, 0);
    return pairs.tv_sec*1000 + pairs.tv_usec/1000;
}

int Threads = 32;
int Items = 1024*1024*1024 / (Threads/2);
int Sum = 0;

class Queue
{
  protected:
    vector<int> buffer;
    // vector<BOOL> isfree; // use this for a templated queue.
    int size, mask;
    int in, out;

  public:
    Queue(int s=64): size(s) //:size(RoundPowerOf2(s))
    {
        buffer = vector<int>(size, 0);
        mask = size-1;
        in = out = 0;
    }

    void push(int item)
    {
        assert(item);

        int i;
        do
        {
            i = in;
            while (i-out == size) sched_yield(); // while full, wait for space
        }
        while (buffer[i & mask] || !__sync_bool_compare_and_swap(&in, i, i+1));

        buffer[i & mask] = item;
    }

    int pop()
    {
        int o;
        do
        {
            o = out;
            while (o == in) sched_yield(); // while empty, wait for item
        }
        while (!buffer[o & mask] || !__sync_bool_compare_and_swap(&out, o, o+1));

        int r = buffer[o &= mask];
        buffer[o] = 0;
        return r;
    }
};

//Queue Q(64);          // default
// Queue Q(1);        // stress (14x slower)
 Queue Q(1024); // benchmark (10,4% faster)

void* producer(void*)
{
    for(int i=0; i < Items; i++)
        Q.push(1);

    Q.push(-1); // signalize termination

    __sync_fetch_and_add(&Sum, Items);
    //printf ("Produced: %9d\n", Items); // comment for benchmarks
    return 0;
}

void* consumer(void*)
{
    int s = 0;

    while(Q.pop() > 0) // Pop items until get a -1
        s--;

    __sync_fetch_and_add(&Sum, s);
    //printf("Consumed: %9d\n", s); // comment for benchmarks
    return 0;
}

int main()
{
    int pairs = Threads/2;
    int ticks = getTicks();

    vector<pthread_t> producers(pairs);
    vector<pthread_t> consumers(pairs);

    for(int i=0; i < pairs; i++)
    {
        pthread_create(&producers[i], NULL, producer, NULL);
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    for(int i=0; i < pairs; i++)
        pthread_join(consumers[i], NULL);

    ticks = getTicks()-ticks;
    printf("Sum.....: %9d // it must be zero.\n", Sum);
    printf("Time....: %9d\n", ticks);
}
//
//    // start the threads
//    for (int i=0; i < P; i++){
//        producers[i] = new Producer(&q, &Sum, items);
//        consumers[i] = new Consumer(&q, &Sum);
//    };
//
//    // wait consumers termination
//    for (int i=0; i < C; i++){
//        consumers[i]->wait();
//    };
//


///*--------------------------------------
// * Utils
//*/
//
//int RoundPowerOf2(int v)
//{
//    v--;
//    v |= v >> 1;
//    v |= v >> 2;
//    v |= v >> 4;
//    v |= v >> 8;
//    v |= v >> 16;
//    return ++v;
//}
//
////inline bool CompareAndSet(int* destination, int currentValue, int newValue)
////{
////    return __sync_bool_compare_and_swap(destination, currentValue, newValue);
////}
//
//inline bool AtomicInc(int* destination, int i=1)
//{
//    return __sync_fetch_and_add(destination, i);
//}
//
//int getTicks()
//{
//    timeval pairs;
//    gettimeofday(&pairs, 0);
//    return pairs.tv_sec*1000 + pairs.tv_usec/1000;
//}
//
//void setStackSize(int size)
//{
//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_attr_setstacksize (&attr, size);
//    pthread_attr_destroy(&attr);
//}
//
//inline int AtomicInt( int * variable, int value ){
//        asm volatile(
//                                "lock; xaddl %%eax, %2;"
//                                :"=a" (value)                   //Output
//                               : "a" (value), "m" (*variable)  //Input
//                               :"memory" );
//        return value;
// }
//
///*--------------------------------------
// * Queue
//*/
//
//class Queue
//{
//  protected:
//    vector<int> buffer;
//    int size;
//    int in, out, mask;
//    int count() { return in - out; }
//
//  public:
//    Queue(int _size=64): size( RoundPowerOf2(_size) )
//    {
//        buffer = vector<int>(size, 0);
//        in = out = 0;
//        mask = size - 1;
//    }
//
//    void push(int item)
//    {
//        int i;
//        assert( item != 0 );
//
//        do{
//            i = in;
//            while ( i-out == size )
//                sched_yield();
//        }
//        while ( buffer[i & mask]  ||  !__sync_bool_compare_and_swap(&in, i, i+1) );
//
//        buffer[i & mask] = item;
//    }
//
//    virtual int pop()
//    {
//        int i;
//
//        do{
//            i = out;
//            while ( i == in )
//                sched_yield();
//        }
//        while ( !buffer[i & mask]  ||  !__sync_bool_compare_and_swap(&out, i, i+1) );
//
//        int r = buffer[i &= mask];
//        buffer[i] = 0;
//        return r;
//    }
//
//};
//
//class Thread
//{
//  private:
//    pthread_t thread;
//
//    static void* EntryPoint(void * me)
//    {
//        ((Thread *)me)->Run();
//
//        pthread_exit(NULL);
//        return NULL;
//    }
//
//  protected:
//    virtual void Run() = 0; // to be overriden;
//
//  public:
//    Thread() { pthread_create(&thread, NULL, EntryPoint, this); }
//    //virtual ~Thread() { }
//
//    void wait()
//    {
//        pthread_join(thread, NULL);
//    }
//};
//
//class Producer: public Thread
//{
//  private:
//    Queue* q;
//    int *Sum;
//    int items;
//
//  protected:
//    virtual void Run()
//    {
//        for(int i = 0; i < items; i++) {
//            q->push(1);
//        }
//        q->push(-1); // signalize the end of the job
//
//        AtomicInc(Sum, items);
//        //printf("produced: %9d (%9d)\n", items, *Sum);
//    }
//
//  public:
//    Producer(Queue* q_, int* Sum_, int items_):
//        Thread(), q(q_), Sum(Sum_), items(items_) { }
//};
//
//class Consumer: public Thread
//{
//  private:
//    Queue* q;
//    int *Sum;
//
//  protected:
//    virtual void Run()
//    {
//        int item, sum = 0;
//
//        do{
//            item = q->pop();
//            sum += item;
//        } while ( item > 0 );
//
//        AtomicInc(Sum, -sum-1);
//        //printf("consumed: %9d (%9d)\n", -sum, *Sum);
//    }
//  public:
//    Consumer(Queue* q_, int* Sum_):
//        Thread(), q(q_), Sum(Sum_) { }
//
//};
//
//int k = 1024;
//
//void Test(int pairs=1, int items=7, int size=4)
//{
//    int Sum=0; int C=pairs; int P=pairs;
//
//    Queue q(size);
//
//    Producer* producers[ P ];
//    Consumer* consumers[ C ];
//
//    int ticks = getTicks();
//
//    // start the threads
//    for (int i=0; i < P; i++){
//        producers[i] = new Producer(&q, &Sum, items);
//        consumers[i] = new Consumer(&q, &Sum);
//    };
//
//    // wait consumers termination
//    for (int i=0; i < C; i++){
//        consumers[i]->wait();
//    };
//
//    ticks = getTicks()-ticks;
//    //printf("-------------------------------\n");
//    printf("q.size(): %9d \n", size);
//    printf("Sum.....: %9d \n", Sum);
//
//    printf("items...: %.0f M \n", (double) P*items/1000000);
//    printf("rounds/s: %.1f M \n", (double) P*items/ticks / 1000);
//    printf("time....: %.2f s \n", (double) ticks/1000);
//
//    // realease the objects
//    for (int i=0; i < P; i++){
//        delete producers[i];
//        delete consumers[i];
//    };
//
//};
//
//int main()
//{
//    setStackSize(128*k);
//
//    // limites de performance de threads.
//    // ilustra bem uma caracteristica do algoritmo que a velocidade é independente do numero de threads,
//    // contando que o buffer seja ajustado de maneira apropriada.
//
//    Test(  1, 50*1000*1000, 64  ); // 15.6
//    Test(  1, 50*1000*1000, 64*k); // 16.6
//    Test(  1, 50*1000*1000, k*k ); // 17.3
//
//    Test(500, 100*1000, 64  );  // 15.6
//    Test(500, 100*1000, 64*k);  // 16.3
//    Test(500, 100*1000, k*k );  // 16.8
//}

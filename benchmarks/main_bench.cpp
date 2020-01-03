#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>
#include <vector>

using namespace std;

/*--------------------------------------
 * Utils
*/

int RoundPowerOf2(int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return ++v;
}

//inline bool CompareAndSet(int* destination, int currentValue, int newValue)
//{
//    return __sync_bool_compare_and_swap(destination, currentValue, newValue);
//}

inline bool AtomicInc(int* destination, int i=1)
{
    return __sync_fetch_and_add(destination, i);
}

int getTicks()
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec*1000 + t.tv_usec/1000;
}

void setStackSize(int size)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize (&attr, size);
    pthread_attr_destroy(&attr);
}

inline int AtomicInt( int * variable, int value ){
        asm volatile(
                                "lock; xaddl %%eax, %2;"
                                :"=a" (value)                   //Output
                               : "a" (value), "m" (*variable)  //Input
                               :"memory" );
        return value;
 }

/*--------------------------------------
 * Queue
*/

class Queue
{
  protected:
    vector<int> buffer;
    int size;
    int tail, head, mask;
    int count() { return tail - head; }

  public:
    Queue(int _size=64): size( RoundPowerOf2(_size) )
    {
        buffer = vector<int>(size, 0);
        tail = head = 0;
        mask = size - 1;
    }

    void push(int item)
    {
        int i;
        assert( item != 0 );

        do{
            i = tail;
            while ( i-head == size )
                sched_yield();
        }
        while ( buffer[i & mask]  ||  !__sync_bool_compare_and_swap(&tail, i, i+1) );

        buffer[i & mask] = item;
    }

    virtual int pop()
    {
        int i;

        do{
            i = head;
            while ( i == tail )
                sched_yield();
        }
        while ( !buffer[i & mask]  ||  !__sync_bool_compare_and_swap(&head, i, i+1) );

        int r = buffer[i &= mask];
        buffer[i] = 0;
        return r;
    }

};

class Thread
{
  private:
    pthread_t thread;

    static void* EntryPoint(void * me)
    {
        ((Thread *)me)->Run();

        pthread_exit(NULL);
        return NULL;
    }

  protected:
    virtual void Run() = 0; // to be overriden;

  public:
    Thread() { pthread_create(&thread, NULL, EntryPoint, this); }
    //virtual ~Thread() { }

    void wait()
    {
        pthread_join(thread, NULL);
    }
};

class Producer: public Thread
{
  private:
    Queue* q;
    int *Sum;
    int items;

  protected:
    virtual void Run()
    {
        for(int i = 0; i < items; i++) {
            q->push(1);
        }
        q->push(-1); // signalize the end of the job

        AtomicInc(Sum, items);
        //printf("produced: %9d (%9d)\n", items, *Sum);
    }

  public:
    Producer(Queue* q_, int* Sum_, int items_):
        Thread(), q(q_), Sum(Sum_), items(items_) { }
};

class Consumer: public Thread
{
  private:
    Queue* q;
    int *Sum;

  protected:
    virtual void Run()
    {
        int item, sum = 0;

        do{
            item = q->pop();
            sum += item;
        } while ( item > 0 );

        AtomicInc(Sum, -sum-1);
        //printf("consumed: %9d (%9d)\n", -sum, *Sum);
    }
  public:
    Consumer(Queue* q_, int* Sum_):
        Thread(), q(q_), Sum(Sum_) { }

};

int k = 1024;

void Test(int pairs=1, int items=7, int size=4)
{
    int Sum=0; int C=pairs; int P=pairs;

    Queue q(size);

    Producer* producers[ P ];
    Consumer* consumers[ C ];

    int ticks = getTicks();

    // start the threads
    for (int i=0; i < P; i++){
        producers[i] = new Producer(&q, &Sum, items);
        consumers[i] = new Consumer(&q, &Sum);
    };

    // wait consumers termination
    for (int i=0; i < C; i++){
        consumers[i]->wait();
    };

    ticks = getTicks()-ticks;
    //printf("-------------------------------\n");
    printf("q.size(): %9d \n", size);
    printf("Sum.....: %9d \n", Sum);

    printf("items...: %.0f M \n", (double) P*items/1000000);
    printf("rounds/s: %.1f M \n", (double) P*items/ticks / 1000);
    printf("time....: %.2f s \n", (double) ticks/1000);

    // realease the objects
    for (int i=0; i < P; i++){
        delete producers[i];
        delete consumers[i];
    };

};

int main()
{
    setStackSize(128*k);

    // limites de performance de threads.
    // ilustra bem uma caracteristica do algoritmo que a velocidade é independente do numero de threads,
    // contando que o buffer seja ajustado de maneira apropriada.

    Test(  1, 50*1000*1000, 64  ); // 15.6
    Test(  1, 50*1000*1000, 64*k); // 16.6
    Test(  1, 50*1000*1000, k*k ); // 17.3

    Test(500, 100*1000, 64  );  // 15.6
    Test(500, 100*1000, 64*k);  // 16.3
    Test(500, 100*1000, k*k );  // 16.8
}

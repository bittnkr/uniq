#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <vector>

class Queue
{
  protected:
    vector<int> buffer;
    int size;
    int tail, head, mask;
    int count() { return tail-head; }

  public:
    Queue(int s=64): size(RoundPowerOf2(s))
    {
        buffer = vector<int>(size, 0);
        tail = head = 0;
        mask = size-1;
    }
    void push(int item)
    {
        assert(item != 0);

        int i;
        do
        {
            i = tail;
            while (i-head == size) sched_yield();
        }
        while (buffer[i & mask] || !__sync_bool_compare_and_swap(&tail, i, i+1));

        buffer[i & mask] = item;
    }
    virtual int pop()
    {
        int i;
        do
        {
            i = head;
            while (i == tail) sched_yield();
        }
        while (!buffer[i & mask] || !__sync_bool_compare_and_swap(&head, i, i+1));

        int r = buffer[i &= mask];
        buffer[i] = 0;
        return r;
    }
};

int Threads = 512;
int Items = 1000*1000;
int Sum = 0;

// Queue Q(1);           // stress
Queue Q(64);             // default
// Queue Q(1024*1024);   // benchmark

void* producer(void*)
{
    for(int i=0; i<Items; i++)
        Q.push(1);

    Q.push(-1); // signalize termination

    __sync_fetch_and_add(&Sum, Items);
    printf ("Produced: %9d", Items);
}

void* consumer(void*)
{
    int sum = 0;

    while(Q.pop() > 0)
        sum++;

    __sync_fetch_and_add(&Sum, -sum);
    printf("Consumed: %9d\n", -sum);
}

int main()
{
    int t = Threads/2;

    vector<pthread_t> producers(t);
    vector<pthread_t> consumers(t);

    for(int i=0; i<t; i++)
    {
        pthread_create(&producers[i], NULL, producer, NULL);
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    for(int i=0; i<t; i++)
        pthread_join(consumers[i], NULL);

    printf("Sum.....: %9d \n\n", Sum);
}

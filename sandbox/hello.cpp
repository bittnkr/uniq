using namespace std;
#include <stdio.h>
#include <pthread.h>
#include <vector>


inline bool CompareAndSet(int* destination, int currentValue, int newValue)
{
    return __sync_bool_compare_and_swap(destination, currentValue, newValue);
}

class Queue
{
    private:
        int _size, _head, _tail, mask;
        vector<int> buffer;
        vector<int> isfree;

    protected:
        int tail()  { while ( full()) sched_yield(); return _tail; }
        int head()  { while (empty()) sched_yield(); return _head; }

    public:
        Queue(int size=32){
            _size  = size;
            _tail  = _head = 0;
            mask   = _size - 1;
            buffer = vector<int>(_size, 0);
            isfree = vector<int>(_size, 1);
        }
        int size()  { return _size; }
        int count() { return _tail - _head; }
        int empty() { return _tail == _head; }
        int full()  { return _size == count(); }

        void push(int item) {
            int t;

            do t = tail();
            while ( !isfree[t & mask] || !CompareAndSet(&_tail, t, t+1) );

            isfree[t &= mask] = 0;
            buffer[t] = item;
        }
        
        virtual int pop() {
            int h;

            do h = head();
            while ( isfree[h & mask] || !CompareAndSet(&_head, h, h+1) );

            int r = buffer[h &= mask];
            isfree[h] = 1;
            return r;
        }
};

Queue Q;

void* producer(void*)
{
    int total = 10*1000*1000;

    for(int i = total; i; i--) Q.push(1);
    Q.push(-1); // end of the job

    printf ("produced: +%9d\n", total);
}

void* consumer(void*)
{
    int total = 0;

    while ( Q.pop() ) total++;

    printf("consumed: -%9d \n", total);
}


int main()
{
    int pairs = 8;
    pthread_t p[pairs], c[pairs];

    for (int i=0; i < pairs; i++){
        pthread_create(&p[i], NULL, &producer, NULL);
        pthread_create(&c[i], NULL, &consumer, NULL);
    };

    for (int i=0; i < pairs; i++){
        pthread_join(c[i], NULL);
    };

    return 0;
}

// Part of uniQ library released under GNU 3.0 
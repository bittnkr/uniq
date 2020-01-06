## States

At any moment, this queue has only 3 possible states: **empty, full & flowing**.

In the **empty** state, the queue has no elements. It can accept new elements, but if some actor tries to remove an element, it will be put on hold, and notified when new element arrive. 

When the queue is **full**, it happens the oposite of the previous case: the queue can delivery data, but not accept new ones. If an actor try to input data, it must be put on hold until one element was consumed and the space released.

In the **flowing** state, the queue has space and can receive and deliver elements. (Not really a new state, but a negative of the previous ones.)


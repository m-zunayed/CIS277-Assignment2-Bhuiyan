# CIS-277 Assignment 2: Network Packet Arrival Queue

## Student

Md Zunayed Bhuiyan

## Design

The Queue is implemented using a fixed-size circular array. It keeps track of the front position, the number of items currently stored, and the total capacity.  

For enqueue, the next position is calculated with (front_ + count_) % capacity_. 

For dequeue, the front moves using (front_ + 1) % capacity_. This lets the Queue wrap back to the beginning of the array without shifting all the waiting items.  

The Queue stores packet descriptors, which contain the packet ID, block pointer, and length. The MemoryPool owns the actual memory blocks used to store the packet bytes.

## How to Compile

```bash
g++ -std=c++23 -Wall -Wextra -pedantic main.cpp PacketQueue.cpp MemoryPool.cpp -o packet_queue
```

## How to Run

```bash
./packet_queue
```

## Tests and Results

| Test | Expected | Actual |
|---|---|---|
| Integer Queue full state | After adding 10, 20, and 30 to capacity 3, full() should be true | Queue full: yes |
| front() without removal | front() should return 10 without changing the size | Front value: 10; size stayed 3 |
| Integer Queue wraparound | After removing 10 and adding 40, remaining order should be 20, 30, 40 | 20, 30, 40 |
| Empty Queue after draining | empty() should be true after all values are removed | Queue empty: yes |
| String Queue | After removing alpha and adding gamma, remaining order should be beta, gamma | beta, gamma |
| Full PacketQueue | A fourth receive should fail and the state should stay unchanged | Receive failed; pending stayed 3 and available blocks stayed 0 |
| Process one packet | Packet 1 should be processed first and one block should become available | Packet 1 processed; pending became 2 and available blocks became 1 |
| Block reuse | After one block is released, packet 4 should be accepted | Packet 4 accepted; pending became 3 and available blocks became 0 |
| Remaining packet order | Remaining packets should be processed in FIFO order 2, 3, 4 | Processed in order 2, 3, 4 |
| Binary data | Bytes including 0x00 and 0xFF should be copied exactly | Packet 1 returned 0x00 0xFF 0x12 0x34 |
| Length 1 packet | Should be accepted | Accepted |
| Exact block-size packet | Should be accepted | Accepted |
| Oversized packet | Should be rejected | Rejected |
| Null pointer packet | Should be rejected | Rejected |
| Zero length packet | Should be rejected | Rejected |
| processNext() on empty queue | Should return false without changing the output values | Returned false; ID unchanged and bytes unchanged |
| Final pool state | After all packets are processed, all 3 blocks should be available again | Pending 0; available blocks 3 |

## Analysis Questions

1. Why must packet service be FIFO even though free-block reuse is LIFO?

Packet service should be FIFO because packets need to be processed in the same order they arrive. The free blocks can still be reused with LIFO because that only affects which memory block is used, not the order the packets are processed.

2. How does your circular array wrap without shifting all waiting packets?

The Queue uses modulo with the capacity to wrap the index back to the beginning of the array. This lets it reuse empty positions without shifting the other elements.

3. What happens at queue overflow and underflow, and why must those attempts leave state unchanged?

If the Queue is full, enqueue returns false and does not add anything. If the Queue is empty, dequeue or front returns false and does not change the output value. This is important so a failed operation does not accidentally change the Queue or the caller’s data.

4. What are the time complexities of queue operations and of receive/processNext when the packet has length bytes?

The Queue operations enqueue, dequeue, front, empty, full, size, and capacity are O(1). receive() and processNext() are O(length) because they have to copy the packet bytes.

5. Why must processNext copy bytes out before returning the block to the pool?

processNext has to copy the packet bytes before releasing the block because the MemoryPool could reuse that block for another packet. If the block was released first, the old packet data could be overwritten before it is copied out.

## AI Use

I used ChatGPT to help me understand the assignment requirements, check my code, and review my testing. I still compiled and ran the program myself and checked the results using the assignment validator.

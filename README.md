# Programming-Assignment-1: Producer-Consumer

# Overview
My name is Alicia Brown. For assignment 1 for Operating Systems, we had to create a producer that produces a maximum of 2 items and a consumer that consumes those 2 items.
This program utilizes shared memory between the 2 programs and uses a critical section to ensure data consistency as well as prevent race conditions.

# How it Works
In the header file we have 3 semaphore: empty, full, and a mutex. Empty and full are used for buffer access to the producer and consumer, while the mutex semaphore protects the critical section, thus ensuring that one program is acessing the buffer. Each program iterates the producer-consumer interaction twice.

Producer
- waits for a slot to be available in the buffer
- locks the critical section
- produces an item
- unlocks the critical section
- signals that a slot has been filled

Consumer
- waits until there is a filled slot
- locks the critical section
- consumes an item
- unlocks the critical section
- signals that the slot is now empty

# Example Output
produced: 0

produced: 1

consumed: 0

consumed: 1

# Compliation Commands
g++ producer.cpp -o producer

g++ consumer.cpp -o consumer

./producer & ./consumer &

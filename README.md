Producer-Consumer problem in C++

Overview:
This project contains C++ programs that implement the producer-consumer problem using threads, semaphores, and shared memory. The producer generates items and places them on a shared table, while the consumer picks up items from the table. The programs are designed to be run concurrently and communicate with each other via semaphores and shared memory using a shared key.

Files:
producer.cpp: The producer program, which generates items and adds them to the shared table.
consumer.cpp: The consumer program, which consumes items from the shared table.

Requirements:
A C++ compiler (e.g., g++)
POSIX threads library (for -pthread)
POSIX real-time library (for -lrt)

How to Compile:
g++ producer.cpp -pthread -lrt -o producer
g++ consumer.cpp -pthread -lrt -o consumer

Running the Program:
./producer
./consumer

Configuration:
MAX_ITEMS - variable that determines how many items the table can hold
SHARED_MEMORY_KEY - key used by producer and consumer to access the same table
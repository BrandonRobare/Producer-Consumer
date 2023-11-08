#include <iostream>
#include <thread>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

const int MAX_ITEMS = 2; // Maximum items the table can hold
const int SHARED_MEMORY_KEY = 1234; // Shared memory key

struct SharedData {
    int table[MAX_ITEMS];
    int itemCount;
};

sem_t emptySlots; // Semaphore to track available slots on the table
sem_t filledSlots; // Semaphore to track filled slots on the table

// Function to consume an item from the table
void consumer(SharedData* data) {
    while (true) {
        sem_wait(&filledSlots); // Wait for a filled slot
        // (simulated by sleep for demonstration purposes)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Consume an item (e.g., process the item)
        data->table[data->itemCount - 1] = 0; // empty slot
        data->itemCount--;

        std::cout << "Consumed an item. Items on the table: " << data->itemCount << std::endl;
        sem_post(&emptySlots); // slot is empty
    }
}

int main() {
    int shmid;
    shmid = shmget(SHARED_MEMORY_KEY, sizeof(SharedData), 0666);

    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    SharedData* sharedData = static_cast<SharedData*>(shmat(shmid, (void*)0, 0));

    sem_init(&emptySlots, 1, MAX_ITEMS);
    sem_init(&filledSlots, 1, 0);

    std::thread consumerThread(consumer, sharedData);

    // Keep the consumer program running
    consumerThread.join();

    shmdt(sharedData);

    sem_destroy(&emptySlots);
    sem_destroy(&filledSlots);

    return 0;
}

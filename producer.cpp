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

// Function to produce an item and add it to the table
void producer(SharedData* data) {
    while (true) {
        // Produce an item (e.g., generate an item)

        sem_wait(&emptySlots); // Wait for an empty slot
        // Lock the critical section for adding an item to the table
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Add the item to the table
        data->table[data->itemCount] = 1; //1 is placeholder
        data->itemCount++;

        std::cout << "Produced an item. Items on the table: " << data->itemCount << std::endl;
        sem_post(&filledSlots); // Signal that a slot is filled
    }
}

int main() {
    int shmid;
    shmid = shmget(SHARED_MEMORY_KEY, sizeof(SharedData), 0666 | IPC_CREAT);

    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    SharedData* sharedData = static_cast<SharedData*>(shmat(shmid, (void*)0, 0));
    sharedData->itemCount = 0;

    sem_init(&emptySlots, 1, MAX_ITEMS);
    sem_init(&filledSlots, 1, 0);

    std::thread producerThread(producer, sharedData);

    // Keep the producer program running
    producerThread.join();

    shmdt(sharedData);
    shmctl(shmid, IPC_RMID, nullptr);
    
    sem_destroy(&emptySlots);
    sem_destroy(&filledSlots);

    return 0;
}

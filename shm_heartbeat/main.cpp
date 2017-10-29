#include <iostream>
#include <time.h>

#include <chrono>   // C++ 11, std::chrono::seconds
#include <thread>   // std::this_thread::sleep_for

#include "sharedmemoryofheartbeats.h"

using namespace std;

void test()
{
    // test create
    std::cout << "Test: create shared memory\n";
    SharedMemoryOfHeartbeats mySharedMemory(1);
    std::cout << "maxNumComponents = " << mySharedMemory.maxNumComponents() << "\n";
    std::cout << "sizeof(Heartbeat) = " << sizeof(Heartbeat) << "\n";


    std::cout << "Test: print shared memory\n";
    std::cout << mySharedMemory;


    // test add
    std::cout << "Test: add two components\n";
    int index_comp_0;
    mySharedMemory.addComponentToSharedMemory("testComp0", index_comp_0);
    int index_comp_1;
    mySharedMemory.addComponentToSharedMemory("testComp1", index_comp_1);

    std::cout << "Test: print shared memory\n";
    std::cout << mySharedMemory;

    // test refresh heartbeat time stamp
    std::cout << "Sleeping for 3 seconds, please wait...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));  // C++ 11

    mySharedMemory.refreshHeartbeatTime(0);
    mySharedMemory.refreshHeartbeatTime(1);

    std::cout << mySharedMemory;


    // test destroy
    std::cout << "Test: destroy shared memory\n";
    mySharedMemory.destroySharedMemory();

}


int main()
{
    cout << "Test shared memory of heartbeat\n\n" << endl;

    test();

    return 0;
}

#include "componentinterface.h"



const int ComponentInterface::HEARTBEAT_INTERVEL = 5;

ComponentInterface::~ComponentInterface()
{

}

void ComponentInterface::updateHeartbeatTimestampPeriodically()
{
    // do this in the child thread
    while(true) {
        d_sharedMemory.refreshHeartbeatTime(d_index_of_shm_unit);

        std::cout << "DEBUG: shared memory is:\n" << d_sharedMemory;

        // then sleep for HEARTBEAT_INTERVAL seconds
        std::this_thread::sleep_for(std::chrono::seconds(HEARTBEAT_INTERVEL));  // C++ 11
    }
}

bool ComponentInterface::startHeartbeat()
{
    if(!d_componentIsInSharedMemory) {
        std::cout << "ERROR: component is not in shared memory, cannot start heartbeat !\n";
        return false;
    }

    std::thread heartbeatThread(updateHeartbeatTimestampPeriodically, this);
    heartbeatThread.detach();

    return true;
}



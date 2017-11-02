#include "componentinterface.h"



const int ComponentInterface::HEARTBEAT_INTERVEL = 5;


void ComponentInterface::updateHeartbeatTimestampPeriodically()
{
    // do this in the child thread
    std::cout << "INFO: *** start heartbeat thread ***\n";

    while(true) {
        d_sharedMemory.refreshHeartbeatTime(d_index_of_shm_unit);

        //std::cout << "DEBUG: shared memory is:\n" << d_sharedMemory;

        // then sleep for HEARTBEAT_INTERVAL seconds
        std::this_thread::sleep_for(std::chrono::seconds(HEARTBEAT_INTERVEL));  // C++ 11

        if(d_stopHeartbeatThread) {
            break;
        }
    }

    std::cout << "INFO: *** exit heartbeat thread ***\n";
}

bool ComponentInterface::startHeartbeat()
{
    if(!d_componentIsInSharedMemory) {
        std::cout << "ERROR: component is not in shared memory, cannot start heartbeat !\n";
        return false;
    }

    d_stopHeartbeatThread = false;

    std::thread heartbeatThread(updateHeartbeatTimestampPeriodically, this);
    heartbeatThread.detach();

    return true;
}

bool ComponentInterface::stopHeartbeat()
{
    // set the flag, and when heartbeat thread checks the flag, should terminate thread
    d_stopHeartbeatThread = true;
    return true;
}



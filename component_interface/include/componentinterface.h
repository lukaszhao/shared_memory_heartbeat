#ifndef COMPONENTINTERFACE_H
#define COMPONENTINTERFACE_H

#include <globalconstants.h>

#include <sharedmemoryofheartbeats.h>

#include <string>
#include <iostream>

#include <thread>
#include <chrono>




class ComponentInterface
{
    public:
        ComponentInterface(const std::string& componentName):
            d_componentName(componentName),
            d_sharedMemory(),  // calling constructor with default maxNumComponents
            d_componentIsInSharedMemory(false),
            d_stopHeartbeatThread(false)
        {
            bool success = d_sharedMemory.addComponentToSharedMemory(componentName, d_index_of_shm_unit);
                           // note: addComponentToSharedMemory function does check if the component
                           // already exists in shared memory
            if(!success) {
                std::cout << "ERROR: Failed to add component [" << componentName <<"] to shared memory !\n";
                d_componentIsInSharedMemory = false;
            } else {
                d_componentIsInSharedMemory = true;
            }
        }

        ~ComponentInterface()
        {
            stopHeartbeat();  // to make sure that heartbeat thread exists before this object can be destroyed
        }

    private:
        int                               d_index_of_shm_unit;
        std::string                       d_componentName;
        SharedMemoryOfHeartbeats          d_sharedMemory;
        bool                              d_componentIsInSharedMemory;

        bool                              d_stopHeartbeatThread;



    public:

        void updateHeartbeatTimestampPeriodically();

        bool startHeartbeat();
            // start a child thread which updates heartbeat time stamp every HEARTBEAT_INTERVAL
            // return true if succeeds, o.w. false

        bool stopHeartbeat();


};

#endif // COMPONENTINTERFACE_H

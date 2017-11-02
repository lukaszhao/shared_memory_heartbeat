#ifndef WATCHDOGINTERFACE_H
#define WATCHDOGINTERFACE_H

#include <sharedmemoryofheartbeats.h>

#include <string>
#include <iostream>

#include <thread>
#include <chrono>


class WatchdogInterface
{
    public:
        WatchdogInterface():
            d_sharedMemory(),
            d_stopMonitoring(false)
        {

        }

        ~WatchdogInterface() {}


    private:
        SharedMemoryOfHeartbeats          d_sharedMemory;

        bool                              d_stopMonitoring;

        const static int                  HEARTBEAT_INTERVEL;

        void static componentHearbeatStoppedCallback(const std::string& componentName, const time_t& lastHeartbeatTime);
            // when monitor thread discovers a component heartbeat stops
            // this function is called, and componentName and lastHeartbeatTime
            // are returned through argument refs



    public:

        void monitorAllComponentsPeriodically(void (*cb)(const std::string& componentName, const time_t& lastHeartbeatTime)) const;

        void startMonitoring();

        void stopMonitoring();

};

#endif // WATCHDOGINTERFACE_H

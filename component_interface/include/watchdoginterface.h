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
            d_sharedMemory()
        {

        }

        ~WatchdogInterface() {}


    private:
        SharedMemoryOfHeartbeats          d_sharedMemory;

        const static int                  HEARTBEAT_INTERVEL;

        void componentHearbeatStoppedCallback(const std::string& componentName, const time_t& lastHeartbeatTime);
            // when monitor thread discovers a component heartbeat stops
            // this function is called, and componentName and lastHeartbeatTime
            // are returned through argument refs



    public:

        void monitorAllComponentsPeriodically();

        void startMonitoring();
};

#endif // WATCHDOGINTERFACE_H

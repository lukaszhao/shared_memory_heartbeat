#include "watchdoginterface.h"

#include <stdlib.h>     // to use abs(int x)


const int WatchdogInterface::HEARTBEAT_INTERVEL = 5;




void WatchdogInterface::componentHearbeatStoppedCallback(const std::string& componentName,
                                                         const time_t& lastHeartbeatTime)
{
    std::cout << "\n==========================================================================\n"
              << "ALERT: component[" << componentName << "] stopped heartbeat!\n"
              << "       last heartbeat time = " << lastHeartbeatTime
              << "\n==========================================================================\n\n";
}


void WatchdogInterface::monitorAllComponentsPeriodically(void (*cb)(const std::string& componentName, const time_t& lastHeartbeatTime)) const
{
    // start a child thread with this function
    //std::cout << "DEBUG: WatchdogInterface::monitorAllComponentsPeriodically() called\n";
    //std::cout << "DEBUG: shared memory is:" << d_sharedMemory;
    std::cout << "INFO: d_sharedMemory.vectorSize() = " << d_sharedMemory.vectorSize() << "\n";

    time_t timeNow;

    while(true) {
        std::cout << "\n\n\n++++++++++++++++++++++++++++++++++++++++++\n";
        for(int i = 0; i < d_sharedMemory.vectorSize(); ++i) {

            //std::cout << "DEBUG: d_sharedMemory.vectorSize() = " << d_sharedMemory.vectorSize() << "\n";

            timeNow = time(nullptr);  // get time now for before comparing each component's lastHeartbeatTime
                                      // maybe it is not necessary, maybe only get now once before entering for loop is enough
            //std::cout << "DEBUG: timeNow = " << timeNow << "\n";

            time_t compLastHeartbeatTime;
            d_sharedMemory.readComponentLastHeartbeatTime(i, compLastHeartbeatTime);
            //std::cout << "DEBUG: compLastHeartbeatTime = " << compLastHeartbeatTime << "\n";

            std::string compName;
            d_sharedMemory.readComponentName(i, compName);
            //std::cout << "DEBUG: compName = " << compName << "\n";

            // print info
            std::cout << i << ": " << compName << "............" << compLastHeartbeatTime << "\n";

            if(abs(static_cast<int>(timeNow - compLastHeartbeatTime)) > HEARTBEAT_INTERVEL * 2) {
                // component stooped heartbeat, callback
                cb(compName, compLastHeartbeatTime);
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(HEARTBEAT_INTERVEL));

        if(d_stopMonitoring) { break; }
    }
}


void WatchdogInterface::startMonitoring()
{
    d_stopMonitoring = false;
    std::thread monitorThread(&(WatchdogInterface::monitorAllComponentsPeriodically),
                              this,
                              WatchdogInterface::componentHearbeatStoppedCallback);
    monitorThread.detach();
}

void WatchdogInterface::stopMonitoring()
{
    d_stopMonitoring = true;
}


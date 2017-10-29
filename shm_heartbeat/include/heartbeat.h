#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <time.h>
#include <string>


class Heartbeat
{
    public:
        Heartbeat();
        virtual ~Heartbeat();

    private:
        time_t                 d_lastHeartbeatTime;
        std::string            d_componentName;

    public:
        // getters

        const time_t& lastHeartbeatTime() {
            return d_lastHeartbeatTime;
        }

        const std::string& componentName() {
            return d_componentName;
        }


        // setters
        void setLastHeartbeatTimeAsNow() {
            d_lastHeartbeatTime = time(nullptr);
        }

        void setComponentName(const std::string& componentName) {
            d_componentName = componentName;
        }


        // copy assignment
        Heartbeat& operator=(const Heartbeat& other) {
            if(this != &other) { // self-check
                d_lastHeartbeatTime = other.d_lastHeartbeatTime;
                d_componentName = other.d_componentName;
            }
            return *this;
        }
};

#endif // HEARTBEAT_H

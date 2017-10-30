#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <time.h>
#include <string>
#include <iostream>


class Heartbeat
{
public:
    Heartbeat();
    ~Heartbeat();

private:
    time_t                 d_lastHeartbeatTime;
    std::string            d_componentName;

public:
    // getters

    const time_t& lastHeartbeatTime()
    {
        return d_lastHeartbeatTime;
    }

    const std::string& componentName()
    {
        std::cout << "DEBUG: Heartbeat::componentName() called, d_componentName = " << d_componentName << "\n";
        return d_componentName;
    }


    // setters
    void setLastHeartbeatTimeAsNow()
    {
        d_lastHeartbeatTime = time(nullptr);
    }

    void setComponentName(const std::string& componentName)
    {
        std::cout << "DEBUG: Heartbeat::setComponentName called\n";
        d_componentName = componentName;
        std::cout << "DEBUG: after setComponentName, d_componentName = " << d_componentName << "\n";
    }


    // copy assignment
    Heartbeat& operator=(const Heartbeat& other)
    {
        if(this != &other)   // self-check
        {
            d_lastHeartbeatTime = other.d_lastHeartbeatTime;
            d_componentName = other.d_componentName;
        }
        return *this;
    }
};

#endif // HEARTBEAT_H

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
    char                   d_componentName[50];

public:
    // getters

    const time_t& lastHeartbeatTime()
    {
        return d_lastHeartbeatTime;
    }

    std::string componentName()
    {
        //std::cout << "DEBUG: Heartbeat::componentName() called, d_componentName = " << d_componentName << "\n";
        std::string str(d_componentName);
        return str;
    }


    // setters
    void setLastHeartbeatTimeAsNow()
    {
        d_lastHeartbeatTime = time(nullptr);
    }

    void setComponentName(const std::string& componentName)
    {
        //std::cout << "DEBUG: Heartbeat::setComponentName called\n";
        size_t strMaxIndex = 48;
        if(strMaxIndex > componentName.size()) {
            strMaxIndex = componentName.size() - 1;
        }
        for(size_t i = 0; i <= strMaxIndex ; ++i) {
            d_componentName[i] = componentName[i];
        }
        for(size_t i = strMaxIndex + 1; i <= 49; ++i) {
            d_componentName[i] = '\0';
        }
        //std::cout << "DEBUG: after setComponentName, d_componentName = " << d_componentName << "\n";
    }


    // copy assignment
    Heartbeat& operator=(const Heartbeat& other)
    {
        if(this != &other)   // self-check
        {
            d_lastHeartbeatTime = other.d_lastHeartbeatTime;
            setComponentName(other.d_componentName);
        }
        return *this;
    }
};

#endif // HEARTBEAT_H

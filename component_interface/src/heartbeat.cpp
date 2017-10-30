#include "heartbeat.h"

Heartbeat::Heartbeat() :
    d_lastHeartbeatTime(0)
{
    for(size_t i = 0; i < 50; ++i) {
        d_componentName[i] = '\0';
    }
}

Heartbeat::~Heartbeat()
{

}

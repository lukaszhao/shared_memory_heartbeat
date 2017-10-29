#include "sharedmemoryofheartbeats.h"

#include <iostream>


// initialize static member
std::string SharedMemoryOfHeartbeats::d_sharedMemoryName = "WatchdogSharedMemoryOfHeartbeats";


SharedMemoryOfHeartbeats::~SharedMemoryOfHeartbeats()
{

}

bool SharedMemoryOfHeartbeats::refreshHeartbeatTime(int index_of_shm_unit)
{
    if(index_of_shm_unit >= d_maxNumComponents) {
        // error: trying to access unit out of bound
        std::cout << "ERROR: trying to access unit out of bound of shared memory !\n";
        return false;
    }

    // refresh last heartbeat time
    (*d_heartbeatVectorPtr)[index_of_shm_unit].setLastHeartbeatTimeAsNow();

    std::cout << "DEBUG: refreshed heartbeat time stamp for index_of_shm_unit = " << index_of_shm_unit << "\n";

    return true;
}

bool SharedMemoryOfHeartbeats::addComponentToSharedMemory(const std::string& componentName,
                                                          int& index_of_shm_unit)
{
    // check if vector still has space for adding one more component
    if(static_cast<int>(d_heartbeatVectorPtr->size()) == d_maxNumComponents) {
        // there is no more space to add one more component
        std::cout << "ERROR: Adding component [" << componentName << "] Faild ! Not enough shared memory space !\n";
        return false;
    }

    Heartbeat hbObj;
    hbObj.setComponentName(componentName);
    hbObj.setLastHeartbeatTimeAsNow();

    d_heartbeatVectorPtr->push_back(hbObj);

    index_of_shm_unit = static_cast<int>(d_heartbeatVectorPtr->size()) - 1;

    std::cout << "INFO: successfully added component [" << componentName <<"]\n";

    return true;
}


bool SharedMemoryOfHeartbeats::readComponentLastHeartbeatTime(int index_of_shm_unit,
                                                              time_t& lastHeartbeatTime) const
{
    // first check if index_of_shm_unit is out of bound or is unit unused
    if(index_of_shm_unit >= d_maxNumComponents) {
        std::cout << "ERROR: index_of_shm_unit is out of bound of shard memory !\n";
        return false;
    }

    // passed checking
    lastHeartbeatTime = (*d_heartbeatVectorPtr)[index_of_shm_unit].lastHeartbeatTime();
    return true;
}

bool SharedMemoryOfHeartbeats::destroySharedMemory()
{
    try{
        std::cout << "First destroy the vector from the segment\n";
        //Destroy the vector from the segment
	    d_segment.destroy<HeartbeatVector>("MyHeartbeatVector");

	    std::cout << "Then destroy managed_shared_memory\n";
	    //Destroy managed_shared_memory using shared_memory_object::remove()
        shared_memory_object::remove("WatchdogSharedMemoryOfHeartbeats");

        return true;
    } catch (...) {
        return false;
    }
}


// friend function
std::ostream& operator<<(std::ostream& os, const SharedMemoryOfHeartbeats& rhs)
{
    os << "shared memory name: " << rhs.d_sharedMemoryName << "\n"
       << "shared memory maxNumComponents: " << rhs.d_maxNumComponents << "\n"
       << "-------------------------------------------------\n";

    for(unsigned int i = 0 ; i < rhs.d_heartbeatVectorPtr->size(); ++i) {
        os << "  i = " << i << "\n"
           << "    componentName = " << rhs.d_heartbeatVectorPtr->at(i).componentName() << "\n"
           << "    lastHeartbeatTime = " << rhs.d_heartbeatVectorPtr->at(i).lastHeartbeatTime() << "\n\n";
    }
    return os;
}

#ifndef SHAREDMEMORYOFHEARTBEATS_H
#define SHAREDMEMORYOFHEARTBEATS_H

#include "heartbeat.h"


#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <string>
#include <iostream>
#include <ostream>

using namespace boost::interprocess;

//Define an STL compatible allocator of Heartbeat that allocates from the managed_shared_memory.
//This allocator will allow placing containers in the segment
typedef allocator<Heartbeat, managed_shared_memory::segment_manager> ShmemAllocator;

//Alias a vector that uses the previous STL-like allocator so that allocates
//its values from the segment
typedef vector<Heartbeat, ShmemAllocator> HeartbeatVector;

class SharedMemoryOfHeartbeats
{
public:

    // prefer to call the following constructor with default argument value
    // so that the size of the shared memory is managed by this class
    SharedMemoryOfHeartbeats(int maxNumComponents = 1000) :
        d_segment(open_or_create, "WatchdogSharedMemoryOfHeartbeats", maxNumComponents * sizeof(Heartbeat) + 1000),
        // assume vector handle over head is less than 1000 bytes
        d_maxNumComponents(maxNumComponents)
    {
        // first try find the vector "MyHeartbeatVector"
        // see if it already exists (if other process created the shared memory and constructed the vector)

        d_heartbeatVectorPtr = d_segment.find<HeartbeatVector>("MyHeartbeatVector").first;

        if(d_heartbeatVectorPtr == 0) {
            // did not find it, this process must be the first to create the the shared memory
            // so we also need to construct the vector
            //std::cout << "DEBUG: Did not find MyHeartbeatVector in shared memory, need to construct it.\n";

            //Initialize shared memory STL-compatible allocator
            const ShmemAllocator alloc_inst(d_segment.get_segment_manager());

            //Construct a vector named "MyHeartbeatVector" in shared memory with argument alloc_inst
            d_heartbeatVectorPtr = d_segment.construct<HeartbeatVector>("MyHeartbeatVector")(alloc_inst);
        } else {
            //std::cout << "DEBUG: Found MyHeartbeatVector in shared memory, size = " << d_heartbeatVectorPtr->size() << "\n";
        }
    }

    ~SharedMemoryOfHeartbeats(){};

private:
    static std::string           d_sharedMemoryName;
    managed_shared_memory        d_segment;
    int                          d_maxNumComponents;
    HeartbeatVector*             d_heartbeatVectorPtr;

public:

    bool refreshHeartbeatTime(int index_of_shm_unit);
    // refresh a vector unit's heartbeat time stamp

    bool addComponentToSharedMemory(const std::string& componentName,
                                    int& index_of_shm_unit);
    // add a component to shared memory
    // if component already exists in shared memory (assume componentName unique)
    // return the index of the unit by int& index_of_shm_unit
    // otherwise add component in vector then return index_of_shm_unit
    // function returns true if succeeds, o.w. false

    bool readComponentLastHeartbeatTime(int index_of_shm_unit,
                                        time_t& lastHeartbeatTime) const;
    // function returns true if succeeds

    bool readComponentName(int index_of_shm_unit,
                           std::string& componentName) const;
    // function returns true if succeeds

    bool destroySharedMemory();
    // this function destroys vector first, then destroys shared memory
    // shared memory does not follow RAII, therefore to release shared memory
    // this function needs to be explicitly called, or shared memory will not be released

    friend std::ostream& operator<<(std::ostream& os, const SharedMemoryOfHeartbeats& rhs);



    // getters
    const std::string& sharedMemoryName() const
    {
        return d_sharedMemoryName;
    }

    int maxNumComponents() const
    {
        return d_maxNumComponents;
    }

    int vectorSize() const
    {
        return d_heartbeatVectorPtr->size();
    }


};

#endif // SHAREDMEMORYOFHEARTBEATS_H


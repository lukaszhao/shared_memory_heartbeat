#include <iostream>

#include <componentinterface.h>
#include <watchdoginterface.h>

using namespace std;


void test_component()
{
    ComponentInterface compInterface("HeartAttack001");
    compInterface.updateHeartbeatTimestampPeriodically();

    char yesOrNo = 'n';
    std::cout << "Enter y to stop... \n"; std::cin >> yesOrNo;
}

void test_watchdog()
{
    WatchdogInterface dogInterface;
    dogInterface.monitorAllComponentsPeriodically();

    char yesOrNo = 'n';
    std::cout << "Enter y to stop... \n"; std::cin >> yesOrNo;
}

void try_destroy()
{
    try
    {
        managed_shared_memory d_segment(open_or_create, "WatchdogSharedMemoryOfHeartbeats", 1000 * sizeof(Heartbeat) + 1000);
        //std::cout << "First destroy the vector from the segment\n";
        //Destroy the vector from the segment
        //d_segment.destroy<HeartbeatVector>("MyHeartbeatVector");

        std::cout << "destroy managed_shared_memory\n";
        //Destroy managed_shared_memory using shared_memory_object::remove()
        shared_memory_object::remove("WatchdogSharedMemoryOfHeartbeats");
    }
    catch (...)
    {
        std::cout << "Failed to destroy\n";
    }
}

int main()
{
    while(true) {
        cout << "0. Destroy shared memory\n";
        cout << "1. Start component\n";
        cout << "2. Start watchdog\n";
        cout << "3. Print shared memory\n";
        cout << "4. Add one component to shared memory\n";
        cout << "5. get last heartbeat time\n";
        cout << "6. update heartbeat time\n";
        int selectNum;
        cout << "select: "; cin >> selectNum;

        switch(selectNum){
            case 0: {
                try_destroy();
            } break;

            case 1: {
                test_component();
            } break;

            case 2: {
                test_watchdog();
            } break;

            case 3: {
                SharedMemoryOfHeartbeats shm;
                cout << "Shared memory:\n" << shm;
            } break;

            case 4: {
                getchar();
                SharedMemoryOfHeartbeats shm;
                int index_temp;
                std::string compNameTemp;
                cout << "Component name = ";
                std::getline(cin, compNameTemp);
                shm.addComponentToSharedMemory(compNameTemp, index_temp);
            } break;

            case 5: {
                int i;
                cout << "i = "; cin >> i;
                SharedMemoryOfHeartbeats shm;
                time_t timeTemp;
                bool success = shm.readComponentLastHeartbeatTime(i, timeTemp);
                if(success) {
                    std::cout << "last heartbeat time = " << timeTemp << "\n";
                } else {
                    std::cout << "Failed to get last heartbeat time\n";
                }
            } break;

            case 6: {
                int i;
                cout << "i = "; cin >> i;
                SharedMemoryOfHeartbeats shm;
                bool success = shm.refreshHeartbeatTime(i);
                if(success) {
                    std::cout << "Successfully refreshed heartbeat time\n";
                } else {
                    std::cout << "Failed to refresh heartbeat time\n";
                }
            } break;
        }
    }


    return 0;
}

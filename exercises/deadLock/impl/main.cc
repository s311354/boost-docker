#include <iostream>
#include <thread>

#include "./deadlock.h"

/**
 *  @brief Verify Guy object's information
 */
bool verifyGuy(const Guy& guy, const std::string name, const unsigned int age, const std::string id)
{
    if ((guy.getName() == name) && (guy.getAge() == age) && (guy.getId() == id))
    {
        return true;
    } else {
        return false;
    }
}

/**
 * @ brief A thread-safe swap demo that will cause deadlock
 * This function will run forever due to deadlock
*/
void deadLockDemo()
{
    const std::string michaelName{"Michael"};
    const unsigned int michaelAge{28};
    const std::string michaelId{"X0001"};

    const std::string howardName{"Howard"};
    const unsigned int howardAge{35};
    const std::string howardId{"Y9999"};

    Guy michael{michaelName, michaelAge, michaelId};
    Guy howard{howardName, howardAge, howardId};

    GuyWrapper michaelWrapped{michael};
    GuyWrapper howardWrapped{howard};

    bool pass = true;

    for (int i = 0; i < 1000; i ++)
    {
        std::thread thread0{swapDeadLock, std::ref(michaelWrapped), std::ref(howardWrapped)};
        thread0.join();
        std::thread thread1{swapDeadLock, std::ref(howardWrapped), std::ref(michaelWrapped)};
        thread1.join();
        std::thread thread2{swapDeadLock, std::ref(michaelWrapped), std::ref(howardWrapped)};
        thread2.join();
        std::thread thread3{swapDeadLock, std::ref(howardWrapped), std::ref(michaelWrapped)};     
        thread3.join();

        if (!verifyGuy(michael, michaelName, michaelAge, michaelId))
        {
            std::cout << "Expected Michael: " << michaelName << " " << michaelAge << " " << michaelId << " " << std::endl;
            std::cout << "Go Michael: " << michael.getName() << " " << michael.getAge() << " " << michael.getId() << " " << std::endl;
            pass = false;
            break;
        }

       if (!verifyGuy(howard, howardName, howardAge, howardId))
       {
           std::cout << "Expected Howard: " << howardName <<  " " << howardAge << " " << howardId << " " << std::endl;
           std::cout << "Got Howard: " << howardName <<  " " << howardAge << " " << howardId << " " << std::endl;
           pass = false;
           break;
       }
    }

    if (pass)
    {
        std::cout << "Thread Lock Test Passed!" << std::endl;
    } else {
        std::cout << "Thread Lock Test Failed!" << std::endl;
    }
}

/**
 * @brief A thread-safe swap demo
 * After even number of swap, the information of Michael and Howard remain
 * unchanged
*/
void safeDemo()
{
    const std::string michaelName{"Michael"};
    const unsigned int michaelAge{28};
    const std::string michaelId{"X0001"};

    const std::string howardName{"Howard"};
    const unsigned int howardAge{35};
    const std::string howardId{"Y9999"};

    Guy michael{michaelName, michaelAge, michaelId};
    Guy howard{howardName, howardAge, howardId};

    GuyWrapper michaelWrapped{michael};
    GuyWrapper howardWrapped{howard};

    bool pass = true;

    for (int i = 0; i < 1000; i ++)
    {
        std::cout << "Time: "  << i << std::endl;
        std::thread thread0{swapSafe, std::ref(michaelWrapped), std::ref(howardWrapped)};
        std::thread thread1{swapSafe, std::ref(howardWrapped),  std::ref(michaelWrapped)};
        std::thread thread2{swapSafe, std::ref(michaelWrapped), std::ref(howardWrapped)};
        std::thread thread3{swapSafe, std::ref(howardWrapped),  std::ref(michaelWrapped)};     

        thread0.join();
        thread1.join();
        thread2.join();
        thread3.join();

        if (!verifyGuy(michael, michaelName, michaelAge, michaelId))
        {
            std::cout << "Expected Michael: " << michaelName << " " << michaelAge << " " << michaelId << " " << std::endl;
            std::cout << "Go Michael: " << michael.getName() << " " << michael.getAge() << " " << michael.getId() << " " << std::endl;
            pass = false;
            break;
        }

       if (!verifyGuy(howard, howardName, howardAge, howardId))
       {
           std::cout << "Expected Howard: " << howardName <<  " " << howardAge << " " << howardId << " " << std::endl;
           std::cout << "Got Howard: " << howardName <<  " " << howardAge << " " << howardId << " " << std::endl;
           pass = false;
           break;
       }
    }

    if (pass)
    {
        std::cout << "Thread Lock Test Passed!" << std::endl;
    } else {
        std::cout << "Thread Lock Test Failed!" << std::endl;
    }
}

/**
 * @ brief A thread-unsafe swap demo
 * After even number of swap, we expect the information of Michael and Howard
 * remain unchanged. However, because of race conditions, sometimes, the information
 * of Michael and Howard are incorrect.
*/
void unsafeDemo()
{
    const std::string michaelName{"Michael"};
    const unsigned int michaelAge{28};
    const std::string michaelId{"X0001"};

    const std::string howardName{"Howard"};
    const unsigned int howardAge{35};
    const std::string howardId{"Y9999"};

    Guy michael{michaelName, michaelAge, michaelId};
    Guy howard{howardName, howardAge, howardId};

    bool pass = true;

    for (int i = 0; i < 1000; i ++)
    {
        std::thread thread0{swap, std::ref(michael), std::ref(howard)};
        std::thread thread1{swap, std::ref(howard), std::ref(michael)};
        std::thread thread2{swap, std::ref(michael), std::ref(howard)};
        std::thread thread3{swap, std::ref(howard), std::ref(michael)};     

        thread0.join();
        thread1.join();
        thread2.join();
        thread3.join();

        if (!verifyGuy(michael, michaelName, michaelAge, michaelId))
        {
            std::cout << "Expected Michael: " << michaelName << " " << michaelAge << " " << michaelId << " " << std::endl;
            std::cout << "Go Michael: " << michael.getName() << " " << michael.getAge() << " " << michael.getId() << " " << std::endl;
            pass = false;
            break;
        }

       if (!verifyGuy(howard, howardName, howardAge, howardId))
       {
           std::cout << "Expected Howard: " << howardName <<  " " << howardAge << " " << howardId << " " << std::endl;
           std::cout << "Got Howard: " << howardName <<  " " << howardAge << " " << howardId << " " << std::endl;
           pass = false;
           break;
       }
    }

    if (pass)
    {
        std::cout << "Thread Lock Test Passed!" << std::endl;
    } else {
        std::cout << "Thread Lock Test Failed!" << std::endl;
    }
}

int main ()
{
    unsafeDemo();
    safeDemo();    
    deadLockDemo();
}

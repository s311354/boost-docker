#include <cstdlib>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

/**
 * @bridf Thread safe sleep function
 * Randomly sleep 0 to 5 nano seconds
*/ 
void random_sleep()
{
    std::chrono::nanoseconds random_duration{std::rand() % 5};
    std::this_thread::sleep_for(random_duration);
}

/**
 *@bridf A Dummy class for personal information
*/
class Guy
{
public:
    Guy();
    Guy(std::string name, unsigned int age, std::string id) : mName{name}, mAge{age}, mId{id} {};

    void setName(const std::string name)
    {
        random_sleep();
        this->mName = name;
    }

    void setAge(const unsigned int age)
    {
        random_sleep();
        this->mAge = age;
    }

    void setId(const std::string id)
    {
        random_sleep();
        this->mId = id;
    }

    std::string getName() const
    {
        random_sleep();
        return this->mName;
    }

    unsigned int getAge() const
    {
        random_sleep();
        return this->mAge;
    }

    std::string getId() const
    {
       random_sleep();
       return this->mId;
    }

private:
    std::string mName;
    unsigned int mAge;
    std::string mId;
};

/**
 *@bridf A Guy wrapper that has an additional std::mutex
 */
class GuyWrapper
{
public:
    GuyWrapper(Guy& guy) : mGuy{guy} {};
    std::mutex mMutex;
    Guy& mGuy;
};

/**
 * @brief Swap the information of two Guy objects.
 * This swap function is thread-unsaft.
*/
void swap(Guy& lhs, Guy& rhs)
{
    if (&lhs == &rhs)
    {
        return;
    }

    std::string tmpName = lhs.getName();
    unsigned int tmpAge = lhs.getAge();
    std::string tmpId = lhs.getId();
    lhs.setName(rhs.getName());
    lhs.setAge(rhs.getAge());
    lhs.setId(rhs.getId());
    rhs.setName(tmpName);
    rhs.setAge(tmpAge);
    rhs.setId(tmpId);
}

/**
 * @brief A swap function that uses mutex but will cause deadlock.
 * Even if we are always locking the first GuyWrapper first and the second
 * GuyWrapper second. Ths user could switch the order of these two objects and
 * cause dead lock.
*/
void swapDeadLock(GuyWrapper& lhs, GuyWrapper& rhs)
{
    if (&lhs == &rhs)
    {
        return;
    }

    std::lock_guard<std::mutex> lock_lhs(lhs.mMutex);
    std::lock_guard<std::mutex> lock_rhs(rhs.mMutex);

    swap(lhs.mGuy, rhs.mGuy);
}

/**
 * @brief A thread-safe swap function.
 * Locking two std::mutex simultaneously prevented deadlock
 * @param lhs
 * @param rhs
*/
void swapSafe(GuyWrapper& lhs, GuyWrapper& rhs)
{
    if (&lhs == &rhs)
    {
        return;
    }

    std::lock(lhs.mMutex, rhs.mMutex);

    // lock_guard objects constructed with adopt_lock do not lock the mutex object on construction,
    // assuming instead that it is already locked by the current thread.
    std::lock_guard<std::mutex> lock_lhs(lhs.mMutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_rhs(lhs.mMutex, std::adopt_lock);

    swap(lhs.mGuy, rhs.mGuy);
}

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

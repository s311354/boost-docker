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
void swap(Guy& lhs, Guy& rhs);

/**
 * @brief A swap function that uses mutex but will cause deadlock.
 * Even if we are always locking the first GuyWrapper first and the second
 * GuyWrapper second. Ths user could switch the order of these two objects and
 * cause dead lock.
*/
void swapDeadLock(GuyWrapper& lhs, GuyWrapper& rhs);

/**
 * @brief A thread-safe swap function.
 * Locking two std::mutex simultaneously prevented deadlock
 * @param lhs
 * @param rhs
*/
void swapSafe(GuyWrapper& lhs, GuyWrapper& rhs);

#include "./deadlock.h"

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
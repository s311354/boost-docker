#include <mutex>
#include <boost/thread/lockable_adapter.hpp>
#include "externally_locked.h"

#ifdef EXTERNAL_LOCK
class BankAccount 
: public boost::basic_lockable_adapter<boost::mutex>  // lockable functions
{
    int balance_;

    public:
        void Deposit(int amount, strict_lock<BankAccount> & guard) {
            // Externally locked
            if (!guard.owns_lock(this))
                throw "Locking Error: Wrong Object Locked";
            balance_ += amount;
        }

        void Deposit(int amount) {
            strict_lock<BankAccount> guard(*this); // Internally locked
            Deposit(amount, guard);
        }

        void Withdraw(int amount, strict_lock<BankAccount> & guard) {
            // Externally locked
            if (!guard.owns_lock(this))
                throw "Locking Error: Wrong Object Locked";
            balance_ -= amount; 
        }

        void Withdraw(int amount) {
            strict_lock<BankAccount> guard(*this); // Internally locked
            Withdraw(amount, guard);
        }

        int GetBalance() {
            boost::lock_guard<BankAccount> guard(*this);
            return balance_;
        }
};

#elif LOCK_GUARD_LOCKABLE_ADAPTER
// Internal locking is insufficient for many real-world synchronization tasks.
// By add lock/unlock functions from outside
class BankAccount
: public boost::basic_lockable_adapter<std::mutex> // syntax error: expected template-name before '<' token 
{
    int balance_;

    public:
        void Deposit(int amount) {
           boost::lock_guard<BankAccount> guard(*this);
           balance_ += amount;
        }

        void Withdraw(int amount) {
           boost::lock_guard<BankAccount> guard(*this);
           balance_ -= amount;
        }

        int GetBalance() {
           boost::lock_guard<BankAccount> guard(*this);
           return balance_;
        }
};
#elif defined LOCKABLE_ADAPTER
class BankAccount 
: public boost::basic_lockable_adapter<std::mutex> // syntac error: expected template-name before '<' token
{
    int balance_;

    public:
        void Deposit(int amount) {
           balance_ += amount;
        }

        void Withdraw(int amount) {
           balance_ -= amount;
        }

        int GetBalance() {
           return balance_;
        }
};
#elif defined LOCK_GUARD
// Guard's constructor locks the passed-in object mtx_, and guard's destructor unlocks mtx_.
// This model is quite deadlock-prone when we try to coordinate multi-object transactions.
// - using synchronization primitives, such as locks
// - avoid the use of blocking calls, such as sleep in critical sections

class BankAccount {
    boost::mutex mtx_; // explicit mutex declaration
    int balance_;

    public:
        void Deposit(int amount) {
           boost::lock_guard<boost::mutex> guard(mtx_);
           balance_ += amount;
        }

        void Withdraw(int amount) {
           boost::lock_guard<boost::mutex> guard(mtx_);
           balance_ -= amount;
        }

        int GetBalance() {
           boost::lock_guard<boost::mutex> guard(mtx_);
           int b = balance_;
           return b;
        }

        int lock() {
           mtx_.lock();
        }
 
        int unlock() {
           mtx_.unlock();
        }
};
#else
class BankAccount {
    boost::mutex mtx_; // explicit mutex declaration
    int balance_;

    public:
        void Deposit(int amount) {
           mtx_.lock();
           balance_ += amount;
           mtx_.unlock();        
        }

        void Withdraw(int amount) {
            mtx_.lock();
            balance_ -= amount;
            mtx_.unlock();
        }

        int GetBalance() {
            mtx_.lock();
            int b = balance_;
            mtx_.unlock();
            return b;
        }
};
#endif

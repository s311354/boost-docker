#include <mutex>
#include <boost/thread/lockable_adapter.hpp>
#include "externally_locked.h"

#ifdef EXTERNAL_INTERNAL_LOCK
// If we want the benefit of internal locking, we simply call Deposit(int) and Withdraw(int)
// If we want to use external locking, we lock the object by constructing a strict_lock<BankAccount> and then you call Deposit(int, strict_lock<BankAccount>&) and Withdraw(int, strict_lock<BankAccount> &)
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
#elif defined CALLER_ENSURED_LOCK
// The caller-ensured locking approach is more flexible and most efficient, but very dangerous
// BankAccount still holds a mutex, but its member functions don't manipulate it at all. Deposit and Withdraw are not thread-safe anymore.
class BankAccount 
: public boost::basic_lockable_adapter<std::mutex>
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
// Internal Locking
// Use a mutex to guarantee exclusive access to each bank
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

// We have access to this BankAccount object only after locking its parent AccountManager object
class AccountManager
: public boost::basic_lockable_adapter<boost::mutex>
{
    public:
        typedef boost::basic_lockable_adapter<boost::mutex> lockable_base_type;

        AccountManager() : checkingAcct_(*this), savingsAcct_(*this) {}

        inline void Checking2Savings(int amount);
        inline void AmoreComplicatedChecking2Savings(int amount);

    private:
        // a little bridge template externally_locked that controls access to a BankAccount
        externally_locked<BankAccount, AccountManager> checkingAcct_;
        externally_locked<BankAccount, AccountManager> savingsAcct_;
};



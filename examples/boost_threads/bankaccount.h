#ifdef LOCK_GUARD_LOCKABLE_ADAPTER
class BankAccount
: public std::basic_lockable_adapter<std::mutex> //  error: expected template-name before '<' token
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
: public std::basic_lockable_adapter<boost::mutex> //  error: expected template-name before '<' token
{
    int balance_;

    public:
        void Deposit(int amount) {
           balance_ += amount;
        }

        void Withdraw(int amount) {
           balance_ -= amount;
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

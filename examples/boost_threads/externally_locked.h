#include <mutex>

using mutex_type = std::mutex;

// strick_lock's role is only to live on the stack as an automatic variable
// strick_lock must adhere to a non-copy and non-alias policy
template <class Lockable>
class strict_lock {
public:
    typedef Lockable lockable_type;

    explicit strict_lock(lockable_type& obj) : obj_(obj) {
        obj.lock(); // locks on construction
    } 

    strict_lock() = delete;

    strict_lock(strict_lock const &) = delete;

    strict_lock& operator = (strict_lock const &) = delete;

    ~strict_lock() { obj_.unlock(); } // unlocks on destruction 

    bool owns_lock(Lockable * mtx) const // strict lockers specific function
    {
        return mtx == &obj_;
    }

private:
    lockable_type& obj_;
};

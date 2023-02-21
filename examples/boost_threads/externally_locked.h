#include <mutex>
#include <boost/concept_check.hpp>

using mutex_type = std::mutex;

// strict_lock's role is only to live on the stack as an automatic variable
// strict_lock must adhere to a non-copy and non-alias policy
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

// Use a little bridge template externallly_locked that controls access to a BankAccount
// externally_locked cloaks an object of type T, and actually provides full access to that object through the get and set member functions, provided we pass a reference tp a strict_lock<Owner> object.
template <typename T, typename Lockable>
class externally_locked {
    // This macro is used to check that a given template parameter meets certain requirements of 
    // has certain properties 
    //BOOST_CONCEPT_ASSERT((boost::LockableConcept<Lockable>));

public:
    externally_locked(T & obj, Lockable & lockable): obj_(obj), lockable_(lockable) {}

    externally_locked(Lockable& lockable): obj_(), lockable_(lockable) {}

    T& get (strict_lock<Lockable> & lock) {

#ifdef BOOST_THREAD_THROW_IF_PRECONDITION_NOT_SATISFIED
        if (!lock.owns_lock(&lockable_)) throw boost::lock_error(); // run time check throw if not locks the same
#endif
        return obj_;
    }

   void set(const T& obj, Lockable& lockable) {
      obj_ = obj;
      lockable_ = lockable;
   }

private:
   T obj_;
   Lockable & lockable_;
};

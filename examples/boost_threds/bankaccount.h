
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

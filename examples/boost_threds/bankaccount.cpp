#include <boost/thread.hpp>
#include "bankaccount.h"

BankAccount JoesAccount;

void bankAgent()
{
    for (int i = 10; i > 0; i --) {
        JoesAccount.Deposit(500);
    }
}

void Joe() {
    for (int i = 10; i > 0; --i) {
       // error: void value not ignored as it ought to be
       // int mypocket = JoesAccount.Withdraw(100);

       // The correct way
       JoesAccount.Withdraw(100);
       int mypocket = JoesAccount.GetBalance();
       std::cout << mypocket << std::endl;
    }
}

int main() {
    boost::thread thread1(bankAgent); // start concurrent execution of bankAgent
    boost::thread thread2(Joe);       // start concurrent execution of Joe
    thread1.join();
    thread2.join();
    return 0;
}


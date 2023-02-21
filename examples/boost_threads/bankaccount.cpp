#include <boost/thread.hpp>
#include "bankaccount.h"
#include <iostream>

BankAccount JoesAccount;

void AccountManager::Checking2Savings(int amount) {
    strict_lock<AccountManager> guard(*this);

    checkingAcct_.get(guard).Withdraw(amount);
    savingsAcct_.get(guard).Deposit(amount);

    int checkingpocket = checkingAcct_.get(guard).GetBalance();
    int savingpocket = savingsAcct_.get(guard).GetBalance();
    std::cout << "Checking account " << checkingpocket << " Saving account " << savingpocket << std::endl;
}

void ATMWithdrawal(BankAccount& acct, int sum) {
    //strict_lock<BankAccount> guard(acct); // Hang issue
    acct.Withdraw(sum);
    acct.Withdraw(2);
}

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

       ATMWithdrawal(JoesAccount, 100);     

       // The correct way
       // JoesAccount.Withdraw(100);
       int mypocket = JoesAccount.GetBalance();
       std::cout << mypocket << std::endl;
    }
}

int main() {

    boost::thread thread1(bankAgent); // start concurrent execution of bankAgent
    boost::thread thread2(Joe);       // start concurrent execution of Joe
    thread1.join();
    thread2.join();

    // AccountManager class holds and manupulates a BankAccount object
    AccountManager().Checking2Savings(100);

    return 0;
}

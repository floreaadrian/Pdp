#include "Bank.h"

Bank::Bank()
{
}

void Bank::saveAccount(BankAccount* bankAccount)
{
    
//    this->bankAccounts.push_back(bankAccount);
    this->bankAccountsMap[bankAccount->getId()] = bankAccount;
}

bool Bank::transfer(BankAccount* sender, BankAccount* reciever, double amount)
{
    this->mtx.lock();
    if (sender->getId() > reciever->getId())
    {
        sender->m.lock();
        reciever->m.lock();
    }
    else
    {
        reciever->m.lock();
        sender->m.lock();
    }
    if (sender->getAmount() < amount)
    {
        if (sender->getId() > reciever->getId())
        {
            sender->m.unlock();
            reciever->m.unlock();
        }
        else
        {
            reciever->m.unlock();
            sender->m.unlock();
        }
        this->mtx.unlock();
        return false;
    }

    sender->subtractAmount(amount);
    reciever->addAmount(amount);
    sender->addLog(0 - amount);
    reciever->addLog(amount);
    if (sender->getId() < reciever->getId())
    {
        sender->m.unlock();
        reciever->m.unlock();
    }
    else
    {
        reciever->m.unlock();
        sender->m.unlock();
    }
    this->mtx.unlock();
    return true;
}

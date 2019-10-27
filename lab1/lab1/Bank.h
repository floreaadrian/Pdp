#pragma once
#include <vector>
#include <algorithm>
#include "BankAccount.h"
#include <map>

using namespace std;
class Bank
{
public:
//    vector<BankAccount*> bankAccounts;
    map<int, BankAccount*> bankAccountsMap;
    mutex mtx;
    Bank();
    void saveAccount(BankAccount* bankAccount);
    bool transfer(BankAccount* sender, BankAccount* reciever, double amount);
    int size();
    BankAccount* getBankAccountById(unsigned int id);
    ~Bank();
};

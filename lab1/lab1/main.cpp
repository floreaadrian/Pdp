#include <iostream>
#include <thread>
#include <ctime>
#include <numeric>
#include <vector>
#include <assert.h>
#include <chrono>
#include <ratio>
#include "Bank.h"

using namespace std;
using namespace std::chrono;

Bank* initializeBank(int no_of_bank_accounts)
{
    Bank* bank = new Bank();
    for (int i = 0; i < no_of_bank_accounts; i++)
    {
        BankAccount* bankAccount = new BankAccount(i + 10, double(rand() % 100000 + 1) / 100);
        bank->saveAccount(bankAccount);
    }
    
    return bank;
}

//void checkBankStatus(Bank* bank)
//{
//    for (BankAccount* bankAccount : bank->bankAccounts)
//    {
//        double totalTransfers = 0;
//        for (double log : bankAccount->getLogs())
//        {
//            totalTransfers += log;
//        }
//        assert(totalTransfers == bankAccount->getAmount());
//    }
//}

void transfer(Bank* bank, int no_of_bank_accounts, int transfer_numbers){
    for(int i=0; i < transfer_numbers; ++i){
        int id1 = rand() % no_of_bank_accounts + 10;
        int id2 = rand() % no_of_bank_accounts + 10;
        while (id1 == id2)
        {
            id2 = rand() % no_of_bank_accounts + 10;
        }
        double amount = double(rand() % 1000 + 1) / 100;
        bank->transfer(bank->bankAccountsMap[id1], bank->bankAccountsMap[id2], amount);
    }
}

double transferTest(int no_of_bank_accounts, int transfer_numbers, int no_of_threads) {
    Bank* bank = initializeBank(no_of_bank_accounts);
    thread thr[no_of_threads + 1];
    
    auto start4 = chrono::high_resolution_clock::now();
    for (int i = 0; i < no_of_threads; i++)
    {
        thr[i] = thread(transfer, bank, no_of_bank_accounts, transfer_numbers/no_of_threads);
    }
    
    for (int i = 0; i < no_of_threads; i++)
    {
        thr[i].join();
    }
    auto finish4 = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed4 = finish4 - start4;

    return elapsed4.count();
}

void statistic(int no_of_bank_accounts, int transfer_numbers, int no_of_threads){
    double nr = 0;
    int nr_test = 5;
    transferTest(no_of_bank_accounts,transfer_numbers,no_of_threads);
    for(int i = 1; i < nr_test; ++i) {
        nr += transferTest(no_of_bank_accounts,transfer_numbers,no_of_threads);
    }
    cout<<"------------------------------------------------\n";
    cout<<no_of_threads<<"   | "<<nr/4<<" s\n";
}

int main()
{
    srand(time(NULL));
    cout<<"threads | seconds\n";
    statistic(100000, 200000, 1);
    statistic(100000, 200000, 2);
    statistic(100000, 200000, 4);
    statistic(100000, 200000, 8);
    statistic(100000, 200000, 16);
//    statistic(50000,1000,8);
//    statistic(50000,2000,50);
//    statistic(50000,2000,100);
//    statistic(50000,2000,500);
//    statistic(50000,2000,1000);
    return 0;
}

#pragma once
#include <string>
#include <vector>
//#include <thread>
#include <mutex>

using namespace std;

class BankAccount
{
private:
    unsigned int id;
    double amount;
    vector<double> log;
public:
    mutex m;

    BankAccount();
    BankAccount(unsigned int id, double amount);
    
    void addAmount(double amount);
    void subtractAmount(double amount);
    void addLog(double operation);
    unsigned int getId();
    double getAmount();
    vector<double> getLogs();
    ~BankAccount();
};


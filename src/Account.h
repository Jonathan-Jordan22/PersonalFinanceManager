#pragma once
#include <string>
#include <vector>
#include "Transaction.h"

class Account {
private:
    std::string name;
    double balance;
    std::vector<Transaction> transactions;

public:
    // Constructor
    Account(const std::string& name, double initialBalance = 0.0);

    // Methods
    void addTransaction(double amount, const std::string& category, const std::string& description);
    double getBalance() const;
    void printTransactions() const;
};

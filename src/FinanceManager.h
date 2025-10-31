#pragma once
#include <vector>
#include <string>
#include "Account.h"

class FinanceManager {
private:
    std::vector<Account> accounts;

public:
    // Add a new account
    void addAccount(const std::string& name, double initialBalance = 0.0);

    // Display summary of all accounts
    void printAccountSummary() const;

    // Get total net worth (sum of balances)
    double getTotalBalance() const;
};
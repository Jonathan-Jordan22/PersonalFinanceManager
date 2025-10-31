#include "Account.h"
#include <iostream>
#include <iomanip>

Account::Account(const std::string& name, double initialBalance)
    : name(name), balance(initialBalance) {}

void Account::addTransaction(double amount, const std::string& category, const std::string& description) {
    Transaction t(amount, category, description);
    transactions.push_back(t);
    balance += amount; // Positive for income, negative for expense
}

double Account::getBalance() const {
    return balance;
}

void Account::printTransactions() const {
    std::cout << "Transaction history for account: " << name << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    for (const auto& t : transactions) {
        std::cout << std::setw(12) << t.getDateString() 
                  << " | " << std::setw(10) << t.getCategory()
                  << " | " << std::setw(8) << t.getAmount()
                  << " | " << t.getDescription() << std::endl;
    }

    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
}

std::string Account::getName() const {
    return name;
}

const std::vector<Transaction>& Account::getTransactions() const {
    return transactions;
}

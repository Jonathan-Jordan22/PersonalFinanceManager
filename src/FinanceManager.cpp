#include "FinanceManager.h"
#include <iostream>
#include <iomanip>

void FinanceManager::addAccount(const std::string& name, double initialBalance) {
    accounts.emplace_back(name, initialBalance);
}

void FinanceManager::printAccountSummary() const {
    std::cout << "\n=== Account Summary ===" << std::endl;
    std::cout << std::left << std::setw(20) << "Account Name" 
              << std::right << std::setw(15) << "Balance" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    for (const auto& account : accounts) {
        std::cout << std::left << std::setw(20) << account.getName()
                  << std::right << std::setw(15) << account.getBalance() << std::endl;
    }

    std::cout << "-----------------------------------------" << std::endl;
    std::cout << std::left << std::setw(20) << "Total"
              << std::right << std::setw(15) << getTotalBalance() << std::endl;
}

double FinanceManager::getTotalBalance() const {
    double total = 0.0;
    for (const auto& account : accounts) {
        total += account.getBalance();
    }
    return total;
}

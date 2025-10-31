#include "FinanceManager.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

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

std::vector<Account>& FinanceManager::getAccounts() {
    return accounts;
}

void FinanceManager::listAccounts() const {
    std::cout << "\nAvailable Accounts:\n";
    for (size_t i = 0; i < accounts.size(); ++i) {
        std::cout << i + 1 << ". " << accounts[i].getName() 
                  << " (Balance: $" << std::fixed << std::setprecision(2)
                  << accounts[i].getBalance() << ")\n";
    }
}

void FinanceManager::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cout << "Error opening file for writing.\n";
        return;
    }

    for (const auto& account : accounts) {
        outFile << account.getName() << "|" << account.getBalance() << "\n";
        // Save transactions
        for (const auto& t : account.getTransactions()) {
            outFile << t.getAmount() << "|" 
                    << t.getCategory() << "|" 
                    << t.getDescription() << "|" 
                    << t.getDateString() << "\n";
        }
    }
}

void FinanceManager::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) return; // File may not exist yet

    accounts.clear();
    std::string line;
    Account* currentAccount = nullptr;

    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string part;
        std::vector<std::string> tokens;

        while (std::getline(ss, part, '|')) tokens.push_back(part);

        if (tokens.size() == 2) {
            // New account
            accounts.emplace_back(tokens[0], std::stod(tokens[1]));
            currentAccount = &accounts.back();
        } else if (tokens.size() == 4 && currentAccount) {
            double amount = std::stod(tokens[0]);
            std::string category = tokens[1];
            std::string description = tokens[2];
            // Ignoring date parsing for simplicity; recreate Transaction
            currentAccount->addTransaction(amount, category, description);
        }
    }
}

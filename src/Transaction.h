#pragma once
#include <string>
#include <ctime>

class Transaction {
private:
    double amount;
    std::string category;
    std::string description;
    std::time_t date;

public:
    // Constructor
    Transaction(double amount, const std::string& category, const std::string& description);

    // Getters
    double getAmount() const;
    std::string getCategory() const;
    std::string getDescription() const;
    std::string getDateString() const;
};

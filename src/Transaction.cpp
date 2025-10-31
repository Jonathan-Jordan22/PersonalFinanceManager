#include "Transaction.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction(double amount, const std::string& category, const std::string& description)
    : amount(amount), category(category), description(description), date(std::time(nullptr)) {}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getCategory() const {
    return category;
}

std::string Transaction::getDescription() const {
    return description;
}

std::string Transaction::getDateString() const {
    std::tm* tm_ptr = std::localtime(&date);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

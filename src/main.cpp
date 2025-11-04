#include <iostream>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <sstream>
#include "FinanceManager.h"

// Function to get a valid double (for balances, amounts)
double getValidatedDouble(const std::string &prompt)
{
    double value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (!std::cin.fail())
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
            return value;
        }

        // Invalid input: reset
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a valid number.\n";
    }
}

// Function to get a valid integer within a range (for menu choices)
int getValidatedInt(const std::string &prompt, int min, int max)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (!std::cin.fail() && value >= min && value <= max)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Enter a number between " << min << " and " << max << ".\n";
    }
}

std::time_t parseDate(const std::string &dateStr)
{
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d"); // expected format "YYYY-MM-DD"
    return std::mktime(&tm);
}

void displayMenu()
{
    std::cout << "\n=== Personal Finance Manager ===\n";
    std::cout << "1. Add Account\n";
    std::cout << "2. Add Transaction\n";
    std::cout << "3. View Account Summary\n";
    std::cout << "4. View Transaction History\n";
    std::cout << "5. Exit\n";
    std::cout << "Select an option: ";
}

int main()
{
    FinanceManager fm;
    fm.loadFromFile("finance_data.txt");
    fm.saveToFile("finance_data.txt");
    int choice;

    while (true)
    {
        displayMenu();
        int choice = getValidatedInt("Select an option: ", 1, 5);

        // Input validation
        if (std::cin.fail())
        {
            std::cin.clear();                                                   // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1)
        {
            std::string name;
            std::cout << "Enter account name: ";
            std::getline(std::cin, name);

            double initialBalance = getValidatedDouble("Enter initial balance: ");
            fm.addAccount(name, initialBalance);
            std::cout << "Account added successfully!\n";
        }
        else if (choice == 2)
        {
            if (fm.getAccounts().empty())
            {
                std::cout << "No accounts exist. Add an account first.\n";
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.get();
                continue;
            }

            fm.listAccounts();
            int accountIndex = getValidatedInt("Select account number: ", 1, fm.getAccounts().size());
            Account &selectedAccount = fm.getAccounts()[accountIndex - 1];

            double amount = getValidatedDouble("Enter amount (negative for expense): ");
            std::string category, description;
            std::cout << "Enter category: ";
            std::getline(std::cin, category);
            std::cout << "Enter description: ";
            std::getline(std::cin, description);

            selectedAccount.addTransaction(amount, category, description);
            std::cout << "Transaction added!\n";
        }
        else if (choice == 3)
        {
            fm.printAccountSummary();

            // Pause until user presses Enter
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.get();
        }
        else if (choice == 4)
        {
            if (fm.getAccounts().empty())
            {
                std::cout << "No accounts exist. Add an account first.\n";
                std::cout << "Press Enter to return to the main menu...";
                std::cin.get();
                continue;
            }

            fm.listAccounts();
            int accountIndex = getValidatedInt("Select account number to view transactions: ", 1, fm.getAccounts().size());
            Account &selectedAccount = fm.getAccounts()[accountIndex - 1];

            std::vector<Transaction> filteredTransactions = selectedAccount.getTransactions(); // copy

            while (true)
            {
                std::cout << "\n=== Transaction Options for " << selectedAccount.getName() << " ===\n";
                std::cout << "1. View all transactions\n";
                std::cout << "2. Filter by category\n";
                std::cout << "3. Filter by amount range\n";
                std::cout << "4. Filter by date range\n";
                std::cout << "5. Sort by amount\n";
                std::cout << "6. Sort by date\n";
                std::cout << "7. Return to main menu\n";

                int option = getValidatedInt("Choose an option: ", 1, 7);

                if (option == 7)
                    break; // back to main menu

                filteredTransactions = selectedAccount.getTransactions(); // reset before each filter/sort

                if (option == 2)
                { // filter by category
                    std::string category;
                    std::cout << "Enter category to filter by: ";
                    std::getline(std::cin, category);

                    std::vector<Transaction> categoryFiltered;
                    for (const auto &t : filteredTransactions)
                    {
                        if (t.getCategory() == category)
                        {
                            categoryFiltered.push_back(t);
                        }
                    }
                    filteredTransactions = categoryFiltered;
                }

                else if (option == 3)
                { // Filter by amount range
                    double minAmount, maxAmount;
                    std::cout << "Enter minimum amount: ";
                    std::cin >> minAmount;
                    std::cout << "Enter maximum amount: ";
                    std::cin >> maxAmount;

                    std::vector<Transaction> amountFiltered;
                    for (const auto &t : filteredTransactions)
                    {
                        if (t.getAmount() >= minAmount && t.getAmount() <= maxAmount)
                        {
                            amountFiltered.push_back(t);
                        }
                    }
                    filteredTransactions = amountFiltered;
                }
                else if (option == 4)
                { // Filter by date range
                    std::string startDateStr, endDateStr;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter start date (YYYY-MM-DD): ";
                    std::getline(std::cin, startDateStr);
                    std::cout << "Enter end date (YYYY-MM-DD): ";
                    std::getline(std::cin, endDateStr);

                    std::time_t startDate = parseDate(startDateStr);
                    std::time_t endDate = parseDate(endDateStr);

                    std::vector<Transaction> dateFiltered;
                    for (const auto &t : filteredTransactions)
                    {
                        std::time_t txDate = t.getDate(); // assuming getDate() returns std::time_t
                        if (txDate >= startDate && txDate <= endDate)
                        {
                            dateFiltered.push_back(t);
                        }
                    }
                    filteredTransactions = dateFiltered;
                }
                else if (option == 5)
                { // sort by amount
                    std::cout << "Sort by amount: 1. Ascending 2. Descending: ";
                    int sortChoice = getValidatedInt("", 1, 2);
                    std::sort(filteredTransactions.begin(), filteredTransactions.end(),
                              [sortChoice](const Transaction &a, const Transaction &b)
                              {
                                  return sortChoice == 1 ? a.getAmount() < b.getAmount() : a.getAmount() > b.getAmount();
                              });
                }
                else if (option == 6)
                { // sort by date
                    std::cout << "Sort by date: 1. Oldest first 2. Newest first: ";
                    int sortChoice = getValidatedInt("", 1, 2);
                    std::sort(filteredTransactions.begin(), filteredTransactions.end(),
                              [sortChoice](const Transaction &a, const Transaction &b)
                              {
                                  return sortChoice == 1 ? a.getDate() < b.getDate() : a.getDate() > b.getDate();
                              });
                }

                // Display transactions
                if (filteredTransactions.empty())
                {
                    std::cout << "No transactions found.\n";
                }
                else
                {
                    std::cout << "\n"
                              << std::left << std::setw(10) << "Amount"
                              << std::setw(15) << "Category"
                              << std::setw(25) << "Description"
                              << "Date\n";
                    std::cout << "---------------------------------------------------------------\n";

                    for (const auto &t : filteredTransactions)
                    {
                        std::cout << std::left << std::setw(10) << t.getAmount()
                                  << std::setw(15) << t.getCategory()
                                  << std::setw(25) << t.getDescription()
                                  << t.getDateString() << "\n";
                    }
                }

                // Pause before showing sub-menu again
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            }
        }
        else if (choice == 5)
        {
            std::cout << "Exiting...\n";
            break;
        }
        else
        {
            std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
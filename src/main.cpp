#include <iostream>
#include <limits>
#include <iomanip>
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
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Press Enter to return to the main menu...";
                std::cin.get();
                continue;
            }

            fm.listAccounts();
            int accountIndex = getValidatedInt("Select account number to view transactions: ", 1, fm.getAccounts().size());
            Account &selectedAccount = fm.getAccounts()[accountIndex - 1];

            std::cout << "\n=== Transactions for " << selectedAccount.getName() << " ===\n";
            const auto &transactions = selectedAccount.getTransactions();

            if (transactions.empty())
            {
                std::cout << "No transactions yet.\n";
            }
            else
            {
                std::cout << std::left << std::setw(10) << "Amount"
                          << std::setw(15) << "Category"
                          << std::setw(25) << "Description"
                          << "Date\n";
                std::cout << "---------------------------------------------------------------\n";

                for (const auto &t : transactions)
                {
                    std::cout << std::left << std::setw(10) << t.getAmount()
                              << std::setw(15) << t.getCategory()
                              << std::setw(25) << t.getDescription()
                              << t.getDateString() << "\n";
                }
            }

            // Pause before returning to menu
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.get();
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
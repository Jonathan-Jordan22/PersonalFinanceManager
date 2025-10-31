#include <iostream>
#include "Account.h"

int main() {
    Account checking("Checking Account", 1000.00);

    checking.addTransaction(-50.25, "Food", "Groceries");
    checking.addTransaction(500.00, "Income", "Paycheck");
    checking.addTransaction(-75.00, "Bills", "Electric bill");

    checking.printTransactions();

    return 0;
}

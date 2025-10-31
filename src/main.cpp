#include <iostream>
#include "FinanceManager.h"

int main() {
    FinanceManager fm;

    fm.addAccount("Checking", 1000.00);
    fm.addAccount("Savings", 2500.00);
    fm.addAccount("Emergency Fund", 500.00);

    fm.printAccountSummary();

    return 0;
}

#include <iostream>
#include <string>
#include <ctime>

#include "Transaction.h"
#include "Report.h"
#include "Account.h"
#include "Loan.h"
#include "User.h"
#include "App.h"
#include "Menu.h"

using namespace std;

// Lấy ngày hiện tại và chuyển sang string YYYY-MM-DD
string getToday() {
    time_t t = time(NULL);
    tm *timePtr = localtime(&t);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", timePtr);
    return string(buf);
}

int main() {
    App app;
    Menu menu(app);
    menu.run();
    return 0;
}

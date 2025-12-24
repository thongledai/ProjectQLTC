#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "App.h"
#include "User.h"
#include "Account.h"
#include "Loan.h"
#include "Report.h"

using namespace std;

string getToday();

// Class Menu: quản lý giao diện và menu tương tác với người dùng
class Menu {
private:
    App& app;
public:
    Menu(App& appRef);

    // Menu dành cho user chưa đăng nhập
    void showMainMenu();

    // Menu dành cho user đã đăng nhập (user hiện tại)
    void showUserMenu(const string& userName);
    void showAccountMenu();
    void showTransactionMenu();
    void showLoanMenu();
    void showReportMenu();

    // Run the menu loop
    void run();
};

#endif

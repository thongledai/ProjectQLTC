#include "Menu.h"

using namespace std;

Menu::Menu(App& appRef) : app(appRef) {}

void Menu::showMainMenu() {
    cout << "\n==== UNG DUNG QUAN LY TAI CHINH CA NHAN ====\n";
    cout << "1. Dang ky\n";
    cout << "2. Dang nhap\n";
    cout << "3. Thoat\n";
    cout << "Nhap lua chon: ";
}

void Menu::showUserMenu(const string& userName) {
    cout << "\n==== Main Menu (User: " << userName << ") ====\n";
    cout << "1. Add Account\n";
    cout << "2. List Accounts\n";
    cout << "3. Deposit to Account\n";
    cout << "4. Withdraw from Account\n";
    cout << "5. Transfer Between Accounts\n";
    cout << "6. List Transactions (for an Account)\n";
    cout << "7. Add Loan (Borrow/Lend)\n";
    cout << "8. List Loans\n";
    cout << "9. Record Loan Payment\n";
    cout << "10. Mark Loan as Paid\n";
    cout << "11. Generate Report (Income/Expense)\n";
    cout << "12. Export Data to CSV\n";
    cout << "13. Logout\n";
    cout << "Choose an option: ";
}

void Menu::run() {
    bool running = true;
    while (running) {
        if (app.getCurrentUser() == nullptr) {
            // Not logged in: show main menu
            showMainMenu();
            int choice;
            if (!(cin >> choice)) {
                // Handle non-integer input
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush leftover newline
            switch (choice) {
                case 1: { // Register new user
                    string name, email, pass;
                    cout << "Enter full name: ";
                    getline(cin, name);
                    cout << "Enter email: ";
                    cin >> email;
                    cout << "Enter password: ";
                    cin >> pass;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    app.registerUser(name, email, pass);
                    break;
                }
                case 2: { // Login existing user
                    string email, pass;
                    cout << "Enter email: ";
                    cin >> email;
                    cout << "Enter password: ";
                    cin >> pass;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    app.login(email, pass);
                    break;
                }
                case 3: // Exit application
                    cout << "Exiting application. Goodbye!" << endl;
                    running = false;
                    break;
                default:
                    cout << "Invalid choice. Please enter a number from the menu." << endl;
            }
        } else {
            // Logged in: show user menu
            string userName = app.getCurrentUser()->getFullName();
            showUserMenu(userName);
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            User* user = app.getCurrentUser();

            // (phần switch user menu của bạn giữ nguyên ở đây)
            // ... dán tiếp nguyên phần case 1..13 của bạn ...
        }
    }
}

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

// Danh sách các class
class Transaction;
class Account;
class Loan;
class Payment;
class Report;
class User;
class App;
class Menu;

// Class Menu: quản lý giao diện và menu tương tác với người dùng
class Menu {
private:
    App& app;
public:
    Menu(App& appRef) : app(appRef) {}

    // Menu dành cho user chưa đăng nhập
    void showMainMenu() {
        cout << "\n==== UNG DUNG QUAN LY TAI CHINH CA NHAN ====\n";
        cout << "1. Dang ky\n";
        cout << "2. Dang nhap\n";
        cout << "3. Thoat\n";
        cout << "Nhap lua chon: ";
    }

    // Menu dành cho user đã đăng nhập (user hiện tại)
    void showUserMenu(const string& userName) {
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

    // Run the menu loop
    void run() {
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
                switch (choice) {
                    case 1: { // Add Account
                        string accName;
                        double initBal;
                        cout << "Enter new account name: ";
                        getline(cin, accName);
                        cout << "Enter initial balance (0 if none): ";
                        cin >> initBal;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        user->addAccount(accName, initBal);
                        cout << "Account \"" << accName << "\" has been added." << endl;
                        break;
                    }
                    case 2: { // List Accounts
                        user->listAccounts();
                        break;
                    }
                    case 3: { // Deposit to Account
                        int accId;
                        double amt;
                        string title, category, note, date;
                        cout << "Enter Account ID to deposit into: ";
                        cin >> accId;
                        cout << "Enter amount to deposit: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter transaction title: ";
                        getline(cin, title);
                        cout << "Enter category (optional): ";
                        getline(cin, category);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        cout << "Enter date (YYYY-MM-DD, leave blank for today): ";
                        getline(cin, date);
                        if (date.empty()) date = getToday();
                        bool found = false;
                        for (Account* acc : user->getAccounts()) {
                            if (acc->getId() == accId) {
                                acc->deposit(amt, title, category, note, date);
                                cout << "Deposited " << amt << " to \"" << acc->getName() 
                                     << "\". New balance: " << acc->getBalance() << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Account ID " << accId << " not found." << endl;
                        }
                        break;
                    }
                    case 4: { // Withdraw from Account
                        int accId;
                        double amt;
                        string title, category, note, date;
                        cout << "Enter Account ID to withdraw from: ";
                        cin >> accId;
                        cout << "Enter amount to withdraw: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter transaction title: ";
                        getline(cin, title);
                        cout << "Enter category (optional): ";
                        getline(cin, category);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        cout << "Enter date (YYYY-MM-DD, leave blank for today): ";
                        getline(cin, date);
                        if (date.empty()) date = getToday();
                        bool found = false;
                        for (Account* acc : user->getAccounts()) {
                            if (acc->getId() == accId) {
                                if (acc->withdraw(amt, title, category, note, date)) {
                                    cout << "Withdrew " << amt << " from \"" << acc->getName() 
                                         << "\". New balance: " << acc->getBalance() << endl;
                                }
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Account ID " << accId << " not found or insufficient balance." << endl;
                        }
                        break;
                    }
                    case 5: { // Transfer between Accounts
                        int fromId, toId;
                        double amt;
                        string note;
                        cout << "Enter source Account ID: ";
                        cin >> fromId;
                        cout << "Enter destination Account ID: ";
                        cin >> toId;
                        cout << "Enter amount to transfer: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter note for transfer (optional): ";
                        getline(cin, note);
                        user->transfer(fromId, toId, amt, note);
                        break;
                    }
                    case 6: { // List Transactions for an Account
                        int accId;
                        cout << "Enter Account ID to view transactions: ";
                        cin >> accId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        bool found = false;
                        for (Account* acc : user->getAccounts()) {
                            if (acc->getId() == accId) {
                                acc->listTransactions();
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Account ID " << accId << " not found." << endl;
                        }
                        break;
                    }
                    case 7: { // Add Loan
                        int typeChoice;
                        string partner;
                        double principal, interest;
                        string startDate, dueDate, note;
                        cout << "Enter loan type (1 = BORROW (you owe), 2 = LEND (you lend out)): ";
                        cin >> typeChoice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        LoanType type = (typeChoice == 2 ? LoanType::LEND : LoanType::BORROW);
                        cout << "Enter partner name (lender/borrower): ";
                        getline(cin, partner);
                        cout << "Enter principal amount: ";
                        cin >> principal;
                        cout << "Enter annual interest rate (%): ";
                        cin >> interest;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter start date (YYYY-MM-DD): ";
                        getline(cin, startDate);
                        cout << "Enter due date (YYYY-MM-DD): ";
                        getline(cin, dueDate);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        user->addLoan(type, partner, principal, interest, startDate, dueDate, note);
                        cout << "Loan added successfully." << endl;
                        break;
                    }
                    case 8: { // List Loans
                        // Update overdue status for each loan before listing (based on current date)
                        string today = getToday();
                        for (Loan* loan : user->getLoans()) {
                            loan->isOverdue(today);
                        }
                        user->listLoans();
                        break;
                    }
                    case 9: { // Record Loan Payment
                        int loanId;
                        double amt;
                        string date, note;
                        cout << "Enter Loan ID to record a payment: ";
                        cin >> loanId;
                        cout << "Enter payment amount: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter payment date (YYYY-MM-DD): ";
                        getline(cin, date);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        bool found = false;
                        for (Loan* loan : user->getLoans()) {
                            if (loan->getId() == loanId) {
                                loan->addPayment(amt, date, note);
                                cout << "Payment recorded. Remaining balance: " << loan->getRemaining() << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Loan ID " << loanId << " not found." << endl;
                        }
                        break;
                    }
                    case 11: { // Generate Income/Expense Report
                        string fromDate, toDate;
                        cout << "Enter start date for report (YYYY-MM-DD, leave blank for no start limit): ";
                        getline(cin, fromDate);
                        cout << "Enter end date for report (YYYY-MM-DD, leave blank for no end limit): ";
                        getline(cin, toDate);
                        Report rep = user->generateReport(fromDate, toDate);
                        rep.display();
                        break;
                    }
                    case 12: { // Export Data to CSV
                        string filename;
                        cout << "Enter filename to export data (e.g., data.csv): ";
                        getline(cin, filename);
                        if (filename.empty()) filename = "export.csv";
                        app.exportDataCSV(filename);
                        break;
                    }
                    case 13: { // Logout
                        app.logout();
                        break;
                    }
                    default:
                        cout << "Invalid choice. Please enter a number from the menu." << endl;
                }
            }
        }
    }
};



#include "Menu.h"

#include <iostream>
#include <limits>
#include <string>

#include "App.h"
#include "User.h"
#include "Account.h"
#include "Loan.h"
#include "Report.h"

using namespace std;

string getToday();

Menu::Menu(App &appRef) : app(appRef) {}

void Menu::showMainMenu()
{
    cout << "\n==== Menu Dang Nhap ====\n";
    cout << "0. Xem Menu\n";
    cout << "1. Dang ky\n";
    cout << "2. Dang nhap\n";
    cout << "3. Thoat ung dung\n";
}

void Menu::showUserMenu(const string &userName)
{
    cout << "\n==== Menu Nguoi Dung - " << userName << " ====\n";
    cout << "0. Xem Menu\n";
    cout << "1. Them tai khoan\n";
    cout << "2. Danh sach tai khoan\n";
    cout << "3. Nap tien vao tai khoan\n";
    cout << "4. Rut tien tu tai khoan\n";
    cout << "5. Chuyen tien giua cac tai khoan\n";
    cout << "6. Xem danh sach giao dich (theo tai khoan)\n";
    cout << "7. Them khoan vay (Vay / Cho vay)\n";
    cout << "8. Danh sach cac khoan vay\n";
    cout << "9. Ghi nhan thanh toan khoan vay\n";
    cout << "10. Tao bao cao (Thu / Chi)\n";
    cout << "11. Xuat du lieu ra file CSV\n";
    cout << "12. Dang xuat\n";
}

void Menu::run()
{
    bool running = true;
    bool showMain = true;
    bool showUser = true;

    while (running)
    {
        if (app.getCurrentUser() == nullptr)
        {
            // CHƯA ĐĂNG NHẬP
            if (showMain)
            {
                showMainMenu();
                showMain = false;
            }
            cout << "\nNhap lua chon: ";
            int choice;
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 0:
            {
                showMainMenu();
                break;
            }
            case 1:
            {
                string name, email, pass;
                cout << "Nhap ho va ten:  ";
                getline(cin, name);
                cout << "Nhap email:  ";
                cin >> email;
                cout << "Nhap mat khau: ";
                cin >> pass;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                app.registerUser(name, email, pass);
                break;
            }
            case 2:
            {
                string email, pass;
                cout << "Nhap email: ";
                cin >> email;
                cout << "Nhap mat khau: ";
                cin >> pass;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                app.login(email, pass);
                break;
            }
            case 3:
                cout << "Thoat chuong trinh. Tam biet!" << endl;
                running = false;
                break;
            default:
                cout << "Lua chon khong hop le. Vui long nhap đung so trong menu." << endl;
            }
        }
        else
        {
            // ĐÃ ĐĂNG NHẬP
            string userName = app.getCurrentUser()->getFullName();

            if (showUser)
            {
                showUserMenu(userName);
                showUser = false;
            }
            cout << "\nNhap lua chon: ";
            int choice;
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            User *user = app.getCurrentUser();

            switch (choice)
            {
            case 0:
            {
                showUserMenu(userName);
                break;
            }
            case 1:
            {

                string accName;
                long initBal;
                cout << "Nhap ten tai khoan moi: ";
                getline(cin, accName);
                cout << "Nhap so du ban dau (0 neu khong co): ";
                cin >> initBal;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                int id;
                cout << "Nhap ID: ";
                cin >> id;

                Account *newAcc = user->addAccount(id, accName, initBal);
                if (newAcc != nullptr)
                {
                    cout << "Da them tai khoan \"" << accName << "\" thanh cong." << endl;
                }
                break;
            }

            case 2:
                user->listAccounts();
                break;

            case 3:
            {
                int accId;
                double amt;
                string title, category, note, date;

                cout << "Nhap ID tai khoan can nap tien: ";
                cin >> accId;
                cout << "Nhap so tien can nap: ";
                cin >> amt;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Nhap tieu de giao dich: ";
                getline(cin, title);
                cout << "Nhap danh muc (khong bat buoc): ";
                getline(cin, category);
                cout << "Nhap ghi chu (khong bat buoc): ";
                getline(cin, note);
                cout << "Nhap ngay (YYYY-MM-DD, bo trong = hom nay): ";
                getline(cin, date);
                if (date.empty())
                    date = getToday();

                bool found = false;
                for (Account *acc : user->getAccounts())
                {
                    if (acc->getId() == accId)
                    {
                        acc->deposit(title, amt, date, category, note);
                        cout << "Da nạp " << amt << " vao \"" << acc->getName()
                             << "\". So du moi: " << acc->getBalance() << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Khong tim thay tai khoan voi ID " << accId << "." << endl;
                }
                break;
            }

            case 4:
            {
                int accId;
                double amt;
                string title, category, note, date;

                cout << "Nhap ID tai khoan can rut tien: ";
                cin >> accId;
                cout << "Nhap so tien can rut: ";
                cin >> amt;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Nhap tieu de giao dich ";
                getline(cin, title);
                cout << "Nhap danh muc(khong bat buoc): ";
                getline(cin, category);
                cout << "Nhap ghi chu(khong bat buoc): ";
                getline(cin, note);
                cout << "Nhap ngay (YYYY-MM-DD, bo trong = hom nay): ";
                getline(cin, date);
                if (date.empty())
                    date = getToday();

                bool found = false;
                for (Account *acc : user->getAccounts())
                {
                    if (acc->getId() == accId)
                    {
                        acc->withdraw(title, amt, date, category, note);
                        cout << "Da rut " << amt << " tu \"" << acc->getName()
                             << "\". So du moi: " << acc->getBalance() << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Khong tim thay tai khoan hoac so du khong du" << endl;
                }
                break;
            }

            case 5:
            {
                int fromId, toId;
                double amt;
                string note;

                cout << "Nhap ID tai khoan nguon: ";
                cin >> fromId;
                cout << "Nhap ID tai khoan dich: ";
                cin >> toId;
                cout << "Nhap so tien can chuyen: ";
                cin >> amt;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Nhap ghi chu chuyen tien ( khong bat buoc ): ";
                getline(cin, note);

                user->transfer(fromId, toId, amt, note);
                break;
            }

            case 6:
            {
                int accId;
                cout << "Nhap ID tai khoan can xem giao dich: ";
                cin >> accId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                bool found = false;
                for (Account *acc : user->getAccounts())
                {
                    if (acc->getId() == accId)
                    {
                        auto a = acc->listTransactions();
                        for (auto x : a)
                        {
                            x->print();
                        }
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Khong tim thay tai khoan." << endl;
                }
                break;
            }

            case 7:
            {
                int typeChoice;
                string partner;
                double principal, interest;
                string startDate, dueDate, note;

                cout << "Chon loai khoan vay (1 = Vay, 2 = Cho vay): ";
                cin >> typeChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                LoanType type = (typeChoice == 2 ? LoanType::LEND : LoanType::BORROW);

                cout << "Nhap ten doi tac: ";
                getline(cin, partner);
                cout << "Nhap so tien goc: ";
                cin >> principal;
                cout << "Nhap lai suat (%/nam): ";
                cin >> interest;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Nhap ngay bat dau (YYYY-MM-DD): ";
                getline(cin, startDate);
                cout << "Nhap ngay den han (YYYY-MM-DD): ";
                getline(cin, dueDate);
                cout << "Nhap ghi chu (khong bat buoc): ";
                getline(cin, note);

                user->addLoan(type, partner, principal, interest, startDate, dueDate, note);
                cout << "Da them khoan vay thanh cong." << endl;
                break;
            }

            case 8:
            {
                string today = getToday();
                for (Loan *loan : user->getLoans())
                {
                    loan->isOverdue(today);
                }
                user->listLoans();
                break;
            }

            case 9:
            {
                int loanId;
                double amt;
                string date, note;

                cout << "Nhap ID khoan vay: ";
                cin >> loanId;
                cout << "Nhap so tien thanh toan: ";
                cin >> amt;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Nhap ngay thanh toan (YYYY-MM-DD): ";
                getline(cin, date);
                cout << "Nhap ghi chu (khong bat buoc): ";
                getline(cin, note);

                bool found = false;
                for (Loan *loan : user->getLoans())
                {
                    if (loan->getId() == loanId)
                    {
                        loan->addPayment(amt, date, note);
                        cout << "Da ghi nhan thanh toan. So du con lai: "
                             << loan->getRemaining() << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Khong tim thay khoan vay." << endl;
                }
                break;
            }

            case 10:
            {
                string fromDate, toDate;

                cout << "Nhap ngay bat dau bao cao (YYYY-MM-DD, bo trong neu khong gioi han): ";
                getline(cin, fromDate);

                while (true)
                {
                    cout << "Nhap ngay ket thuc bao cao (YYYY-MM-DD, bo trong neu khong gioi han): ";
                    getline(cin, toDate);

                    // Neu ca hai deu co gia tri thi moi so sanh
                    if (!fromDate.empty() && !toDate.empty())
                    {
                        if (toDate < fromDate)
                        {
                            cout << "Ngay ket thuc phai lon hon hoac bang ngay bat dau. Vui long nhap lai!\n";
                            continue;
                        }
                    }
                    break; // hop le
                }
                Report rep = user->generateReport(fromDate, toDate);
                rep.display();
                break;
            }

            case 11:
            {
                // chua code
            }

            case 12:
                app.logout();
                showMain = true;
                showUser = true;
                break;

            default:
                cout << "Lua chon khong hop le. Vui long nhap lai." << endl;
            }
        }
    }
}
//g++ Main.cpp App.cpp Menu.cpp Utils.cpp Account.cpp Loan.cpp Report.cpp Transaction.cpp User.cpp -o Main

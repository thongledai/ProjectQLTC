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

Menu::Menu(App& appRef) : app(appRef) {}

void Menu::showMainMenu() {
    cout << "\n==== ỨNG DỤNG QUẢN LÝ TÀI CHÍNH CÁ NHÂN ====\n";
    cout << "1. Đăng ký\n";
    cout << "2. Đăng nhập\n";
    cout << "3. Thoát\n";
    cout << "Nhập lựa chọn: ";
}

void Menu::showUserMenu(const string& userName) {
    cout << "\n==== MENU CHÍNH (Người dùng: " << userName << ") ====\n";
    cout << "1. Thêm tài khoản\n";
    cout << "2. Danh sách tài khoản\n";
    cout << "3. Nạp tiền vào tài khoản\n";
    cout << "4. Rút tiền từ tài khoản\n";
    cout << "5. Chuyển tiền giữa các tài khoản\n";
    cout << "6. Xem danh sách giao dịch (theo tài khoản)\n";
    cout << "7. Thêm khoản vay (Vay / Cho vay)\n";
    cout << "8. Danh sách các khoản vay\n";
    cout << "9. Ghi nhận thanh toán khoản vay\n";
    cout << "10. Tạo báo cáo (Thu / Chi)\n";
    cout << "11. Xuất dữ liệu ra file CSV\n";
    cout << "12. Đăng xuất\n";
    cout << "Chọn chức năng: ";
}

void Menu::run() {
    bool running = true;

    while (running) {
        if (app.getCurrentUser() == nullptr) {
            // ===== CHƯA ĐĂNG NHẬP =====
            showMainMenu();

            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: {
                    string name, email, pass;
                    cout << "Nhập họ và tên: ";
                    getline(cin, name);
                    cout << "Nhập email: ";
                    cin >> email;
                    cout << "Nhập mật khẩu: ";
                    cin >> pass;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    app.registerUser(name, email, pass);
                    break;
                }
                case 2: {
                    string email, pass;
                    cout << "Nhập email: ";
                    cin >> email;
                    cout << "Nhập mật khẩu: ";
                    cin >> pass;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    app.login(email, pass);
                    break;
                }
                case 3:
                    cout << "Thoát chương trình. Tạm biệt!" << endl;
                    running = false;
                    break;
                default:
                    cout << "Lựa chọn không hợp lệ. Vui lòng nhập đúng số trong menu." << endl;
            }

        } else {
            // ===== ĐÃ ĐĂNG NHẬP =====
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
                case 1: {

                    string accName;
                    long initBal;
                    cout << "Nhập tên tài khoản mới: ";
                    getline(cin, accName);
                    cout << "Nhập số dư ban đầu (0 nếu không có): ";
                    cin >> initBal;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    int id;
                    cout<< "Nhập ID: ";
                    cin>>id;

                    user->addAccount(id,accName, initBal);
                    cout << "Đã thêm tài khoản \"" << accName << "\" thành công." << endl;
                    break;
                }

                case 2:
                    user->listAccounts();
                    break;

                case 3: {
                    int accId;
                    double amt;
                    string title, category, note, date;

                    cout << "Nhập ID tài khoản cần nạp tiền: ";
                    cin >> accId;
                    cout << "Nhập số tiền cần nạp: ";
                    cin >> amt;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Nhập tiêu đề giao dịch: ";
                    getline(cin, title);
                    cout << "Nhập danh mục (không bắt buộc): ";
                    getline(cin, category);
                    cout << "Nhập ghi chú (không bắt buộc): ";
                    getline(cin, note);
                    cout << "Nhập ngày (YYYY-MM-DD, bỏ trống = hôm nay): ";
                    getline(cin, date);
                    if (date.empty()) date = getToday();

                    bool found = false;
                    for (Account* acc : user->getAccounts()) {
                        if (acc->getId() == accId) {
                            acc->deposit(title,amt, date,category, note);
                            cout << "Đã nạp " << amt << " vào \"" << acc->getName()
                                 << "\". Số dư mới: " << acc->getBalance() << endl;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Không tìm thấy tài khoản với ID " << accId << "." << endl;
                    }
                    break;
                }

                case 4: {
                    int accId;
                    double amt;
                    string title, category, note, date;

                    cout << "Nhập ID tài khoản cần rút tiền: ";
                    cin >> accId;
                    cout << "Nhập số tiền cần rút: ";
                    cin >> amt;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Nhập tiêu đề giao dịch: ";
                    getline(cin, title);
                    cout << "Nhập danh mục (không bắt buộc): ";
                    getline(cin, category);
                    cout << "Nhập ghi chú (không bắt buộc): ";
                    getline(cin, note);
                    cout << "Nhập ngày (YYYY-MM-DD, bỏ trống = hôm nay): ";
                    getline(cin, date);
                    if (date.empty()) date = getToday();

                    bool found = false;
                    for (Account* acc : user->getAccounts()) {
                        if (acc->getId() == accId) {
                            acc->withdraw(title,amt,date,category, note);
                                cout << "Đã rút " << amt << " từ \"" << acc->getName()
                                     << "\". Số dư mới: " << acc->getBalance() << endl;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Không tìm thấy tài khoản hoặc số dư không đủ." << endl;
                    }
                    break;
                }

                case 5: {
                    int fromId, toId;
                    double amt;
                    string note;

                    cout << "Nhập ID tài khoản nguồn: ";
                    cin >> fromId;
                    cout << "Nhập ID tài khoản đích: ";
                    cin >> toId;
                    cout << "Nhập số tiền cần chuyển: ";
                    cin >> amt;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Nhập ghi chú chuyển tiền (không bắt buộc): ";
                    getline(cin, note);

                    user->transfer(fromId, toId, amt, note);
                    break;
                }

                case 6: {
                    int accId;
                    cout << "Nhập ID tài khoản cần xem giao dịch: ";
                    cin >> accId;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    bool found = false;
                    for (Account* acc : user->getAccounts()) {
                        if (acc->getId() == accId) {
                            auto a = acc->listTransactions();
                            for (auto x: a) {
                                x->print();
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Không tìm thấy tài khoản." << endl;
                    }
                    break;
                }

                case 7: {
                    int typeChoice;
                    string partner;
                    double principal, interest;
                    string startDate, dueDate, note;

                    cout << "Chọn loại khoản vay (1 = Vay, 2 = Cho vay): ";
                    cin >> typeChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    LoanType type = (typeChoice == 2 ? LoanType::LEND : LoanType::BORROW);

                    cout << "Nhập tên đối tác: ";
                    getline(cin, partner);
                    cout << "Nhập số tiền gốc: ";
                    cin >> principal;
                    cout << "Nhập lãi suất (%/năm): ";
                    cin >> interest;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Nhập ngày bắt đầu (YYYY-MM-DD): ";
                    getline(cin, startDate);
                    cout << "Nhập ngày đến hạn (YYYY-MM-DD): ";
                    getline(cin, dueDate);
                    cout << "Nhập ghi chú (không bắt buộc): ";
                    getline(cin, note);

                    user->addLoan(type, partner, principal, interest, startDate, dueDate, note);
                    cout << "Đã thêm khoản vay thành công." << endl;
                    break;
                }

                case 8: {
                    string today = getToday();
                    for (Loan* loan : user->getLoans()) {
                        loan->isOverdue(today);
                    }
                    user->listLoans();
                    break;
                }

                case 9: {
                    int loanId;
                    double amt;
                    string date, note;

                    cout << "Nhập ID khoản vay: ";
                    cin >> loanId;
                    cout << "Nhập số tiền thanh toán: ";
                    cin >> amt;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Nhập ngày thanh toán (YYYY-MM-DD): ";
                    getline(cin, date);
                    cout << "Nhập ghi chú (không bắt buộc): ";
                    getline(cin, note);

                    bool found = false;
                    for (Loan* loan : user->getLoans()) {
                        if (loan->getId() == loanId) {
                            loan->addPayment(amt, date, note);
                            cout << "Đã ghi nhận thanh toán. Số dư còn lại: "
                                 << loan->getRemaining() << endl;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Không tìm thấy khoản vay." << endl;
                    }
                    break;
                }

                case 10: {
                    string fromDate, toDate;
                    cout << "Nhập ngày bắt đầu báo cáo (YYYY-MM-DD, bỏ trống nếu không giới hạn): ";
                    getline(cin, fromDate);
                    cout << "Nhập ngày kết thúc báo cáo (YYYY-MM-DD, bỏ trống nếu không giới hạn): ";
                    getline(cin, toDate);

                    Report rep = user->generateReport(fromDate, toDate);
                    rep.display();
                    break;
                }

                case 11: {
                    string filename;
                    cout << "Nhập tên file xuất dữ liệu (ví dụ: data.csv): ";
                    getline(cin, filename);
                    if (filename.empty()) filename = "export.csv";

                    app.exportDataCSV(filename);
                    break;
                }

                case 12:
                    app.logout();
                    break;

                default:
                    cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại." << endl;
            }
        }
    }
}

#include "Menu.h"

#include <iostream>
#include <limits>
#include <string>

#include "App.h"
#include "User.h"
#include "Account.h"
#include "Loan.h"
#include "Report.h"
#include "FileRepository.h"
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
    cout << "\n===== MENU NGUOI DUNG - " << userName << " =====\n";
    cout << "0. Xem menu\n";
    cout << "1. Tai khoan\n";
    cout << "2. Giao dich\n";
    cout << "3. Khoan vay\n";
    cout << "4. Bao cao\n";
    cout << "5. Dang xuat\n";
}
void Menu::showAccountMenu()
{
    cout << "\n===== MENU TAI KHOAN =====\n";
    cout << "0. Xem menu\n";
    cout << "1. Them tai khoan\n";
    cout << "2. Sua thong tin tai khoan (chua ho tro)\n";
    cout << "3. Xoa tai khoan (chua ho tro)\n";
    cout << "4. Xem danh sach tai khoan\n";
    cout << "5. Thoat\n";
}
void Menu::showTransactionMenu()
{
    cout << "\n===== MENU GIAO DICH =====\n";
    cout << "0. Xem menu\n";
    cout << "1. Nap tien vao tai khoan\n";
    cout << "2. Rut tien tu tai khoan\n";
    cout << "3. Chuyen tien giua cac tai khoan\n";
    cout << "4. Chuyen tien cho nguoi khac\n";
    cout << "5. Xem danh sach giao dich\n";
    cout << "6. Thoat\n";
}
void Menu::showLoanMenu()
{
    cout << "\n===== MENU KHOAN VAY =====\n";
    cout << "0. Xem menu\n";
    cout << "1. Them khoan vay (Vay / Cho vay)\n";
    cout << "2. Thanh toan khoan vay\n";
    cout << "3. Xem danh sach cac khoan vay\n";
    cout << "4. Xem lich su thanh toan\n";
    cout << "5. Thoat\n";
}
void Menu::showReportMenu()
{
    cout << "\n===== MENU BAO CAO =====\n";
    cout << "0. Xem menu\n";
    cout << "1. Bao cao thu chi tai khoan\n";
    cout << "2. Bao cao thu chi user (chua ho tro)\n";
    cout << "3. Thoat\n";
}

void Menu::run()
{
    bool running = true;
    bool showMain = true;
    bool showUser = true;
    int mainChoice = -1;
    int subChoice = -1;

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
            // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (choice)
            {
            case 0:
            {
                showMainMenu();
                break;
            }
            case 1:
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // <-- Chỉ cần gọi một lần trước khi bắt đầu nhập

                string name, email, pass;

                cout << "Nhap ho va ten: ";
                getline(cin, name); // OK

                cout << "Nhap email: ";
                getline(cin, email); // OK

                cout << "Nhap mat khau: ";
                getline(cin, pass);

                app.registerUser(name, email, pass);
                break;
            }
            case 2:
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string email, pass;
                cout << "Nhap email: ";
                getline(cin, email);
                cout << "Nhap mat khau: ";
                getline(cin, pass);
                // app.login(email, pass);
                if (app.login(email, pass));
                {
                    User *currUser = app.getCurrentUser();
                    currUser->setAccounts(FileRepository::loadAccountsByUser(currUser->getId()));
                }
                break;
            }
            case 3:
            {
                cout << "Thoat chuong trinh. Tam biet!" << endl;
                running = false;
                break;
            default:
                cout << "Lua chon khong hop le. Vui long nhap đung so trong menu." << endl;
            }
            }
        }
        else
        {
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
            case 1: // ===== TAI KHOAN =====
            {
                showAccountMenu();
                cout << "Nhap lua chon: ";
                cin >> subChoice;
                cin.ignore();

                switch (subChoice)
                {
                case 0:
                    showAccountMenu();
                    break;
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
                {
                    
                }
                case 3:
                    // xóa tk
                    break;
                case 4:
                {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    string email, pass;
                    cout << "Nhap email: ";
                    getline(cin, email);
                    cout << "Nhap mat khau: ";
                    getline(cin, pass);
                    app.login(email, pass);
                    break;
                }

                case 5:
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
                }
                break;
            }
            case 2: // ===== GIAO DICH =====
            {
                showTransactionMenu();
                cout << "Nhap lua chon: ";
                cin >> subChoice;
                cin.ignore();

                switch (subChoice)
                {
                case 0:
                    showTransactionMenu();
                    break;
                case 1:
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
                case 2:
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
                case 3:
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
                case 4:
                {
                    User *sender = app.getCurrentUser();
                    cout << "Danh sach tai khoan cua ban:\n";
                    sender->listAccountsBrief();

                    int fromId;
                    cout << "Chon ID tai khoan chuyen tien: ";
                    cin >> fromId;
                    cin.ignore();
                    Account *FromAcc = sender->findAccountById(fromId);
                    if (FromAcc == nullptr)
                    {
                        cout << "khong ton tai tai khoan chuyen tien\n";
                        break;
                    }
                    string email;
                    cout << "Nhap email nguoi nhan: ";
                    getline(cin, email);

                    User *FromUser = app.findUserByEmail(email);
                    if (FromUser == nullptr)
                    {
                        cout << "Khong ton tai email nguoi nhan\n";
                        break;
                    }
                    cout << "Danh sach tai khoan nguoi nhan:\n";
                    FromUser->listAccountsBrief();
                    int toId;
                    cout << "Chon ID tai khoan nhan: ";
                    cin >> toId;
                    cin.ignore();
                    Account *ToAcc = FromUser->findAccountById(toId);
                    if (ToAcc == nullptr)
                    {

                        cout << "khong ton tai tai khoan nguoi nhan\n";
                        break;
                    }
                    long amount;
                    cout << "Nhap so tien: ";
                    cin >> amount;
                    cin.ignore();

                    string note;
                    cout << "Ghi chu: ";
                    getline(cin, note);

                    if (app.transferUser(fromId, email, toId, amount, note))
                        cout << "Chuyen tien thanh cong!\n";
                    else
                        cout << "Chuyen tien that bai!\n";

                    break;
                }

                case 5:
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
                            auto a = acc->getTransactions();
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

                case 6:
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
                }
                break;
            }
            case 3: // ===== KHOAN VAY =====
            {
                showLoanMenu();
                cout << "Nhap lua chon: ";
                cin >> subChoice;
                cin.ignore();

                switch (subChoice)
                {
                case 0:
                    showLoanMenu();
                    break;
                case 1:
                {
                    User *A = app.getCurrentUser();
                    if (!A)
                    {
                        cout << "Ban chua dang nhap!\n";
                        break;
                    }

                    int typeChoice;
                    cout << "Chon loai (1.Vay, 2.Cho vay): ";
                    cin >> typeChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    string partnerEmail;
                    cout << "Nhap email doi tac: ";
                    getline(cin, partnerEmail);

                    User *B = app.findUserByEmail(partnerEmail);
                    if (!B || B->getId() == A->getId())
                    {
                        cout << "Email doi tac khong hop le!\n";
                        break;
                    }

                    // ====== chọn account nguồn/đích tùy loại ======
                    int fromId, toId;
                    long amount;
                    double interest;
                    string startDate, dueDate, note;

                    if (typeChoice == 1)
                    {
                        // ===== A VAY B: tiền B -> A =====
                        cout << "\n[Nguoi vay] Chon tai khoan NHAN tien:\n";
                        A->listAccountsBrief();
                        cout << "Nhap ID tai khoan nhan: ";
                        cin >> toId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (A->findAccountById(toId) == nullptr)
                        {
                            cout << "Khong ton tai tai khoan nhan!\n";
                            break;
                        }

                        cout << "\n[Nguoi cho vay] Chon tai khoan GUI tien:\n";
                        B->listAccountsBrief();
                        cout << "Nhap ID tai khoan gui: ";
                        cin >> fromId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (B->findAccountById(fromId) == nullptr)
                        {
                            cout << "Khong ton tai tai khoan gui!\n";
                            break;
                        }
                    }
                    else if (typeChoice == 2)
                    {
                        // ===== A CHO B VAY: tiền A -> B =====
                        cout << "\n[Nguoi cho vay] Chon tai khoan GUI tien:\n";
                        A->listAccountsBrief();
                        cout << "Nhap ID tai khoan gui: ";
                        cin >> fromId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (A->findAccountById(fromId) == nullptr)
                        {
                            cout << "Khong ton tai tai khoan gui!\n";
                            break;
                        }
                        cout << "\n[Nguoi vay] Chon tai khoan NHAN tien:\n";
                        B->listAccountsBrief();
                        cout << "Nhap ID tai khoan nhan: ";
                        cin >> toId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (B->findAccountById(toId) == nullptr)
                        {
                            cout << "Khong ton tai tai khoan nhan!\n";
                            break;
                        }
                    }
                    else
                    {
                        cout << "Lua chon khong hop le!\n";
                        break;
                    }
                    // ===== nhập thông tin khoản vay =====
                    cout << "Nhap so tien goc: ";
                    cin >> amount;
                    cout << "Nhap lai suat (%): ";
                    cin >> interest;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Nhap ngay bat dau (YYYY-MM-DD): ";
                    getline(cin, startDate);
                    cout << "Nhap ngay den han (YYYY-MM-DD): ";
                    getline(cin, dueDate);
                    cout << "Nhap ghi chu (khong bat buoc): ";
                    getline(cin, note);

                    // ====== 1) Chuyển tiền (tái dùng hàm chuyển khoản) ======
                    bool okTransfer = false;
                    if (typeChoice == 1)
                    {
                        // B -> A (phải chỉ định sender là B)
                        okTransfer = app.transferFromUser(
                            B, fromId, A->getEmail(), toId, amount,
                            "Giai ngan vay: " + note);
                    }
                    else
                    {
                        // A -> B (dùng hàm cũ luôn)
                        okTransfer = app.transferUser(
                            fromId, B->getEmail(), toId, amount,
                            "Cho vay: " + note);
                    }

                    if (!okTransfer)
                    {
                        cout << "Chuyen tien that bai!\n";
                        break;
                    }
                    // ====== 2) Tự động tạo khoản vay cho cả 2 ======
                    if (typeChoice == 1)
                    {
                        // A vay B
                        A->addLoan(LoanType::BORROW, B->getEmail(), amount, interest, startDate, dueDate, note);

                        string noteB = "Cho vay: " + A->getEmail();
                        if (!note.empty())
                            noteB += " | " + note;
                        B->addLoan(LoanType::LEND, A->getEmail(), amount, interest, startDate, dueDate, noteB);
                    }
                    else
                    {
                        // A cho B vay
                        A->addLoan(LoanType::LEND, B->getEmail(), amount, interest, startDate, dueDate, note);

                        string noteB = "Vay tu: " + A->getEmail();
                        if (!note.empty())
                            noteB += " | " + note;
                        B->addLoan(LoanType::BORROW, A->getEmail(), amount, interest, startDate, dueDate, noteB);
                    }

                    cout << "Thanh cong! Da chuyen tien va tao khoan vay cho ca 2 ben.\n";
                    break;
                }

                case 2:
                {
                    User *A = app.getCurrentUser();
                    if (!A)
                    {
                        cout << "Ban chua dang nhap!\n";
                        break;
                    }

                    int loanId;
                    long amt;
                    string date, note;

                    cout << "Nhap ID khoan vay: ";
                    cin >> loanId;

                    Loan *loanA = A->findLoanById(loanId);
                    if (!loanA)
                    {
                        cout << "Khong tim thay khoan vay.\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }

                    cout << "Nhap so tien thanh toan: ";
                    cin >> amt;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    date = getToday();
                    cout << "Nhap ghi chu (khong bat buoc): ";
                    getline(cin, note);

                    // partnerEmail là email đối tác
                    string partnerEmail = loanA->getPartnerEmail();
                    User *B = app.findUserByEmail(partnerEmail);
                    if (!B)
                    {
                        cout << "Khong tim thay user doi tac (email): " << partnerEmail << "\n";
                        break;
                    }
                    // ===== chọn tài khoản de TRẢ =====
                    cout << "\nChon tai khoan tra no:\n";
                    A->listAccountsBrief();
                    int fromA;
                    cout << "Nhap ID tai khoan tra (A): ";
                    cin >> fromA;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (A->findAccountById(fromA) == nullptr)
                    {
                        cout << "Khong ton tai tai khoan tra cua A!\n";
                        break;
                    }
                    // ===== chọn tài khoản để NHẬN =====
                    cout << "\nChon tai khoan nhan tien:\n";
                    B->listAccountsBrief();
                    int toB;
                    cout << "Nhap ID tai khoan nhan (B): ";
                    cin >> toB;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (B->findAccountById(toB) == nullptr)
                    {
                        cout << "Khong ton tai tai khoan nhan cua B!\n";
                        break;
                    }
                    // ===== 1) chuyển tiền A -> B để cập nhật số dư =====
                    bool okTransfer = app.transferUser(fromA, B->getEmail(), toB, amt, "Thanh toan vay: " + note);
                    if (!okTransfer)
                    {
                        cout << "Chuyen tien thanh toan that bai!\n";
                        break;
                    }
                    // ===== 2) cập nhật payment cho loan của A =====
                    loanA->addPayment(amt, date, note);

                    // ===== 3) cập nhật loan đối ứng của B =====
                    LoanType needTypeB = (loanA->getType() == LoanType::BORROW) ? LoanType::LEND : LoanType::BORROW;
                    Loan *loanB = B->findMatchingLoan(
                        needTypeB,
                        A->getEmail(), // partner của B chính là email A
                        loanA->getPrincipal(),
                        loanA->getStartDate(),
                        loanA->getDueDate());

                    if (loanB)
                    {
                        string noteB = "Nhan thanh toan: " + A->getEmail();
                        if (!note.empty())
                            noteB += " | " + note;
                        loanB->addPayment(amt, date, noteB);
                    }
                    else
                    {
                        cout << "Canh bao: Khong tim thay khoan vay doi ung ben B de cap nhat payment.\n";
                    }

                    cout << "Da thanh toan. Con lai (A): " << loanA->getRemaining() << "\n";
                    break;
                }

                case 3:
                {
                    string today = getToday();
                    for (Loan *loan : user->getLoans())
                    {
                        loan->isOverdue(today);
                    }
                    user->listLoans();
                    break;
                }

                case 10:
                {
                    User *user = app.getCurrentUser();
                    if (!user)
                    {
                        cout << "Ban chua dang nhap!\n";
                        break;
                    }

                    int loanId;
                    cout << "Nhap ID khoan vay can xem lich su thanh toan: ";
                    cin >> loanId;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    bool found = false;
                    for (Loan *loan : user->getLoans())
                    {
                        if (loan->getId() == loanId)
                        {
                            loan->showPaymentHistory();
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        cout << "Khong tim thay khoan vay voi ID nay.\n";
                    }
                    break;
                }

                case 5:
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
                }
                break;
            }
            case 4: // ===== BAO CAO =====
            {
                showReportMenu();
                cout << "Nhap lua chon: ";
                cin >> subChoice;
                cin.ignore();

                switch (subChoice)
                {
                case 0:
                    showReportMenu();
                    break;
                case 1:
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

                case 2:
                    // BÁO CÁO THU CHI USER
                case 3:
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
                }
                break;
            }
            case 5:
            {
                app.logout();
                showMain = true;
                showUser = true;
                break;
            }
            }
        }
    }
}

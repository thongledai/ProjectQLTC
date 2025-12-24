#include <iostream>
#include <vector>
#include <string>

#include "Transaction.h"
#include "Report.h"
#include "Loan.h"
#include "Account.h"
#include "User.h"
#include "App.h"
using namespace std;

int User::nextId = 0;

// Class User: đại diện cho một người người dùng của hệ thống(có tài khoản và khoản)
User::User(const string &fullName, const string &email, const string &password)
{
    this->id = ++nextId;
    this->fullName = fullName;
    this->email = email;
    this->password = password;
}

// Giải phóng toàn bộ tài nguyên động mà User sở hữu
User::~User()
{
    for (Account *acc : accounts)
        delete acc;
    for (Loan *loan : loans)
        delete loan;
}

// ===== Getter / Setter =====
int User::getId() const { return id; }
string User::getFullName() const { return fullName; }
string User::getEmail() const { return email; }
string User::getPassword() const { return password; }
const vector<Account *> &User::getAccounts() const { return accounts; }
const vector<Loan *> &User::getLoans() const { return loans; }

void User::setId(int ID) { this->id = ID; }
void User::setFullName(string fullname) { this->fullName = fullName; }
void User::setEmail(string email) { this->email = email; }
void User::addAccount(Account* acc) { this->accounts.push_back(acc); }
void User::addLoan(Loan* l) { this->loans.push_back(l); }

// Kiểm tra xác thực mật khẩu
bool User::checkPassword(const string &pw) const
{
    return pw == password;
}

// ===== Account =====

// Thêm tài khoản mới cho User
Account *User::addAccount(const int &id, const string &name, long initialBalance)
{
    for (Account *acc : accounts)
    {
        if (acc->getId() == id)
        {
            cout << "ID da ton tai, vui long nhap lai." << endl;
            return nullptr; // trả về nullptr báo không thêm được
        }
    }
    Account *account = new Account(id, name, initialBalance);
    accounts.push_back(account);
    return account;
}

// Xóa tài khoản theo ID
bool User::removeAccount(int accountId)
{
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i]->getId() == accountId)
        {
            delete accounts[i];
            accounts.erase(accounts.begin() + i);
            return true;
        }
    }
    return false;
}

// Đổi tên tài khoản hiện có theo ID
bool User::renameAccount(int accountId, const string &newName)
{
    for (Account *acc : accounts)
    {
        if (acc->getId() == accountId)
        {
            acc->setName(newName);
            return true;
        }
    }
    return false;
}

// Chuyển tiền nội bộ giữa các tài khoản của cùng User
bool User::transfer(int fromAccountId, int toAccountId, long amount, const string &note)
{
    if (fromAccountId == toAccountId)
    {
        cout << "khong the chuyen tien trong cung 1 tai khoan" << endl;
        return false;
    }
    Account *fromAcc = nullptr;
    Account *toAcc = nullptr;
    for (Account *acc : accounts)
    {
        if (acc->getId() == fromAccountId)
            fromAcc = acc;
        if (acc->getId() == toAccountId)
            toAcc = acc;
    }
    if (fromAcc == nullptr || toAcc == nullptr)
    {
        cout << "khong tim thay tai khoan " << endl;
        return false;
    }
    if (amount < 0)
    {
        cout << "so tien phai la so duong" << endl;
        return false;
    }
    if (amount > fromAcc->getBalance())
    {
        cout << "so du trong tai khoan nguon khong du" << endl;
        return false;
    }
    string today = getToday();
    // Ghi nhận giao dịch chuyển tiền ở cả hai phía
    fromAcc->withdraw("chuyen tien den " + toAcc->getName(), amount, today, " 0chuyen khoan", note);
    toAcc->deposit("chuyen tien tu " + fromAcc->getName(), amount, today, " chuyen khoan", note);
    cout << "da chuyen tien " << amount << " tu \"" << fromAcc->getName() << "\" den \"" << toAcc->getName() << "\"." << endl;
    return true;
}

// Chuyển tiền giữa các User
bool User::transferToOtherUser(int fromAccountId, User *receiver, int toAccountId, long amount, const string &note)
{
    if (receiver == nullptr)
    {
        cout << "Nguoi nhan khong hop le." << endl;
        return false;
    }

    if (this == receiver)
    {
        cout << "Dung dung phuong thuc transfer, su dung chuyen tien noi bo." << endl;
        return false;
    }

    if (amount <= 0)
    {
        cout << "So tien phai la so duong." << endl;
        return false;
    }

    // Tìm tài khoản nguồn của User hiện tại
    Account *fromAcc = nullptr;
    for (Account *acc : accounts)
    {
        if (acc->getId() == fromAccountId)
        {
            fromAcc = acc;
            break;
        }
    }
    if (fromAcc == nullptr)
    {
        cout << "Khong tim thay tai khoan nguon cua ban." << endl;
        return false;
    }

    if (amount > fromAcc->getBalance())
    {
        cout << "So du trong tai khoan nguon khong du." << endl;
        return false;
    }

    // Tìm tài khoản đích của User nhận
    Account *toAcc = nullptr;
    for (Account *acc : receiver->getAccounts())
    {
        if (acc->getId() == toAccountId)
        {
            toAcc = acc;
            break;
        }
    }
    if (toAcc == nullptr)
    {
        cout << "Khong tim thay tai khoan nhan trong User nhan." << endl;
        return false;
    }

    string today = getToday();

    // Ghi nhận giao dịch rút tiền ở User gửi
    fromAcc->withdraw("Chuyen tien den " + toAcc->getName() + " (User: " + receiver->getFullName() + ")",
                      amount, today, "chuyen khoan ngoai", note);

    // Ghi nhận giao dịch gửi tiền ở User nhận
    toAcc->deposit("Chuyen tien tu " + fromAcc->getName() + " (User: " + fullName + ")",
                   amount, today, "chuyen khoan ngoai", note);

    cout << "Da chuyen " << amount << " tu tai khoan \"" << fromAcc->getName() << "\" cua User \""
         << fullName << "\" den tai khoan \"" << toAcc->getName() << "\" cua User \"" << receiver->getFullName() << "\"." << endl;

    return true;
}

// ===== Loan =====

Loan *User::addLoan(LoanType type, const string &partnerName, long principal, long interestRate,
                    const string &startDate, const string &dueDate, const string &note)
{
    Loan *loan = new Loan(type, partnerName, principal, interestRate, startDate, dueDate, note);
    loans.push_back(loan);
    return loan;
}

// Cập nhật thông tin khoản vay (lãi suất hoặc ngày đến hạn)
bool User::updateLoan(int loanId, long newInterestRate, const string &newDueDate)
{
    for (Loan *loan : loans)
    {
        if (loan->getId() == loanId)
        {
            if (newInterestRate >= 0)
                loan->setInterestRate(newInterestRate);
            if (!newDueDate.empty())
                loan->setDueDate(newDueDate);
            return true;
        }
    }
    return false;
}
Loan* User::findLoanById(int loanId) const {
    for (Loan* l : loans) {
        if (l && l->getId() == loanId) return l;
    }
    return nullptr;
}

// match đối ứng(partnerEmail + type)
Loan* User::findMatchingLoan(LoanType type, const string& partnerEmail,
                            long principal, const string& startDate, const string& dueDate) const {
    for (Loan* l : loans) {
        if (!l) continue;
        if (l->getType() == type &&
            l->getPartnerEmail() == partnerEmail &&
            l->getPrincipal() == principal &&
            l->getStartDate() == startDate &&
            l->getDueDate() == dueDate) {
            return l;
        }
    }
    return nullptr;
}



// ===== Báo cáo =====
// Tổng hợp toàn bộ giao dịch từ các Account để sinh Report
Report User::generateReport(const string &fromDate, const string &toDate)
{
    vector<Transaction *> allTx;
    for (Account *acc : accounts)
    {
        vector<Transaction *> rangeTx = acc->getTransactions();
        allTx.insert(allTx.end(), rangeTx.begin(), rangeTx.end());
    }
    Report report(fromDate, toDate);
    report.build(allTx);
    return report;
}

// Tổng số dư của toàn bộ tài khoản
long User::getTotalBalance() const
{
    long total = 0;
    for (Account *acc : accounts)
    {
        total += acc->getBalance();
    }
    return total;
}

// danh sách Account (id, ten tai khoan, so du)
void User::listAccounts() const
{
    if (accounts.empty())
    {
        cout << "Danh sach rong" << endl;
    }
    else
    {
        cout << "Tai khoan cho nguoi dung \"" << fullName << "\":" << endl;
        for (Account *acc : accounts)
        {
            cout << "[AccountID " << acc->getId() << "] "
                 << acc->getName() << " So du: " << acc->getBalance() << endl;
        }
    }
}
// Liệt kê tất cả các khoản vay
void User::listLoans() const {
    if (loans.empty()) {
        cout << "Khong co khoan vay nao" << endl;
    } else {
        cout << "  Danh sach cac khoan vay - \"" << fullName << "\":" << endl;
        for (Loan* loan : loans) {
         // Kiểm tra quá hạn phục vụ hiển thị
            string statusStr = loanStatusToString(loan->getStatus());
            cout << "  [LoanID " << loan->getId() << "] | "
                 << (loan->getType() == LoanType::BORROW ? "Vay " : "Cho Vay ")
                 <<"\""<< loan->getPartnerEmail()<<"\""
                 << " | Tien goc: " << loan->getPrincipal()
                 << " | Lai xuat: " << loan->getInterestRate()
                 << " | Tong no: " << loan->getDueTotal()
                 << " | Da tra: " << loan->getPaidTotal()
                 << " | Con no: " << loan->getRemaining()
                 << " | Tinh trang: " << statusStr << endl;
        }
    }
}


// Hàm tìm tài khoản theo ID
Account* User::findAccountById(int accountId) const
{
    for (Account* acc : accounts)
    {
        if (acc->getId() == accountId)
        {
            return acc; // tìm thấy tài khoản, trả về con trỏ
        }
    }
    return nullptr; // không tìm thấy trả về nullptr
}
// danh sách Account (id, ten tai khoan)
void User::listAccountsBrief() const
{
    if (accounts.empty())
    {
        cout << "Danh sach rong" << endl;
    }
    else
    {
        cout << "Tai khoan cho nguoi dung \"" << fullName << "\":" << endl;
        for (Account *acc : accounts)
        {
            cout << "[AccountID " << acc->getId() << "] "
                 << acc->getName() << endl;
        }
    }
}

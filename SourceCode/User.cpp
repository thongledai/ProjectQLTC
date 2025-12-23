#include <iostream>
#include <vector>
#include <string>

#include "Transaction.h"
#include "Report.h"
#include "Loan.h"
#include "Account.h"
#include "User.h"

using namespace std;

int User::nextId = 0;

// Class User: đại diện cho một người người dùng của hệ thống(có tài khoản và khoản)
User::User(const string& fullName, const string& email, const string& password) {
    this->id = ++nextId;
    this->fullName = fullName;
    this->email = email;
    this->password = password;
}

// Giải phóng toàn bộ tài nguyên động mà User sở hữu
User::~User() {
    for (Account* acc : accounts) delete acc;
    for (Loan* loan : loans)     delete loan;
}

// ===== Getter / Setter =====
int User::getId() const { return id; }
string User::getFullName() const { return fullName; }
string User::getEmail() const { return email; }
string User::getPassword() const { return password; }
const vector<Account*>& User::getAccounts() const { return accounts; }
const vector<Loan*>& User::getLoans() const { return loans; }

void User::setId(int ID) { this->id = ID; }
void User::setFullName(string fullname) { this->fullName=fullName; }
void User::setEmail(string email) { this->email=email; }
void User::setPassword(string password) { this->password=password; }

// Kiểm tra xác thực mật khẩu
bool User::checkPassword(const string& pw) const {
    return pw == password;
}
  
// ===== Account =====

// Thêm tài khoản mới cho User
Account* User::addAccount(const int& id,const string& name, double initialBalance) {
    Account* account = new Account(id, name, initialBalance);
    accounts.push_back(account);
    return account;
}

// Xóa tài khoản theo ID
bool User::removeAccount(int accountId) {
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i]->getId() == accountId) {
            delete accounts[i];                    
            accounts.erase(accounts.begin() + i);   
            return true;
        }
    }
    return false;
}

// Đổi tên tài khoản hiện có theo ID
bool User::renameAccount(int accountId, const string& newName) {
    for (Account* acc : accounts) {
        if (acc->getId() == accountId) {
            acc->setName(newName);
            return true;
        }
    }
    return false;
}

// Chuyển tiền nội bộ giữa các tài khoản của cùng User
bool User::transfer(int fromAccountId, int toAccountId, double amount, const string& note) {
    if (fromAccountId == toAccountId) {
        cout << "khong the chuyen tien trong cung 1 tai khoan" << endl;
        return false;
    }
    Account* fromAcc = nullptr;
    Account* toAcc   = nullptr;
    for (Account* acc : accounts) {
        if (acc->getId() == fromAccountId) fromAcc = acc;
        if (acc->getId() == toAccountId)   toAcc   = acc;
    }
    if (fromAcc == nullptr || toAcc == nullptr) {
        cout << "khong tim thay tai khoan " << endl;
        return false;
    }
    if (amount < 0) {
        cout << "so tien phai la so duong" << endl;
        return false;
    }
    if (amount > fromAcc->getBalance()) {
        cout << "so du trong tai khoan nguon khong du" << endl;
        return false;
    }
    string today = getToday();
    // Ghi nhận giao dịch chuyển tiền ở cả hai phía
    fromAcc->withdraw("chuyen tien den" + toAcc->getName(), amount, today, "chuyen khoan", note);
    toAcc->deposit("chuyen tien tu" + fromAcc->getName(), amount, today, "chuyen khoan", note);
    cout << "da chuyen tien " << amount << " tu \"" << fromAcc->getName() << "\" den \"" << toAcc->getName() << "\"." << endl;
    return true;
}

 // ===== Loan =====

Loan* User::addLoan(LoanType type, const string& partnerName, double principal, double interestRate,
              const string& startDate, const string& dueDate, const string& note) {
    Loan* loan = new Loan(type, partnerName, principal, interestRate, startDate, dueDate, note);
    loans.push_back(loan);
    return loan;
}

// Cập nhật thông tin khoản vay (lãi suất hoặc ngày đến hạn)
bool User::updateLoan(int loanId, double newInterestRate, const string& newDueDate) {
    for (Loan* loan : loans) {
        if (loan->getId() == loanId) {
            if (newInterestRate >= 0) loan->setInterestRate(newInterestRate);
            if (!newDueDate.empty())  loan->setDueDate(newDueDate);
            return true;
        }
    }
    return false;
}

// xóa khoản vay thông qua ID
bool User::removeLoan(int loanId) {
    for (auto it = loans.begin(); it != loans.end(); ++it) {
        if ((*it)->getId() == loanId) {
            delete *it;
            loans.erase(it);
            return true;
        }
    }
    return false;
}

// ===== Báo cáo =====
// Tổng hợp toàn bộ giao dịch từ các Account để sinh Report
Report User::generateReport(const string& fromDate, const string& toDate) {
    vector<Transaction*> allTx;
    for (Account* acc : accounts) {
        vector<Transaction*> rangeTx = acc->getTransactions(fromDate, toDate);
        allTx.insert(allTx.end(), rangeTx.begin(), rangeTx.end());
    }
    Report report(fromDate, toDate);
    report.build(allTx);
    return report;
}

// Tổng số dư của toàn bộ tài khoản
long User::getTotalBalance() const {
    long total = 0;
    for (Account* acc : accounts) {
        total += acc->getBalance();
    }
    return total;
}

// danh sách Account
void User::listAccounts() const {
    if (accounts.empty()) {
        cout << "danh sach rong" << endl;
    } else {
        cout << "tai khoan cho nguoi dung \"" << fullName << "\":" << endl;
        for (Account* acc : accounts) {
            cout << "  [AccountID " << acc->getId() << "] " 
                 << acc->getName() << " So du: " << acc->getBalance() << endl;
        }
    }
}

// Liệt kê tất cả các khoản vay
void User::listLoans() const {
    if (loans.empty()) {
        cout << "Khong co khoan vay nao" << endl;
    } else {
        cout << "Danh sach cac khoan vay - \"" << fullName << "\":" << endl;
        for (Loan* loan : loans) {
         // Kiểm tra quá hạn phục vụ hiển thị
            string statusStr = loanStatusToString(loan->getStatus());
            cout << "  [LoanID " << loan->getId() << "] | "
                 << (loan->getType() == LoanType::BORROW ? "Vay " : "Cho Vay ")
                 <<"\""<< loan->getPartnerName()<<"\""
                 << " | Tien goc: " << loan->getPrincipal()
                 << " | Lai xuat: " << loan->getInterestRate()
                 << " | Tong no: " << loan->getDueTotal()
                 << " | Da tra: " << loan->getPaidTotal()
                 << " | Con no: " << loan->getRemaining()
                 << " | Tinh trang: " << statusStr << endl;
        }
    }
}

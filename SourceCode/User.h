
#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>

#include "Transaction.h"
#include "Report.h"
#include "Loan.h"
#include "Account.h"

using namespace std;

string getToday();

// Class User: đại diện cho một người người dùng của hệ thống(có tài khoản và khoản)
class User
{
private:
    static int nextId; // Biến tĩnh dùng để sinh ID tự tăng
    int id;
    string fullName;
    string password;
    string email;

    // User sở hữu nhiều Account và Loan
    // Dùng con trỏ để quản lý động vòng đời đối tượng
    vector<Account *> accounts;
    vector<Loan *> loans;

public:
    User(const string &fullName, const string &email, const string &password);
    // Giải phóng toàn bộ tài nguyên động mà User sở hữu
    ~User();

    // ===== Getter / Setter =====
    int getId() const;
    string getFullName() const;
    string getEmail() const;
    string getPassword() const;
    const vector<Account *> &getAccounts() const;
    const vector<Loan *> &getLoans() const;

    void setId(int ID);
    void setFullName(string fullname);
    void setEmail(string email);
    void setPassword(string password);
    void addAccount(Account *acc);
    void addLoan(Loan *l);

    // Kiểm tra xác thực mật khẩu
    bool checkPassword(const string &pw) const;

    // ===== Account =====

    // Thêm tài khoản mới cho User
    Account *addAccount(const int &id, const string &name, long initialBalance = 0);

    // Xóa tài khoản theo ID
    bool removeAccount(int accountId);

    // Đổi tên tài khoản hiện có theo ID
    bool renameAccount(int accountId, const string &newName);

    // Chuyển tiền nội bộ giữa các tài khoản của cùng User
    bool transfer(int fromAccountId, int toAccountId, long amount, const string &note = "");

    // ===== Loan =====

    Loan *addLoan(LoanType type, const string &partnerName, long principal, long interestRate,
                  const string &startDate, const string &dueDate, const string &note = "");

    // thanh toán vay
    Loan *findLoanById(int loanId) const;
    Loan *findMatchingLoan(LoanType type, const string &partnerEmail, long principal, const string &startDate, const string &dueDate) const;

    // Cập nhật thông tin khoản vay (lãi suất hoặc ngày đến hạn)
    bool updateLoan(int loanId, long newInterestRate = -1, const string &newDueDate = "");

    // ===== Báo cáo =====
    // Tổng hợp toàn bộ giao dịch từ các Account để sinh Report
    Report generateReport(const string &fromDate = "", const string &toDate = "");

    // Tổng số dư của toàn bộ tài khoản
    long getTotalBalance() const;

    // danh sách Account
    void listAccounts() const;

    // chuyển khoản đến người khác
    bool transferToOtherUser(int fromAccountId, User *receiver, int toAccountId, long amount, const string &note);
    // Liệt kê tất cả các khoản vay (kèm thông tin tóm tắt)
    void listLoans() const;
    // kiểm tra xem có tồn tại Acount có ID cần tìm không
    Account *findAccountById(int accountId) const;
    void listAccountsBrief() const;

    // kiểm tra id, name trùng
    bool isAccountIdExist(int id) const;
    bool isAccountNameExist(const string &name) const;
};
#endif

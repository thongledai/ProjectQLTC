#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "Transaction.h"
#include "Report.h"
#include "Account.h"

using namespace std;

// class Account đại diện cho một tài khoản tài chính cụ thể (ví dụ: tiền mặt, ngân hàng, ví điện tử)
Account::Account(const int& id, const string& name, long initialBalance) //số dư ban đầu = 0
{
    this->id = id;
    this->name = name;
    this->balance = initialBalance;
}

Account::~Account() {
    for (auto p : transactions) delete p;
    transactions.clear();
}

//các hàm get-set
int Account::getId() const { return id; }
void Account::setId(int newId) { id = newId; }
const string& Account::getName() const { return name; }
void Account::setName(const string& newName) { name = newName; }
long Account::getBalance() const { return balance; }
void Account::setBalance(long newBalance) { balance = newBalance; }
const vector<Transaction*> Account::getTransactions() const { return this->transactions; }



Transaction Account::deposit (const string& title, long amount, const string& date, const string& category, const string& note) 
{ //thu
    if (amount <= 0){
        throw invalid_argument("So tien khong duoc nho hon hoac bang 0");
    }
    Transaction trans(title, amount, date, TransactionType::INCOME, category, note);    
    this->addTransaction(trans);
    this->balance += trans.getAmount();    
    return trans;
}

Transaction Account::withdraw (const string& title ,long amount, const string& date, const string& category, const string& note)
{ //chi 
    if (amount <= 0)
        throw invalid_argument("So tien khong duoc nho hon hoac bang 0");

    if (this->balance < amount)
        throw runtime_error("So du khong du");

    Transaction trans(title, amount, date, TransactionType::EXPENSE, category, note);
    this->addTransaction(trans);
    this->balance -= trans.getAmount();

    return trans;
}

void Account::addTransaction (const Transaction& tx) {
    transactions.push_back(new Transaction(tx));
}



bool Account::editTransaction(const int& txId, const string& category) {
    for (size_t i = 0; i < transactions.size(); ++i) {
        Transaction* t = transactions.at(i);
        if (t->getId() == txId) {
            // Giao dịch đã được thực hiện thì chỉ được sửa phân loại
            t->setCategory(category);
            return true;
        }
    }
    return false; // không tìm thấy ID 
}

long Account::getBalance() {
    return this->balance;
}

//báo cáo các giao dịch trong khoảng từ fromDate đến toDate
void Account::reportTrans(const string& fromDate, const string& toDate){
    Report report(fromDate, toDate);
    report.build(transactions);
    report.display();
}

int Account::nextId = 0;

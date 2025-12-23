#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "Transaction.h"
#include "Report.h"

using namespace std;

// class Account đại diện cho một tài khoản tài chính cụ thể (ví dụ: tiền mặt, ngân hàng, ví điện tử)
class Account{
    private:
        static int nextId;
        int id;
        string name;
        long balance;
        vector<Transaction*> transactions; 
    public:
        Account(const int& id, const string& name, long initialBalance = 0) //số dư ban đầu = 0
        {
            this->id = id;
            this->name = name;
            this->balance = initialBalance;
        }
        ~Account() {
            for (auto p : transactions) delete p;
            transactions.clear();
        }
        //các hàm get-set
        int getId() const { return id; }
        void setId(int newId) { id = newId; }
        const string& getName() const { return name; }
        void setName(const string& newName) { name = newName; }
        long getBalance() const { return balance; }
        void setBalance(long newBalance) { balance = newBalance; }
        const vector<Transaction*>& getTransactions() const { return transactions; }

        // Trả về danh sách giao dịch trong khoảng [fromDate, toDate].
        // Nếu fromDate hoặc toDate rỗng thì không áp giới hạn tương ứng.
        vector<Transaction*> getTransactions(const string& fromDate = "", const string& toDate = "") const {
            vector<Transaction*> result;
            for (Transaction* t : transactions) {
                const string& d = t->getDate();
                if (!fromDate.empty() && d < fromDate) continue;
                if (!toDate.empty()   && d > toDate)   continue;
                result.push_back(t);
            }
            return result;
        }

        Transaction deposit (const string& title, long amount, const string& date, const string& category, const string& note) 
        { //thu
            if (amount <= 0){
                throw invalid_argument("So tien khong duoc nho hon hoac bang 0");
            }
            Transaction trans(title, amount, date, TransactionType::INCOME, category, note);         
            return trans;
        }
        Transaction withdraw (const string& title ,long amount, const string& date, const string& category, const string& note)
        { //chi 
            if (amount <= 0)
                throw invalid_argument("So tien khong duoc nho hon hoac bang 0");

            if (this->balance < amount)
                throw runtime_error("So du khong du");

            Transaction trans(title, amount, date, TransactionType::EXPENSE, category, note);
            return trans;
        }

        void addTransaction (const Transaction& tx) {
            transactions.push_back(new Transaction(tx));
            // cập nhật số dư
            if(tx.getType() == TransactionType::INCOME ) {
                this->balance += tx.getAmount();
            } else if(tx.getType() == TransactionType::EXPENSE ) {
                this->balance -= tx.getAmount();
            }
        }
        bool editTransaction(const int& txId, const Transaction& updated) {
            for (size_t i = 0; i < transactions.size(); ++i) {
                Transaction* t = transactions.at(i);
                if (t->getId() == txId) {
                    // xóa bỏ loại giao dịch + trả lại số dư trước đó
                    if (t->getType() == TransactionType::INCOME) 
                        this->balance -= t->getAmount();
                    else if (t->getType() == TransactionType::EXPENSE) 
                        this->balance += t->getAmount();
                    
                    t->setTitle(updated.getTitle());
                    t->setAmount(updated.getAmount());
                    t->setDate(updated.getDate());
                    t->setType(updated.getType());
                    t->setCategory(updated.getCategory());
                    t->setNote(updated.getNote());
                    // id không thay đổi
                    
                    // cập nhật số dư sau update
                    if (t->getType() == TransactionType::INCOME)
                        this->balance += t->getAmount();
                    else if (t->getType() == TransactionType::EXPENSE)
                        this->balance -= t->getAmount();
                    return true;
                }
            }
            return false; // không tìm thấy ID 
        }

        bool removeTransaction(const int& txId) {
            if (transactions.empty()) return false; //nếu danh sách rỗng
            for (size_t i = 0; i < transactions.size(); ++i) {
                Transaction* t = transactions.at(i);
                if (t->getId() == txId) {
                    //xóa gd
                    transactions.erase(transactions.begin() + i);
                    return true;
                }
            }
            return false; // không tìm thấy ID 
        }
        long getBalance() {
            return this->balance;
        }
        //báo cáo các giao dịch trong khoảng từ fromDate đến toDate
        void reportTrans(const string& fromDate, const string& toDate){
            Report report(fromDate, toDate);
            report.build(transactions);
            report.display();
        }
};


int Account::nextId = 0;

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>
#include <stdexcept>

#include "Transaction.h"
#include "Report.h"

// class Account đại diện cho một tài khoản tài chính cụ thể (ví dụ: tiền mặt, ngân hàng, ví điện tử)
class Account{
    private:
        static int nextId;
        int id;
        std::string name;
        long balance;
        std::vector<Transaction*> transactions; 
    public:
        Account(const int& id, const std::string& name, long initialBalance = 0); //số dư ban đầu = 0
        ~Account();

        //các hàm get-set
        int getId() const;
        void setId(int newId);
        const std::string& getName() const;
        void setName(const std::string& newName);
        long getBalance() const;
        void setBalance(long newBalance);
        const std::vector<Transaction*> getTransactions() const;

        Transaction deposit (const std::string& title, long amount, const std::string& date,
                             const std::string& category, const std::string& note);

        Transaction withdraw (const std::string& title ,long amount, const std::string& date,
                              const std::string& category, const std::string& note);

        void addTransaction (const Transaction& tx);
        void addTransaction(Transaction* trans);
        bool editTransaction(const int& txId, const string& category );
        

        long getBalance();

        //báo cáo các giao dịch trong khoảng từ fromDate đến toDate
        void reportTrans(const std::string& fromDate, const std::string& toDate);
};

#endif

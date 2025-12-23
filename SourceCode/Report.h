#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "Transaction.h"

using namespace std;

// Class Report: báo cáo tài chính (thu nhập, chi tiêu, chênh lệch thu chi) trong 1 khoảng thời gian
class Report {
private:
    string fromDate;
    string toDate;
    long totalIncome;
    long totalExpense;
    long netChange;
public:
    Report(const string& fromDate = "", const string& toDate = "") {
        this->fromDate = fromDate;
        this->toDate = toDate;
        this->totalIncome = 0;
        this->totalExpense = 0;
        this->netChange = 0;
    }

    // Tổng hợp dữ liệu cho bài báo cáo từ danh sách giao dịch Transaction
    void build(const vector<Transaction*> transactions) {
        this->totalIncome = 0;
        this->totalExpense = 0;
        this->netChange =0;

        //Kiểm tra nếu dữ liệu nhập vào không hợp lệ thì kết thúc hàm
        if (this->fromDate > this->toDate) return;
            
        for (int i=0; i<transactions.size(); i++){
            const Transaction* tx = transactions.at(i);

            // Bỏ qua nếu giao dịch nằm ngoài kỳ báo cáo
            if (tx->getDate() < this->fromDate || tx->getDate() > this->toDate)
                continue; 

            if (tx->isIncome()) {
                this->totalIncome += tx->getAmount();
            } else if (tx->isExpense()) {
                this->totalExpense += tx->getAmount();
            }
        }
        this->netChange = this->totalIncome - this->totalExpense;
    }

    long getTotalIncome() const  { return this->totalIncome; }
    long getTotalExpense() const { return this->totalExpense; }
    long getNetChange() const    { return this->netChange; }

    // In báo cáo theo kỳ đã nhập
    void display() const {
        cout << "Bao cao tu ngay " << this->fromDate << " den ngay " << this->toDate << ":\n";
        if (this->totalIncome ==0 && this->totalExpense ==0){
            cout << "Khong co giao dich nao trong ky bao cao.\n";
            return;
        }
        cout << "Tong thu nhap: " << this->totalIncome << endl;
        cout << "Tong chi tieu: " << this->totalExpense << endl;
        cout << "Chenh lech thu chi: " << this->netChange << endl;
    }
};

#ifndef REPORT_H
#define REPORT_H

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
    Report(const string& fromDate = "", const string& toDate = "");

    // Tổng hợp dữ liệu cho bài báo cáo từ danh sách giao dịch Transaction
    void build(const vector<Transaction*> transactions);

    long getTotalIncome() const;
    long getTotalExpense() const;
    long getNetChange() const;

    // In báo cáo theo kỳ đã nhập
    void display() const;
};

#endif

#ifndef LOAN_H
#define LOAN_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

string getToday();

// Thanh toán khoản vay
class Payment {
private:
    static int nextId;
    int id;
    long amount;
    string date;
    string note;
public:
    Payment(long amount, const string& date, const string& note = "");
    int getId() const { return id; }
    long getAmount() const{ return amount; }
    string getDate() const  { return date; }
    string getNote() const  { return note; }
};

// BROROW: user đi vay tiền người khác
// LEND: user cho vay tiền
enum class LoanType { BORROW, LEND };
string loanTypeToString(LoanType t);

// Trạng thái khoản vay
// OPEN: chưa thanh toán
// PAR: đã trả 1 ít
// PAID: đã trả hết
// OVER: đã quá hạn
enum class LoanStatus { OPEN, PARTIALLY_PAID, PAID, OVERDUE };
string loanStatusToString(LoanStatus s);

// Class Loan: hồ sơ vay và cho vay
class Loan {
private:
    static int nextId; 
    int id;
    LoanType type; 
    string partnerName;
    long principal; // tiền gốc
    double interestRate; // lãi xuất
    string startDate;
    string dueDate;
    LoanStatus status;
    vector<Payment*> payments;
    string note;

public:
    Loan(LoanType type, const string& partnerName, long principal, double interestRate,
         const string& startDate, const string& dueDate, const string& note = "");

    ~Loan();

    //getter
    int getId() const         { return id; }
    LoanType getType() const  { return type; }
    string getPartnerName() const { return partnerName; }
    long getPrincipal() const   { return principal; }
    double getInterestRate() const{ return interestRate; }
    string getStartDate() const   { return startDate; }
    string getDueDate() const     { return dueDate; }
    LoanStatus getStatus() const  { return status; }
    string getNote() const        { return note; }
    
    //setter
    void setPartnerName(const string& name) { partnerName = name; }
    void setInterestRate(double rate)       { interestRate = rate; }
    void setDueDate(const string& date)     { dueDate = date; }
    
    // Tổng số tiền đã thanh toán
    long getPaidTotal() const;
    
    // Tổng số tiền nợ
    long getDueTotal() const;
    
    // Số tiền còn nợ
    long getRemaining() const;
    
    // Thanh toán
    void addPayment(long amount, const string& date, const string& note = "");

    // Kiểm tra khoản vay có bị quá hạn không
    bool isOverdue(const string& today);

    // In ra danh sách đã thanh toán
    void listPayments() const;
};

#endif

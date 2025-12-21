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
    Payment(long amount, const string& date, const string& note = "") {
        this->id = ++nextId;
        this->amount = amount;
        this->date = date;
        this->note = note;
    }
    int getId() const { return id; }
    long getAmount() const{ return amount; }
    string getDate() const  { return date; }
    string getNote() const  { return note; }
};
int Payment::nextId = 0;

// BROROW: user đi vay tiền người khác
// LEND: user cho vay tiền
enum class LoanType { BORROW, LEND };
string loanTypeToString(LoanType t) {
    return (t == LoanType::BORROW ? "BORROW" : "LEND");
}

// Trạng thái khoản vay
// OPEN: chưa thanh toán
// PAR: đã trả 1 ít
// PAID: đã trả hết
// OVER: đã quá hạn
enum class LoanStatus { OPEN, PARTIALLY_PAID, PAID, OVERDUE };
string loanStatusToString(LoanStatus s) {
    switch(s) {
        case LoanStatus::OPEN:           return "OPEN";
        case LoanStatus::PARTIALLY_PAID: return "PARTIALLY_PAID";
        case LoanStatus::PAID:           return "PAID";
        case LoanStatus::OVERDUE:        return "OVERDUE";
    }
    return "UNKNOWN";
}

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
         const string& startDate, const string& dueDate, const string& note = "") {
        this->id = ++nextId; // cấp ID tự động
        this->type = type;
        this->partnerName = partnerName;
        this->principal = principal;
        this->interestRate = interestRate;
        this->startDate = startDate;
        this->dueDate = dueDate;
        this->note = note;
        this->status = LoanStatus::OPEN;
    }

    ~Loan() {
        for (Payment* p : payments) {
            delete p;
        }
    }

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
    long getPaidTotal() const {
        double total = 0;
        for (Payment* p : payments) {
            total += p->getAmount();
        }
        return total;
    }
    
    // Tổng số tiền nợ
    long getDueTotal() const{
        return principal + principal * interestRate / 100.0;
    }
    
    // Số tiền còn nợ
    long getRemaining() const {
        double rem = getDueTotal() - getPaidTotal();
        return (rem < 0 ? 0 : rem);
    }
    
    // Thanh toán
    void addPayment(long amount, const string& date, const string& note = "") {
        if (amount <= 0) {
            cout << "So tien thanh toan phai duong" << endl;
            return;
        }
        Payment* payment = new Payment(amount, date, note);
        payments.push_back(payment);

        if (getRemaining()<=0) {
            status = LoanStatus::PAID;
        } else {
            if (getPaidTotal()> 0) {
                status = LoanStatus::PARTIALLY_PAID;
            }
            string today = getToday();
            if (today > dueDate && status != LoanStatus::PAID) {
                status = LoanStatus::OVERDUE;
            }
        }
    }

    // Kiểm tra khoản vay có bị quá hạn không
    bool isOverdue(const string& today) {
        if (status != LoanStatus::PAID && today > dueDate) {
            status = LoanStatus::OVERDUE;
            return true;
        }
        return false;
    }

    // In ra danh sách đã thanh toán
    void listPayments() const {
        if (payments.empty()) {
            cout << "No payments recorded for this loan." << endl;
        } else {
            cout << "Payments for loan ID " << id << ":" << endl;
            for (Payment* p : payments) {
                cout << "  [PaymentID " << p->getId() << "] "
                     << p->getDate() << " - Amount: " << p->getAmount();
                if (!p->getNote().empty()) cout << " (Note: " << p->getNote() << ")";
                cout << endl;
            }
        }
    }
};
int Loan::nextId = 0;

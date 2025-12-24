#include "Loan.h"

using namespace std;

int Payment::nextId = 0;
int Loan::nextId = 0;

Payment::Payment(long amount, const string& date, const string& note) {
    this->id = ++nextId;
    this->amount = amount;
    this->date = date;
    this->note = note;
}

string loanTypeToString(LoanType t) {
    return (t == LoanType::BORROW ? "BORROW" : "LEND");
}

LoanType stringToLoanType(const string& str) {
    if (str == "BORROW") {
        return LoanType::BORROW;
    } else if (str == "LEND") {
        return LoanType::LEND;
    } else return LoanType::UNKNOWN;
}

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
Loan::Loan(LoanType type, const string& partnerEmail, long principal, double interestRate,
           const string& startDate, const string& dueDate, const string& note) {
    this->id = ++nextId; // cấp ID tự động
    this->type = type;
    this->partnerEmail = partnerEmail;
    this->principal = principal;
    this->interestRate = interestRate;
    this->startDate = startDate;
    this->dueDate = dueDate;
    this->note = note;
    this->status = LoanStatus::OPEN;
}

Loan::~Loan() {
    for (Payment* p : payments) {
        delete p;
    }
}

// Tổng số tiền đã thanh toán
long Loan::getPaidTotal() const {
    double total = 0;
    for (Payment* p : payments) {
        total += p->getAmount();
    }
    return total;
}

// Tổng số tiền nợ
long Loan::getDueTotal() const{
    return principal + principal * interestRate / 100.0;
}

// Số tiền còn nợ
long Loan::getRemaining() const {
    double rem = getDueTotal() - getPaidTotal();
    return (rem < 0 ? 0 : rem);
}

// Thanh toán
void Loan::addPayment(long amount, const string& date, const string& note) {
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
bool Loan::isOverdue(const string& today) {
    if (status != LoanStatus::PAID && today > dueDate) {
        status = LoanStatus::OVERDUE;
        return true;
    }
    return false;
}

// In ra danh sách đã thanh toán
void Loan::listPayments() const {
    if (payments.empty()) {
        cout << "No payments recorded for this loan." << endl;
    } else {
        cout << "Payments for loan ID " << id << ":" << endl;
        for (Payment* p : payments) {
            cout << " [PaymentID " << p->getId() << "] "
                 << p->getDate() << " - Amount: " << p->getAmount();
            if (!p->getNote().empty()) cout << " (Note: " << p->getNote() << ")";
            cout << endl;
        }
    }
}
void Loan::showPaymentHistory() const {
    cout << "\nLich Su Thanh Toan Khoan Vay:\n";
    cout << "Loan ID: " << id << endl;
    cout << "Loai: " << loanTypeToString(type) << endl;
    cout << "Doi tac: " << partnerEmail << endl;
    cout << "Tien goc: " << principal << endl;
    cout << "Tong no: " << getDueTotal() << endl;
    cout << "Da tra: " << getPaidTotal() << endl;
    cout << "Con lai: " << getRemaining() << endl;
    cout << "Trang thai: " << loanStatusToString(status) << endl;
    listPayments();  
}

 

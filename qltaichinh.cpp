#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <limits>
using namespace std;

// Enum for transaction types
enum class TransactionType { INCOME, EXPENSE };
string transactionTypeToString(TransactionType t) {
    switch(t) {
        case TransactionType::INCOME:  return "INCOME";
        case TransactionType::EXPENSE: return "EXPENSE";
    }
    return "UNKNOWN";
}

// Enum for loan types (borrow or lend)
enum class LoanType { BORROW, LEND };
string loanTypeToString(LoanType t) {
    return (t == LoanType::BORROW ? "BORROW" : "LEND");
}

// Enum for loan status
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

// Utility to get current date as YYYY-MM-DD string
string getToday() {
    time_t t = time(NULL);
    tm *timePtr = localtime(&t);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", timePtr);
    return string(buf);
}

// Forward declarations for classes
class Transaction;
class Account;
class Loan;
class Payment;
class Report;
class User;
class App;
class Menu;

// Class Payment: represents a payment made towards a Loan
class Payment {
private:
    static int nextId;
    int id;
    double amount;
    string date;
    string note;
public:
    Payment(double amount, const string& date, const string& note = "") {
        this->id = ++nextId;
        this->amount = amount;
        this->date = date;
        this->note = note;
    }
    int getId() const       { return id; }
    double getAmount() const{ return amount; }
    string getDate() const  { return date; }
    string getNote() const  { return note; }
    void setNote(const string& newNote) { note = newNote; }
};
int Payment::nextId = 0;

// Class Transaction: represents a financial transaction (income, expense, or transfer)
class Transaction {
private:
    static int nextId;
    int id;
    string title;
    double amount;
    string date;
    TransactionType type;
    string category;
    string note;
public:
    Transaction(const string& title, double amount, const string& date, TransactionType type,
                const string& category = "", const string& note = "") {
        this->id = ++nextId;
        this->title = title;
        this->amount = amount;
        this->date = date;
        this->type = type;
        this->category = category;
        this->note = note;
    }
    int getId() const        { return id; }
    string getTitle() const  { return title; }
    double getAmount() const { return amount; }
    string getDate() const   { return date; }
    TransactionType getType() const { return type; }
    string getCategory() const { return category; }
    string getNote() const     { return note; }
    void setTitle(const string& newTitle)     { title = newTitle; }
    void setAmount(double newAmount)          { amount = newAmount; }
    void setDate(const string& newDate)       { date = newDate; }
    void setCategory(const string& newCategory){ category = newCategory; }
    void setNote(const string& newNote)       { note = newNote; }
    bool isIncome() const  { return type == TransactionType::INCOME; }
    bool isExpense() const { return type == TransactionType::EXPENSE; }
};
int Transaction::nextId = 0;

// class Account đại diện cho một tài khoản tài chính cụ thể (ví dụ: tiền mặt, ngân hàng, ví điện tử)
class Account{
    private:
        string id;
        string name;
        long balance;
        vector<Transaction>* transactions; 
    public:
        Account(const string& id, const string& name, long initialBalance = 0) //số dư ban đầu = 0
        {
            this->id = id;
            this->name = name;
            this->balance = initialBalance;
            transactions = new vector<Transaction>();
        }
        ~Account() { delete transactions; }
        const string& getName() const { return name; }
        double getBalance() const { return balance; }
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
            transactions->push_back(tx);
            // cập nhật số dư
            if(tx.getType() == TransactionType::INCOME ) {
                this->balance += tx.getAmount();
            } else if(tx.getType() == TransactionType::EXPENSE ) {
                this->balance -= tx.getAmount();
            }
        }
        bool editTransaction(const string& txId, const Transaction& updated) {
            for (size_t i = 0; i < transactions->size(); ++i) {
                Transaction* t = &transactions->at(i);
                if (to_string(t->id) == txId) {
                    // xóa bỏ loại giao dịch + trả lại số dư trước đó
                    if (t->getType() == TransactionType::INCOME) 
                        this->balance -= t->getAmount();
                    else if (t->getType() == TransactionType::EXPENSE) 
                        this->balance += t->getAmount();
                    
                    t->title = updated.title;
                    t->amount = updated.amount;
                    t->date = updated.date;
                    t->type = updated.type;
                    t->category = updated.category;
                    t->note = updated.note;
                    // t.id không thay đổi

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

        bool removeTransaction(const string& txId) {
            if (transactions->empty()) return false; //nếu danh sách rỗng
            for (size_t i = 0; i < transactions->size(); ++i) {
                Transaction* t = &transactions->at(i);
                if (to_string(t->id) == txId) {
                    //trả lại số dư trước đó
                    if (t->getType() == TransactionType::INCOME) {
                        balance -= t->getAmount();
                    } else if (t->getType() == TransactionType::EXPENSE) {
                        balance += t->getAmount();
                    }
                    //xóa gd
                    transactions->erase(transactions->begin() + i);
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

// Class Loan: represents a borrowing or lending record
class Loan {
private:
    static int nextId;
    int id;
    LoanType type;
    string partnerName;
    double principal;
    double interestRate;
    string startDate;
    string dueDate;
    LoanStatus status;
    vector<Payment*> payments;
    string note;
public:
    Loan(LoanType type, const string& partnerName, double principal, double interestRate,
         const string& startDate, const string& dueDate, const string& note = "") {
        this->id = ++nextId;
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
        // Free all Payment objects
        for (Payment* p : payments) {
            delete p;
        }
    }
    int getId() const         { return id; }
    LoanType getType() const  { return type; }
    string getPartnerName() const { return partnerName; }
    double getPrincipal() const   { return principal; }
    double getInterestRate() const{ return interestRate; }
    string getStartDate() const   { return startDate; }
    string getDueDate() const     { return dueDate; }
    LoanStatus getStatus() const  { return status; }
    string getNote() const        { return note; }
    void setPartnerName(const string& name) { partnerName = name; }
    void setInterestRate(double rate)       { interestRate = rate; }
    void setDueDate(const string& date)     { dueDate = date; }
    void setNote(const string& newNote)     { note = newNote; }

    // Record a payment towards this loan
    void addPayment(double amount, const string& date, const string& note = "") {
        if (amount <= 0) {
            cout << "Payment amount must be positive!" << endl;
            return;
        }
        Payment* payment = new Payment(amount, date, note);
        payments.push_back(payment);
        // Update loan status after payment
        double paidTotal = getPaidTotal();
        if (paidTotal >= principal) {
            status = LoanStatus::PAID;
        } else {
            if (paidTotal > 0) {
                status = LoanStatus::PARTIALLY_PAID;
            }
            string today = getToday();
            if (today > dueDate && status != LoanStatus::PAID) {
                status = LoanStatus::OVERDUE;
            }
        }
    }

    // Calculate total paid amount so far
    double getPaidTotal() const {
        double total = 0;
        for (Payment* p : payments) {
            total += p->getAmount();
        }
        return total;
    }

    // Calculate remaining amount (principal minus paid)
    double getRemaining() const {
        double rem = principal - getPaidTotal();
        return (rem < 0 ? 0 : rem);
    }

    // Mark the loan as fully paid
    void markPaid() {
        status = LoanStatus::PAID;
    }

    // Check if loan is overdue as of a given date (and update status if so)
    bool isOverdue(const string& today) {
        if (status != LoanStatus::PAID && today > dueDate) {
            status = LoanStatus::OVERDUE;
            return true;
        }
        return false;
    }

    // List all payments made for this loan
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
    void build(const vector<Transaction>* transactions) {
        this->totalIncome = 0;
        this->totalExpense = 0;
        this->netChange =0;
        
        for (int i=0; i<transactions->size(); i++){
            const Transaction* tx = &transactions->at(i);

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

// Class User: đại diện cho một người người dùng của hệ thống(có tài khoản và khoản)
class User {
private:
   static int nextId;                 // Biến tĩnh dùng để sinh ID tự tăng

    int id;
    string fullName;
    string password;
    string email;

    // User sở hữu nhiều Account và Loan
    // Dùng con trỏ để quản lý động vòng đời đối tượng
    vector<Account*> accounts;
    vector<Loan*> loans;

public:
    User(const string& fullName, const string& email, const string& password) {
        this->id = ++nextId;
        this->fullName = fullName;
        this->email = email;
        this->password = password;
    }
    // Giải phóng toàn bộ tài nguyên động mà User sở hữu
    ~User() {
        for (Account* acc : accounts) delete acc;
        for (Loan* loan : loans)     delete loan;
    }
    // ===== Getter / Setter =====
    int getId() const { return id; }
    string getFullName() const { return fullName; }
    string getEmail() const { return email; }
    const vector<Account*>& getAccounts() const { return accounts; }
    const vector<Loan*>& getLoans() const { return loans; }

    void setId(int ID) { this->id = ID; }
    void setFullName(string fullname) { this->fullName=fullName; }
    void setEmail(string email) { this->email=email; }
    void setPassword(string password) { this->password=password; }

    // Kiểm tra xác thực mật khẩu
    bool checkPassword(const string& pw) const {
        return pw == password;
    }
  
    // ===== Account =====

    // Thêm tài khoản mới cho User
    Account* addAccount(const string& name, double initialBalance = 0.0) {
        Account* account = new Account(name, initialBalance);
        accounts.push_back(account);
        return account;
    }

    // Xóa tài khoản theo ID
   bool removeAccount(int accountId) {
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
    bool renameAccount(int accountId, const string& newName) {
        for (Account* acc : accounts) {
            if (acc->getId() == accountId) {
                acc->setName(newName);
                return true;
            }
        }
        return false;
    }
// Chuyển tiền nội bộ giữa các tài khoản của cùng User
    bool transfer(int fromAccountId, int toAccountId, double amount, const string& note = "") {
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
        fromAcc->withdraw(amount, "chuyen tien den" + toAcc->getName(), "chuyen khoan", note, today, TransactionType::TRANSFER);
        toAcc->deposit(amount, "chuyen tien tu" + fromAcc->getName(), "chuyen khoan", note, today, TransactionType::TRANSFER);
        cout << "da chuyen tien " << amount << " tu \"" << fromAcc->getName() << "\" den \"" << toAcc->getName() << "\"." << endl;
        return true;
    }

     // ===== Loan =====

    Loan* addLoan(LoanType type, const string& partnerName, double principal, double interestRate,
                  const string& startDate, const string& dueDate, const string& note = "") {
        Loan* loan = new Loan(type, partnerName, principal, interestRate, startDate, dueDate, note);
        loans.push_back(loan);
        return loan;
    }

    // Cập nhật thông tin khoản vay (lãi suất hoặc ngày đến hạn)
    bool updateLoan(int loanId, double newInterestRate = -1, const string& newDueDate = "") {
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
    bool removeLoan(int loanId) {
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
    Report generateReport(const string& fromDate = "", const string& toDate = "") {
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
    long getTotalBalance() const {
        long total = 0;
        for (Account* acc : accounts) {
            total += acc->getBalance();
        }
        return total;
    }

    // danh sách Account
    void listAccounts() const {
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

    // Liệt kê tất cả các khoản vay (kèm thông tin tóm tắt)
    void listLoans() const {
        if (loans.empty()) {
            cout << "khong co khoan vay nao duoc ghi nhan" << endl;
        } else {
            cout << " khoan vay danh cho nguoi dung \"" << fullName << "\":" << endl;
            for (Loan* loan : loans) {
             // Kiểm tra quá hạn phục vụ hiển thị
                string statusStr = loanStatusToString(loan->getStatus());
                cout << "  [LoanID " << loan->getId() << "] "
                     << (loan->getType() == LoanType::BORROW ? "muon " : "cho muon")
                     << loan->getPartnerName()
                     << " | Tien goc: " << loan->getPrincipal()
                     << " | Tra : " << loan->getPaidTotal()
                     << " | con lai : " << loan->getRemaining()
                     << " | tinh trang khoan vay o thoi diem hien tai: " << statusStr << endl;
            }
        }
    }
};
int User::nextId = 0;

// Class App: orchestrates the users and manages the current logged-in user
class App {
private:
    vector<User*> users;
    User* currentUser;
public:
    App() : currentUser(nullptr) {}
    ~App() {
        // Clean up all users
        for (User* u : users) {
            delete u;
        }
    }

    // Register a new user (returns the User pointer or nullptr if failed)
    User* registerUser(const string& fullName, const string& email, const string& password) {
        // Ensure email is unique
        for (User* u : users) {
            if (u->getEmail() == email) {
                cout << "Email \"" << email << "\" is already registered. Please use a different email." << endl;
                return nullptr;
            }
        }
        User* user = new User(fullName, email, password);
        users.push_back(user);
        cout << "Registration successful! You can now log in." << endl;
        return user;
    }

    // Log in a user by email and password
    bool login(const string& email, const string& password) {
        for (User* u : users) {
            if (u->getEmail() == email) {
                if (u->checkPassword(password)) {
                    currentUser = u;
                    cout << "Welcome, " << u->getFullName() << "!" << endl;
                    return true;
                } else {
                    cout << "Incorrect password. Please try again." << endl;
                    return false;
                }
            }
        }
        cout << "No account found with email: " << email << endl;
        return false;
    }

    // Log out the current user
    void logout() {
        if (currentUser) {
            cout << "User \"" << currentUser->getFullName() << "\" has been logged out." << endl;
        }
        currentUser = nullptr;
    }

    User* getCurrentUser() const {
        return currentUser;
    }

    // Export current user's transaction data to a CSV file
    void exportDataCSV(const string& filename) {
        if (currentUser == nullptr) {
            cout << "No user is logged in." << endl;
            return;
        }
        ofstream file(filename);
        if (!file) {
            cout << "Unable to open file: " << filename << endl;
            return;
        }
        // Write CSV header
        file << "Account,Date,Title,Amount,Type,Category,Note\n";
        // For each account of current user, output each transaction
        for (Account* acc : currentUser->getAccounts()) {
            for (Transaction* tx : acc->getTransactions()) {
                file << acc->getName() << ","
                     << tx->getDate() << ","
                     << (tx->getType() == TransactionType::EXPENSE ? "-" : "") << tx->getAmount() << ","
                     << transactionTypeToString(tx->getType()) << ","
                     << tx->getCategory() << ","
                     << tx->getNote() << "\n";
            }
        }
        file.close();
        cout << "Data exported to \"" << filename << "\" successfully." << endl;
    }
};

// Class Menu: handles the user interaction menu system
class Menu {
private:
    App& app;
public:
    Menu(App& appRef) : app(appRef) {}

    // Display menu for not logged-in state
    void showMainMenu() {
        cout << "\n==== Personal Finance Management ====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
    }

    // Display menu for logged-in state (current user)
    void showUserMenu(const string& userName) {
        cout << "\n==== Main Menu (User: " << userName << ") ====\n";
        cout << "1. Add Account\n";
        cout << "2. List Accounts\n";
        cout << "3. Deposit to Account\n";
        cout << "4. Withdraw from Account\n";
        cout << "5. Transfer Between Accounts\n";
        cout << "6. List Transactions (for an Account)\n";
        cout << "7. Add Loan (Borrow/Lend)\n";
        cout << "8. List Loans\n";
        cout << "9. Record Loan Payment\n";
        cout << "10. Mark Loan as Paid\n";
        cout << "11. Generate Report (Income/Expense)\n";
        cout << "12. Export Data to CSV\n";
        cout << "13. Logout\n";
        cout << "Choose an option: ";
    }

    // Run the menu loop
    void run() {
        bool running = true;
        while (running) {
            if (app.getCurrentUser() == nullptr) {
                // Not logged in: show main menu
                showMainMenu();
                int choice;
                if (!(cin >> choice)) {
                    // Handle non-integer input
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush leftover newline
                switch (choice) {
                    case 1: { // Register new user
                        string name, email, pass;
                        cout << "Enter full name: ";
                        getline(cin, name);
                        cout << "Enter email: ";
                        cin >> email;
                        cout << "Enter password: ";
                        cin >> pass;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        app.registerUser(name, email, pass);
                        break;
                    }
                    case 2: { // Login existing user
                        string email, pass;
                        cout << "Enter email: ";
                        cin >> email;
                        cout << "Enter password: ";
                        cin >> pass;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        app.login(email, pass);
                        break;
                    }
                    case 3: // Exit application
                        cout << "Exiting application. Goodbye!" << endl;
                        running = false;
                        break;
                    default:
                        cout << "Invalid choice. Please enter a number from the menu." << endl;
                }
            } else {
                // Logged in: show user menu
                string userName = app.getCurrentUser()->getFullName();
                showUserMenu(userName);
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                User* user = app.getCurrentUser();
                switch (choice) {
                    case 1: { // Add Account
                        string accName;
                        double initBal;
                        cout << "Enter new account name: ";
                        getline(cin, accName);
                        cout << "Enter initial balance (0 if none): ";
                        cin >> initBal;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        user->addAccount(accName, initBal);
                        cout << "Account \"" << accName << "\" has been added." << endl;
                        break;
                    }
                    case 2: { // List Accounts
                        user->listAccounts();
                        break;
                    }
                    case 3: { // Deposit to Account
                        int accId;
                        double amt;
                        string title, category, note, date;
                        cout << "Enter Account ID to deposit into: ";
                        cin >> accId;
                        cout << "Enter amount to deposit: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter transaction title: ";
                        getline(cin, title);
                        cout << "Enter category (optional): ";
                        getline(cin, category);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        cout << "Enter date (YYYY-MM-DD, leave blank for today): ";
                        getline(cin, date);
                        if (date.empty()) date = getToday();
                        bool found = false;
                        for (Account* acc : user->getAccounts()) {
                            if (acc->getId() == accId) {
                                acc->deposit(amt, title, category, note, date);
                                cout << "Deposited " << amt << " to \"" << acc->getName() 
                                     << "\". New balance: " << acc->getBalance() << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Account ID " << accId << " not found." << endl;
                        }
                        break;
                    }
                    case 4: { // Withdraw from Account
                        int accId;
                        double amt;
                        string title, category, note, date;
                        cout << "Enter Account ID to withdraw from: ";
                        cin >> accId;
                        cout << "Enter amount to withdraw: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter transaction title: ";
                        getline(cin, title);
                        cout << "Enter category (optional): ";
                        getline(cin, category);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        cout << "Enter date (YYYY-MM-DD, leave blank for today): ";
                        getline(cin, date);
                        if (date.empty()) date = getToday();
                        bool found = false;
                        for (Account* acc : user->getAccounts()) {
                            if (acc->getId() == accId) {
                                if (acc->withdraw(amt, title, category, note, date)) {
                                    cout << "Withdrew " << amt << " from \"" << acc->getName() 
                                         << "\". New balance: " << acc->getBalance() << endl;
                                }
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Account ID " << accId << " not found or insufficient balance." << endl;
                        }
                        break;
                    }
                    case 5: { // Transfer between Accounts
                        int fromId, toId;
                        double amt;
                        string note;
                        cout << "Enter source Account ID: ";
                        cin >> fromId;
                        cout << "Enter destination Account ID: ";
                        cin >> toId;
                        cout << "Enter amount to transfer: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter note for transfer (optional): ";
                        getline(cin, note);
                        user->transfer(fromId, toId, amt, note);
                        break;
                    }
                    case 6: { // List Transactions for an Account
                        int accId;
                        cout << "Enter Account ID to view transactions: ";
                        cin >> accId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        bool found = false;
                        for (Account* acc : user->getAccounts()) {
                            if (acc->getId() == accId) {
                                acc->listTransactions();
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Account ID " << accId << " not found." << endl;
                        }
                        break;
                    }
                    case 7: { // Add Loan
                        int typeChoice;
                        string partner;
                        double principal, interest;
                        string startDate, dueDate, note;
                        cout << "Enter loan type (1 = BORROW (you owe), 2 = LEND (you lend out)): ";
                        cin >> typeChoice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        LoanType type = (typeChoice == 2 ? LoanType::LEND : LoanType::BORROW);
                        cout << "Enter partner name (lender/borrower): ";
                        getline(cin, partner);
                        cout << "Enter principal amount: ";
                        cin >> principal;
                        cout << "Enter annual interest rate (%): ";
                        cin >> interest;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter start date (YYYY-MM-DD): ";
                        getline(cin, startDate);
                        cout << "Enter due date (YYYY-MM-DD): ";
                        getline(cin, dueDate);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        user->addLoan(type, partner, principal, interest, startDate, dueDate, note);
                        cout << "Loan added successfully." << endl;
                        break;
                    }
                    case 8: { // List Loans
                        // Update overdue status for each loan before listing (based on current date)
                        string today = getToday();
                        for (Loan* loan : user->getLoans()) {
                            loan->isOverdue(today);
                        }
                        user->listLoans();
                        break;
                    }
                    case 9: { // Record Loan Payment
                        int loanId;
                        double amt;
                        string date, note;
                        cout << "Enter Loan ID to record a payment: ";
                        cin >> loanId;
                        cout << "Enter payment amount: ";
                        cin >> amt;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter payment date (YYYY-MM-DD): ";
                        getline(cin, date);
                        cout << "Enter note (optional): ";
                        getline(cin, note);
                        bool found = false;
                        for (Loan* loan : user->getLoans()) {
                            if (loan->getId() == loanId) {
                                loan->addPayment(amt, date, note);
                                cout << "Payment recorded. Remaining balance: " << loan->getRemaining() << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Loan ID " << loanId << " not found." << endl;
                        }
                        break;
                    }
                    case 10: { // Mark Loan as Paid
                        int loanId;
                        cout << "Enter Loan ID to mark as fully paid: ";
                        cin >> loanId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        bool found = false;
                        for (Loan* loan : user->getLoans()) {
                            if (loan->getId() == loanId) {
                                loan->markPaid();
                                cout << "Loan " << loanId << " marked as PAID." << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Loan ID " << loanId << " not found." << endl;
                        }
                        break;
                    }
                    case 11: { // Generate Income/Expense Report
                        string fromDate, toDate;
                        cout << "Enter start date for report (YYYY-MM-DD, leave blank for no start limit): ";
                        getline(cin, fromDate);
                        cout << "Enter end date for report (YYYY-MM-DD, leave blank for no end limit): ";
                        getline(cin, toDate);
                        Report rep = user->generateReport(fromDate, toDate);
                        rep.display();
                        break;
                    }
                    case 12: { // Export Data to CSV
                        string filename;
                        cout << "Enter filename to export data (e.g., data.csv): ";
                        getline(cin, filename);
                        if (filename.empty()) filename = "export.csv";
                        app.exportDataCSV(filename);
                        break;
                    }
                    case 13: { // Logout
                        app.logout();
                        break;
                    }
                    default:
                        cout << "Invalid choice. Please enter a number from the menu." << endl;
                }
            }
        }
    }
};

// Main function: create App and Menu, then start the menu loop
int main() {
    App app;
    Menu menu(app);
    menu.run();
    return 0;
}

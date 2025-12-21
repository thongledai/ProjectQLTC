#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>



using namespace std;

// Enum to represent transaction types
enum class TransactionType { INCOME, EXPENSE, TRANSFER_IN, TRANSFER_OUT };

// Class representing a financial transaction (income, expense, or transfer)
class Transaction {
private:
    TransactionType type;
    double amount;
    string date;
    string note;
public:
    Transaction(TransactionType t, double amt, const string& dt, const string& nt)
        : type(t), amount(amt), date(dt), note(nt) {}
    TransactionType getType() const { return type; }
    double getAmount() const { return amount; }
    const string& getDate() const { return date; }
    const string& getNote() const { return note; }
};

// Class representing a financial account with a balance and transaction history
class Account {
private:
    string name;
    double balance;
    vector<Transaction*> transactions; // using pointers to Transaction
public:
    Account(const string& name, double initialBalance = 0.0) 
      : name(name), balance(initialBalance) {}
    ~Account() {
        // Release dynamically allocated transactions
        for (Transaction* t : transactions) {
            delete t;
        }
        transactions.clear();
    }
    const string& getName() const { return name; }
    double getBalance() const { return balance; }
    // Add a transaction to this account (updates balance accordingly)
    void addTransaction(TransactionType type, double amount, const string& date, const string& note) {
        Transaction* newTrans = new Transaction(type, amount, date, note);
        transactions.push_back(newTrans);
        // Update account balance based on transaction type
        if(type == TransactionType::INCOME || type == TransactionType::TRANSFER_IN) {
            balance += amount;
        } else if(type == TransactionType::EXPENSE || type == TransactionType::TRANSFER_OUT) {
            balance -= amount;
        }
    }
    // Remove a transaction by index (0-based), returns true if removed
    bool removeTransaction(size_t index) {
        if(index >= transactions.size()) return false;
        Transaction* t = transactions[index];
        // Adjust balance by reversing the transaction effect
        if(t->getType() == TransactionType::INCOME || t->getType() == TransactionType::TRANSFER_IN) {
            balance -= t->getAmount();
        } else if(t->getType() == TransactionType::EXPENSE || t->getType() == TransactionType::TRANSFER_OUT) {
            balance += t->getAmount();
        }
        transactions.erase(transactions.begin() + index);
        delete t;
        return true;
    }
    // List all transactions in this account
    void listTransactions() const {
        if(transactions.empty()) {
            cout << "No transactions for account \"" << name << "\".\n";
        } else {
            cout << "Transactions for account \"" << name << "\":\n";
            for(size_t i = 0; i < transactions.size(); ++i) {
                const Transaction* t = transactions[i];
                string typeStr;
                switch(t->getType()) {
                    case TransactionType::INCOME: typeStr = "Income"; break;
                    case TransactionType::EXPENSE: typeStr = "Expense"; break;
                    case TransactionType::TRANSFER_IN: typeStr = "Transfer In"; break;
                    case TransactionType::TRANSFER_OUT: typeStr = "Transfer Out"; break;
                }
                // Display index (1-based), date, type, sign and amount, note
                cout << i+1 << ". [" << t->getDate() << "] " << typeStr << " ";
                if(t->getType() == TransactionType::EXPENSE || t->getType() == TransactionType::TRANSFER_OUT)
                    cout << "-";
                else
                    cout << "+";
                cout << t->getAmount() << " - " << t->getNote() << "\n";
            }
        }
    }
    // Get the list of transaction pointers (read-only)
    const vector<Transaction*>& getTransactions() const { return transactions; }
};

// Class to manage multiple accounts (collection of Account objects)
class FinanceManager {
private:
    vector<Account*> accounts; // using pointers to Account
public:
    ~FinanceManager() {
        // Clean up all accounts
        for(Account* acc : accounts) {
            delete acc;
        }
        accounts.clear();
    }
    void addAccount(const string& name, double initialBalance = 0.0) {
        Account* acc = new Account(name, initialBalance);
        accounts.push_back(acc);
    }
    bool removeAccount(const string& name) {
        for(auto it = accounts.begin(); it != accounts.end(); ++it) {
            if((*it)->getName() == name) {
                delete *it;
                accounts.erase(it);
                return true;
            }
        }
        return false;
    }
    Account* findAccount(const string& name) {
        for(Account* acc : accounts) {
            if(acc->getName() == name) return acc;
        }
        return nullptr;
    }
    void listAccounts() const {
        if(accounts.empty()) {
            cout << "No accounts available.\n";
        } else {
            cout << "Accounts list:\n";
            for(size_t i = 0; i < accounts.size(); ++i) {
                const Account* acc = accounts[i];
                cout << i+1 << ". " << acc->getName() << " - Balance: " << acc->getBalance() << "\n";
            }
        }
    }
    const vector<Account*>& getAccounts() const { return accounts; }
};

// Class to handle the text-based menu interface and user interactions
class Menu {
private:
    FinanceManager manager;
    // Helper functions for each menu action
    void printMainMenu() const {
        cout << "\n=== Personal Finance Manager ===\n";
        cout << "1. Add Account\n";
        cout << "2. Remove Account\n";
        cout << "3. List Accounts\n";
        cout << "4. Add Transaction\n";
        cout << "5. Remove Transaction\n";
        cout << "6. Show Transactions of an Account\n";
        cout << "7. Transfer Between Accounts\n";
        cout << "8. Export Data to CSV\n";
        cout << "0. Exit\n";
    }
    void addAccountMenu() {
        string name;
        cout << "Enter new account name: ";
        getline(cin, name);
        if(name.empty()) {
            cout << "Account name cannot be empty.\n";
            return;
        }
        if(manager.findAccount(name) != nullptr) {
            cout << "Account \"" << name << "\" already exists.\n";
            return;
        }
        double initBalance = 0.0;
        cout << "Enter initial balance (or 0 if none): ";
        if(!(cin >> initBalance)) {
            cout << "Invalid amount. Setting initial balance to 0.\n";
            initBalance = 0.0;
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        manager.addAccount(name, initBalance);
        cout << "Account \"" << name << "\" added successfully.\n";
    }
    void removeAccountMenu() {
        string name;
        cout << "Enter account name to remove: ";
        getline(cin, name);
        if(name.empty()) {
            cout << "Name cannot be empty.\n";
            return;
        }
        if(manager.removeAccount(name)) {
            cout << "Account \"" << name << "\" removed.\n";
        } else {
            cout << "Account \"" << name << "\" not found.\n";
        }
    }
    void listAccountsMenu() {
        manager.listAccounts();
    }
    void addTransactionMenu() {
        string accName;
        cout << "Enter account name to add transaction: ";
        getline(cin, accName);
        Account* acc = manager.findAccount(accName);
        if(acc == nullptr) {
            cout << "Account not found.\n";
            return;
        }
        int typeChoice;
        cout << "Enter transaction type (1 = Income, 2 = Expense): ";
        if(!(cin >> typeChoice)) {
            cout << "Invalid type choice.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if(typeChoice != 1 && typeChoice != 2) {
            cout << "Invalid type choice.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        TransactionType type = (typeChoice == 1) ? TransactionType::INCOME : TransactionType::EXPENSE;
        double amt;
        cout << "Enter amount: ";
        if(!(cin >> amt)) {
            cout << "Invalid amount entered.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if(amt <= 0) {
            cout << "Amount must be positive.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if(type == TransactionType::EXPENSE && amt > acc->getBalance()) {
            cout << "Warning: expense amount is greater than current balance.\n";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string date;
        cout << "Enter date (YYYY-MM-DD or any format): ";
        getline(cin, date);
        if(date.empty()) date = "N/A";
        string note;
        cout << "Enter note/description: ";
        getline(cin, note);
        if(note.empty()) note = "(none)";
        acc->addTransaction(type, amt, date, note);
        cout << "Transaction added to account \"" << accName << "\".\n";
    }
    void removeTransactionMenu() {
        string accName;
        cout << "Enter account name to remove a transaction from: ";
        getline(cin, accName);
        Account* acc = manager.findAccount(accName);
        if(acc == nullptr) {
            cout << "Account not found.\n";
            return;
        }
        const vector<Transaction*>& trans = acc->getTransactions();
        if(trans.empty()) {
            cout << "No transactions in this account.\n";
            return;
        }
        acc->listTransactions();
        cout << "Enter transaction number to remove: ";
        size_t index;
        if(!(cin >> index)) {
            cout << "Invalid input.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if(index < 1 || index > trans.size()) {
            cout << "Invalid transaction number.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        size_t idx0 = index - 1;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(acc->removeTransaction(idx0)) {
            cout << "Transaction #" << index << " removed from account \"" << accName << "\".\n";
        } else {
            cout << "Failed to remove transaction.\n";
        }
    }
    void listTransactionsMenu() {
        string accName;
        cout << "Enter account name to view transactions: ";
        getline(cin, accName);
        Account* acc = manager.findAccount(accName);
        if(acc == nullptr) {
            cout << "Account not found.\n";
        } else {
            acc->listTransactions();
        }
    }
    void transferMenu() {
        string fromName, toName;
        cout << "Enter source account name: ";
        getline(cin, fromName);
        cout << "Enter destination account name: ";
        getline(cin, toName);
        if(fromName == toName) {
            cout << "Source and destination cannot be the same.\n";
            return;
        }
        Account* accFrom = manager.findAccount(fromName);
        Account* accTo = manager.findAccount(toName);
        if(accFrom == nullptr || accTo == nullptr) {
            cout << "One or both accounts not found.\n";
            return;
        }
        double amt;
        cout << "Enter transfer amount: ";
        if(!(cin >> amt)) {
            cout << "Invalid amount.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if(amt <= 0) {
            cout << "Amount must be positive.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if(amt > accFrom->getBalance()) {
            cout << "Insufficient balance in source account.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string date;
        cout << "Enter date of transfer: ";
        getline(cin, date);
        if(date.empty()) date = "N/A";
        string note;
        cout << "Enter note for transfer: ";
        getline(cin, note);
        if(note.empty()) note = "(transfer)";
        accFrom->addTransaction(TransactionType::TRANSFER_OUT, amt, date, 
                                 string("Transfer to ") + accTo->getName() + 
                                 (note == "(transfer)" ? "" : (" - " + note)));
        accTo->addTransaction(TransactionType::TRANSFER_IN, amt, date, 
                               string("Transfer from ") + accFrom->getName() + 
                               (note == "(transfer)" ? "" : (" - " + note)));
        cout << "Transferred " << amt << " from \"" << fromName << "\" to \"" << toName << "\" successfully.\n";
    }
    void exportCSVMenu() {
        string filename;
        cout << "Enter filename to export (e.g., data.csv): ";
        getline(cin, filename);
        if(filename.empty()) {
            cout << "Filename cannot be empty.\n";
            return;
        }
        ofstream fout(filename);
        if(!fout) {
            cout << "Error opening file for writing.\n";
            return;
        }
        fout << "Account,Date,Type,Amount,Note\n";
        for(const Account* acc : manager.getAccounts()) {
            for(const Transaction* t : acc->getTransactions()) {
                string typeStr;
                switch(t->getType()) {
                    case TransactionType::INCOME: typeStr = "Income"; break;
                    case TransactionType::EXPENSE: typeStr = "Expense"; break;
                    case TransactionType::TRANSFER_IN: typeStr = "Transfer In"; break;
                    case TransactionType::TRANSFER_OUT: typeStr = "Transfer Out"; break;
                }
                string accName = acc->getName();
                string date = t->getDate();
                string note = t->getNote();
                for(char &c : accName) if(c == ',') c = ';';
                for(char &c : date) if(c == ',') c = ';';
                for(char &c : note) if(c == ',') c = ';';
                fout << accName << "," << date << "," << typeStr << "," 
                     << t->getAmount() << "," << note << "\n";
            }
        }
        fout.close();
        cout << "Data exported to " << filename << " successfully.\n";
    }
    void importCSVMenu() {
        string filename;
        cout << "Enter CSV filename to import: ";
        getline(cin, filename);

        ifstream fin(filename);
        if (!fin) {
            cout << "Cannot open file.\n";
            return;
        }

        string line;
        getline(fin, line); // bỏ header

        while (getline(fin, line)) {
            stringstream ss(line);
            string accName, date, typeStr, amountStr, note;

            getline(ss, accName, ',');
            getline(ss, date, ',');
            getline(ss, typeStr, ',');
            getline(ss, amountStr, ',');
            getline(ss, note);

            double amount = stod(amountStr);

            Account* acc = manager.findAccount(accName);
            if (!acc) {
                manager.addAccount(accName, 0);
                acc = manager.findAccount(accName);
            }

            TransactionType type;
            if (typeStr == "Income") type = TransactionType::INCOME;
            else if (typeStr == "Expense") type = TransactionType::EXPENSE;
            else if (typeStr == "Transfer In") type = TransactionType::TRANSFER_IN;
            else if (typeStr == "Transfer Out") type = TransactionType::TRANSFER_OUT;
            else continue;

            acc->addTransaction(type, amount, date, note);
        }

        fin.close();
        cout << "Import CSV completed successfully.\n";
    }

    
public:
    void run() {
        int choice;
        do {
            printMainMenu();
            cout << "Enter your choice: ";
            if(!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                choice = -1;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch(choice) {
                case 1: addAccountMenu(); break;
                case 2: removeAccountMenu(); break;
                case 3: listAccountsMenu(); break;
                case 4: addTransactionMenu(); break;
                case 5: removeTransactionMenu(); break;
                case 6: listTransactionsMenu(); break;
                case 7: transferMenu(); break;
                case 8: exportCSVMenu(); break;
                case 0: cout << "Exiting program...\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } while(choice != 0);
    }
};

int main() {
    Menu menu;
    menu.run();
    return 0;
}

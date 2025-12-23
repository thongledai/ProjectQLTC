#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

// Enum cho các loại giao dịch
enum class TransactionType { INCOME, EXPENSE };

// Chuyển enum TransactionType sang string
string transactionTypeToString(TransactionType t);

// Class Transaction: Thể hiện một giao dịch tài chính: income (thu), expense (chi)
class Transaction {
private:
    static int nextId;
    int id;
    string title;
    long amount;
    string date;
    TransactionType type;
    string category;
    string note;
public:
    Transaction(const string& title, long amount, const string& date, TransactionType type,
                const string& category = "", const string& note = "");

    int getId() const;
    string getTitle() const;
    long getAmount() const;
    string getDate() const;
    TransactionType getType() const;
    string getCategory() const;
    string getNote() const;

    void setTitle(const string& newTitle);
    void setAmount(double newAmount);
    void setDate(const string& newDate);
    void setCategory(const string& newCategory);
    void setNote(const string& newNote);
    void setType(const TransactionType& t);

    void print()const ;
    bool isIncome() const;
    bool isExpense() const;
};

#endif

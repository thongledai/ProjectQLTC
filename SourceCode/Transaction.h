#pragma once
#include <string>

using namespace std;

// Enum cho các loại giao dịch
enum class TransactionType { INCOME, EXPENSE };
string transactionTypeToString(TransactionType t) {
    switch(t) {
        case TransactionType::INCOME:  return "INCOME";
        case TransactionType::EXPENSE: return "EXPENSE";
    }
    return "UNKNOWN";
}

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
    long getAmount() const { return amount; }
    string getDate() const   { return date; }
    TransactionType getType() const { return type; }
    string getCategory() const { return category; }
    string getNote() const     { return note; }
    void setTitle(const string& newTitle)     { title = newTitle; }
    void setAmount(double newAmount)          { amount = newAmount; }
    void setDate(const string& newDate)       { date = newDate; }
    void setCategory(const string& newCategory){ category = newCategory; }
    void setNote(const string& newNote)       { note = newNote; }
    void setType(const TransactionType& t) { type = t; }
    bool isIncome() const  { return type == TransactionType::INCOME; }
    bool isExpense() const { return type == TransactionType::EXPENSE; }
};
int Transaction::nextId = 0;

#include "Transaction.h"

using namespace std;

int Transaction::nextId = 0;

// Chuyển enum TransactionType sang string
string transactionTypeToString(TransactionType t) {
    switch(t) {
        case TransactionType::INCOME:  return "INCOME";
        case TransactionType::EXPENSE: return "EXPENSE";
    }
    return "UNKNOWN";
}

// Constructor
Transaction::Transaction(const string& title, long amount, const string& date, TransactionType type,
                         const string& category, const string& note) {
    this->id = ++nextId;
    this->title = title;
    this->amount = amount;
    this->date = date;
    this->type = type;
    this->category = category;
    this->note = note;
}

int Transaction::getId() const        { return id; }
string Transaction::getTitle() const  { return title; }
long Transaction::getAmount() const   { return amount; }
string Transaction::getDate() const   { return date; }
TransactionType Transaction::getType() const { return type; }
string Transaction::getCategory() const { return category; }
string Transaction::getNote() const     { return note; }

void Transaction::setTitle(const string& newTitle)        { title = newTitle; }
void Transaction::setAmount(double newAmount)             { amount = newAmount; }
void Transaction::setDate(const string& newDate)          { date = newDate; }
void Transaction::setCategory(const string& newCategory)  { category = newCategory; }
void Transaction::setNote(const string& newNote)          { note = newNote; }
void Transaction::setType(const TransactionType& t)       { type = t; }

bool Transaction::isIncome() const  { return type == TransactionType::INCOME; }
bool Transaction::isExpense() const { return type == TransactionType::EXPENSE; }

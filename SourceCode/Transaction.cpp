#include "Transaction.h"
<<<<<<< HEAD

=======
#include <iostream>
>>>>>>> master
using namespace std;

int Transaction::nextId = 0;

// Chuyển enum TransactionType sang string
<<<<<<< HEAD
string transactionTypeToString(TransactionType t) {
    switch(t) {
        case TransactionType::INCOME:  return "INCOME";
        case TransactionType::EXPENSE: return "EXPENSE";
=======
string transactionTypeToString(TransactionType t)
{
    switch (t)
    {
    case TransactionType::INCOME:
        return "INCOME";
    case TransactionType::EXPENSE:
        return "EXPENSE";
>>>>>>> master
    }
    return "UNKNOWN";
}

<<<<<<< HEAD
// Constructor
Transaction::Transaction(const string& title, long amount, const string& date, TransactionType type,
                         const string& category, const string& note) {
=======
TransactionType stringToTransactionType(const string& str) {
    if (str == "INCOME") return TransactionType::INCOME;
    else if (str == "EXPENSE") return TransactionType::EXPENSE;
    else return TransactionType::UNKNOWN;
}


// Constructor
Transaction::Transaction(const string &title, long amount, const string &date, TransactionType type,
                         const string &category, const string &note)
{
>>>>>>> master
    this->id = ++nextId;
    this->title = title;
    this->amount = amount;
    this->date = date;
    this->type = type;
    this->category = category;
    this->note = note;
}

<<<<<<< HEAD
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
=======
int Transaction::getId() const { return id; }
string Transaction::getTitle() const { return title; }
long Transaction::getAmount() const { return amount; }
string Transaction::getDate() const { return date; }
TransactionType Transaction::getType() const { return type; }
string Transaction::getCategory() const { return category; }
string Transaction::getNote() const { return note; }

void Transaction::setId(int id) { this->id = id; }
void Transaction::setTitle(const string &newTitle) { title = newTitle; }
void Transaction::setAmount(double newAmount) { amount = newAmount; }
void Transaction::setDate(const string &newDate) { date = newDate; }
void Transaction::setCategory(const string &newCategory) { category = newCategory; }
void Transaction::setNote(const string &newNote) { note = newNote; }
void Transaction::setType(const TransactionType &t) { type = t; }
void Transaction::print() const
{
    cout << "Ma giao dich: " << id << endl;
    cout << "Tieu de: " << title << endl;
    cout << "So tien: " << amount << endl;
    cout << "Ngay: " << date << endl;
    cout << "Loai giao dich: " << transactionTypeToString(type) << endl;
    cout << "Danh muc: " << category << endl;
    cout << " Ghi chu: " << note << endl;
}

bool Transaction::isIncome() const { return type == TransactionType::INCOME; }
>>>>>>> master
bool Transaction::isExpense() const { return type == TransactionType::EXPENSE; }

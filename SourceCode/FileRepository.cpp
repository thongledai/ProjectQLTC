#include "FileRepository.h"
#include "Loan.h"
#include <fstream>

json FileRepository::loadDatabase() {
    std::ifstream f("database.json");
    if (!f.is_open())
        return json{{"users", json::array()}};

    if (f.peek() == std::ifstream::traits_type::eof()) {
        return json{{"users", json::array()}};
    }
    
    json db;
    f >> db;
    f.close();
    return db;
}

std::vector<User*> FileRepository::loadUsersWithData() {
    json db = loadDatabase();
    std::vector<User*> users;

    if (!db.contains("users")) return users;

    for (auto& jUser : db["users"]) {
        User* user = new User(
            jUser["userName"],
            jUser["userEmail"],
            jUser["userPassword"]
        );
        user->setId(jUser["userId"]);

        // ===== LOANS =====
        if (jUser.contains("loans")) {
            for (auto& jLoan : jUser["loans"]) {
                Loan* loan = new Loan(
                    stringToLoanType(jLoan["loanType"]),
                    jLoan["loanPartnerEmail"],
                    jLoan["loanPrincipal"],
                    jLoan["loanInterestRate"],
                    jLoan["loanStartDate"],
                    jLoan["loanDueDate"],
                    jLoan["loanNote"]
                );
                loan->setId(jLoan["loanId"]);

                user->addLoan(loan);
            }
        }

        // ===== ACCOUNTS =====
        if (jUser.contains("accounts")) {
            for (auto& jAcc : jUser["accounts"]) {
                Account* acc = new Account(
                    jAcc["accountId"],
                    jAcc["accountName"],
                    jAcc["accountBalance"]
                );

                // ===== TRANSACTIONS =====
                if (jAcc.contains("transactions")) {
                    for (auto& jTr : jAcc["transactions"]) {
                        Transaction* t = new Transaction(
                            jTr["transactionTitle"],
                            jTr["transactionAmount"],
                            jTr["transactionDate"],
                            stringToTransactionType(jTr["transactionType"]),
                            jTr["transactionCategory"],
                            jTr["transactionNote"]
                        );
                        t->setId(jTr["transactionId"]);

                        acc->addTransaction(t);
                    }
                }

                user->addAccount(acc);
            }
        }

        users.push_back(user);
    }

    return users;
}


void FileRepository::saveData(const vector<User*>& users) {
    json j;

    for (auto u : users) {
        json jUser;
        jUser["userId"] = u->getId();
        jUser["userName"] = u->getFullName();
        jUser["userEmail"] = u->getEmail();
        jUser["userPassword"] = u->getPassword();

        for (auto loan: u->getLoans()) {
            json jLoan;
            jLoan["loanId"] = loan->getId();
            jLoan["loanType"] = loanTypeToString(loan->getType());
            jLoan["loanPartnerEmail"] = loan->getPartnerEmail();
            jLoan["loanPrincipal"] = loan->getPrincipal();
            jLoan["loanInterestRate"] = loan->getInterestRate();
            jLoan["loanStartDate"] = loan->getStartDate();
            jLoan["loanDueDate"] = loan->getDueDate();
            jLoan["loanNote"] = loan->getNote();

            jUser["loans"].push_back(jLoan);
        }

        for (auto acc : u->getAccounts()) {
            json jAcc;
            jAcc["accountId"] = acc->getId();
            jAcc["accountName"] = acc->getName();
            jAcc["accountBalance"] = acc->getBalance();

            for (auto t : acc->getTransactions()) {
                json jT;
                jT["transactionId"] = t->getId();
                jT["transactionTitle"] = t->getTitle();
                jT["transactionAmount"] = t->getAmount();
                jT["transactionDate"] = t->getDate();
                jT["transactionType"] = transactionTypeToString(t->getType());
                jT["transactionCategory"] = t->getCategory();
                jT["transactionNote"] = t->getNote();

                jAcc["transactions"].push_back(jT);
            }

            jUser["accounts"].push_back(jAcc);
        }

        j["users"].push_back(jUser);
    }

    ofstream file("database.json");
    file << j.dump(4);
    file.close();
}



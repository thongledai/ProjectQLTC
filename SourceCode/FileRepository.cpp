#include "FileRepository.h"
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

json* FileRepository::findUserNode(json& db, int userId) {
    for (auto& u : db["users"])
        if (u["userId"] == userId)
            return &u;
    return nullptr;
}

std::vector<User*> FileRepository::loadUsers() {
    json db = loadDatabase();
    std::vector<User*> users;

    for (auto& u : db["users"]) {
        User* newUser = new User(u["userName"], u["userEmail"], u["userPassword"]);
        newUser->setId(u["userId"]);

        users.push_back(newUser);
    }
    return users;
}

// std::vector<Account*> FileRepository::loadAccounts(int userId) {
//     json db = loadDatabase();
//     std::vector<Account*> res;

//     json* user = findUserNode(db, userId);
//     if (!user || !user->contains("accounts")) return res;

//     for (auto& jAcc : (*user)["accounts"]) {
//         Account* newAccount = new Account(jAcc["accountId"], jAcc["accountName"], jAcc["accountBalance"]);

//         res.push_back(newAccount);
//     }
//     return res;
// }

// std::vector<Transaction*> FileRepository::loadTransactionsByAccount(int userId, int accId) {
//     json db = loadDatabase();
//     std::vector<Transaction*> res;

//     json* user = findUserNode(db, userId);
//     if (!user) return res;

//     for (auto& acc : (*user)["accounts"]) {
//         if (acc["accountId"] != accId) continue;

//         if (!acc.contains("transactions")) return res;

//         for (auto& jTr : acc["transactions"]) {
//             Transaction* t = new Transaction(
//                 jTr["transactionTitle"].get<std::string>(),
//                 jTr["transactionAmount"].get<long>(),
//                 jTr["transactionDate"].get<std::string>(),
//                 stringToTransactionType(
//                     jTr["transactionType"].get<std::string>()
//                 ),
//                 jTr["transactionCategory"].get<std::string>(),
//                 jTr["transactionNote"].get<std::string>()
//             );

//             t->setId(jTr["transactionId"]);
//             res.push_back(t);
//         }
//     }
//     return res;
// }

void FileRepository::saveData(const vector<User*>& users) {
    json j;

    for (auto u : users) {
        json jUser;
        jUser["userId"] = u->getId();
        jUser["userName"] = u->getFullName();
        jUser["userEmail"] = u->getEmail();
        jUser["userPassword"] = u->getPassword();

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



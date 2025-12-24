#include "FileRepository.h"
#include <fstream>

/* ================== LOW LEVEL ================== */

json FileRepository::loadDatabase() {
    std::ifstream f("database.json");
    if (!f.is_open())
        return json{{"users", json::array()}};

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

void FileRepository::saveDatabase(const json& db) {
    std::ofstream file("database.json");
    file << db.dump(4);
    file.close();
}

std::vector<User*> FileRepository::loadUsers() {
    json db = loadDatabase();
    std::vector<User*> users;

    for (auto& u : db["users"]) {
        User* newUser = new User(u["fullName"], u["email"], u["password"]);
        newUser->setId(u["userId"]);

        users.push_back(newUser);
    }
    return users;
}

// void FileRepository::saveUsers(const std::vector<User*>& users) {
//     json db = loadDatabase();

//     // Reset danh sách users
//     db["users"] = json::array();

//     for (const User* user : users) {
//         json jUser;
//         jUser["userId"]   = user->getId();
//         jUser["fullName"] = user->getFullName();
//         jUser["email"]    = user->getEmail();
//         jUser["password"] = user->getPassword();

//         // accounts & loans sẽ được các hàm khác ghi vào sau
//         jUser["accounts"] = json::array();
//         jUser["loans"]    = json::array();

//         db["users"].push_back(jUser);
//     }

//     saveDatabase(db);
// }

// Xử lý dữ liệu Account
std::vector<Account*> FileRepository::loadAccounts(int userId) {
    json db = loadDatabase();
    std::vector<Account*> res;

    json* user = findUserNode(db, userId);
    if (!user || !user->contains("accounts")) return res;

    for (auto& jAcc : (*user)["accounts"]) {
        Account* newAccount = new Account(jAcc["id"], jAcc["name"], jAcc["balance"]);

        res.push_back(newAccount);
    }
    return res;
}

// void FileRepository::saveAccounts(int userId, const std::vector<Account*>& accounts) {
//     json db = loadDatabase();

//     json* user = findUserNode(db, userId);
//     if (!user) return;

//     (*user)["accounts"] = json::array();

//     for (const Account* acc : accounts) {
//         json jAcc{
//             {"id", acc->getId()},
//             {"name", acc->getName()},
//             {"balance", acc->getBalance()}
//         };

//         (*user)["accounts"].push_back(jAcc);
//     }

//     saveDatabase(db);
// }






void FileRepository::saveData(const vector<User*>& users) {
    json j;

    for (auto u : users) {
        json jUser;
        jUser["userId"] = u->getId();
        jUser["fullName"] = u->getFullName();
        jUser["email"] = u->getEmail();
        jUser["password"] = u->getPassword();

        for (auto acc : u->getAccounts()) {
            json jAcc;
            jAcc["id"] = acc->getId();
            jAcc["name"] = acc->getName();
            jAcc["balance"] = acc->getBalance();

            for (auto t : acc->getTransactions()) {
                json jT;
                jT["id"] = t->getId();
                jT["title"] = t->getTitle();
                jT["amount"] = t->getAmount();
                jT["date"] = t->getDate();
                jT["type"] = transactionTypeToString(t->getType());
                jT["category"] = t->getCategory();
                jT["note"] = t->getNote();

                jAcc["transactions"].push_back(jT);
            }

            jUser["accounts"].push_back(jAcc);
        }

        j["users"].push_back(jUser);
    }

    ofstream file("database.json");
    file << j.dump(4); // format đẹp
    file.close();
}



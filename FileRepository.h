#pragma once
#include <vector>
#include <string>
#include "json.hpp"
#include "User.h"
#include "Account.h"
#include "Transaction.h"
#include "Loan.h"

using json = nlohmann::json;

class FileRepository {
private:
    static json loadDatabase();
    static json* findUserNode(json& db, int userId);
    static void saveDatabase(const json& db);

public:
    static std::vector<User*> loadUsers();
    // static void saveUsers(const std::vector<User*>& users);

    static std::vector<Account*> loadAccounts(int userId);
    // static void saveAccounts(int userId, const std::vector<Account*>& accounts);

    static void saveData(const vector<User*>& users);
};

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

public:
    static std::vector<User*> loadUsers();
    static std::vector<Account*> loadAccounts(int userId);

    static std::vector<Transaction*> loadTransactionsByAccount(int userId, int accId);

    static void saveData(const vector<User*>& users);
};

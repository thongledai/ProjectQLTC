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

public:
    static std::vector<User*> loadUsersWithData();

    static void saveData(const vector<User*>& users);
};

#ifndef APP_H
#define APP_H

#include <vector>
#include <string>

#include "User.h"

// Class App: quản lý danh sách người dùng và người dùng đang đăng nhập
class App {
private:
    std::vector<User*> users; // App là lớp sở hữu các đối tượng User
    User* currentUser; // con trỏ đến người dùng hiện đang đăng nhập
public:
    App();
    ~App();

    // Kiểm tra có người dùng nào đang đăng nhập không
    bool isLoggedIn() const;

    // Đăng ký tài khoản mới (trả về nullptr nếu đăng ký thất bại)
    User* registerUser(const std::string& fullName, const std::string& email, const std::string& password);

    // Đăng nhập User bằng email và mật khẩu
    bool login(const std::string& email, const std::string& password);

    // Đăng xuất User hiện tại
    void logout();

    // Lấy thông tin User đang đăng nhập
    User* getCurrentUser() const;

    // Xuất dữ liệu ra file CSV
    void exportDataCSV(const std::string& filename);
};

#endif

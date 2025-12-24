#ifndef APP_H
#define APP_H

#include <vector>
#include <string>

#include "User.h"

// Class App: quản lý danh sách người dùng và người dùng đang đăng nhập
class App
{
private:
    std::vector<User *> users; // App là lớp sở hữu các đối tượng User
    User *currentUser;         // con trỏ đến người dùng hiện đang đăng nhập
public:
    App();
    ~App();

    // Kiểm tra có người dùng nào đang đăng nhập không
    bool isLoggedIn() const;

    // Đăng ký tài khoản mới (trả về nullptr nếu đăng ký thất bại)
    User *registerUser(const std::string &fullName, const std::string &email, const std::string &password);

    // Đăng nhập User bằng email và mật khẩu
    bool login(const std::string &email, const std::string &password);

    // Đăng xuất User hiện tại
    void logout();

    // Lấy thông tin User đang đăng nhập
    User *getCurrentUser() const;

    // Xuất dữ liệu ra file CSV
    void exportDataCSV(const std::string &filename);

    const std::vector<User *> &getAllUsers() const;

    // chuyển khoản đến user khác bằng email
    bool transferUser(
        int fromAccountId,
        const std::string &receiverEmail,
        int toAccountId,
        long amount,
        const std::string &note);
    // chuyển khoản vay/cho vay
    bool transferFromUser(
        User *sender,
        int fromAccountId,
        const string &receiverEmail,
        int toAccountId,
        long amount,
        const string &note);

    // chuyển khoản từ người dung

    User *findUserByEmail(const std::string &email);
};

#endif

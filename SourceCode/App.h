#include <iostream>
#include <vector>
#include <string>

#include "User.h"

using namespace std;

// Class App: quản lý danh sách người dùng và người dùng đang đăng nhập
class App {
private:
    vector<User*> users; // App là lớp sở hữu các đối tượng User
    User* currentUser; // con trỏ đến người dùng hiện đang đăng nhập
public:
    App() : currentUser(nullptr) {}

    ~App() {
        // Xóa toàn bộ user
        for (User* u : users) {
            delete u;
        }
        // Xóa danh sách con trỏ sau khi đã delete
        users.clear();
    }

    // Kiểm tra có người dùng nào đang đăng nhập không
    bool isLoggedIn() const {
        return currentUser != nullptr;
    }

    // Đăng ký tài khoản mới (trả về nullptr nếu đăng ký thất bại)
    User* registerUser(const string& fullName, const string& email, const string& password) {
        // Kiểm tra email đã tồn tại hay chưa
        for (User* u : users) {
            if (u->getEmail() == email) {
                cout << "Email \"" << email << "\" da duoc dang ky! Vui long nhap email khac." << endl;
                return nullptr;
            }
        }
        // Email chưa tồn tại nên tạo User mới
        User* newUser = new User(fullName, email, password);
        users.push_back(newUser);
        cout << "Dang ky thanh cong! Ban co the dang nhap ngay bay gio." << endl;
        return newUser;
    }

    // Đăng nhập User bằng email và mật khẩu
    bool login(const string& email, const string& password) {
        // Nếu đã có người đăng nhập thì không cho đăng nhập tiếp
        if (isLoggedIn()){
            cout << "Hien tai da co nguoi dang nhap! Vui long dang xuat truoc.";
            return false;
        }

        // Tìm User có email tương ứng và kiểm tra mật khẩu
        for (User* u : users) {
            if (u->getEmail() == email) {
                if (u->checkPassword(password)) {
                    currentUser = u;
                    cout << "Chao mung " << u->getFullName() << "! Ban da dang nhap thanh cong." << endl;
                    return true;
                } else {
                    cout << "Mat khau khong dung! Vui long thu lai." << endl;
                    return false;
                }
            }
        }
        cout << "Khong tim thay tai khoan voi email: " << email << endl;
        return false;
    }

    // Đăng xuất User hiện tại
    void logout() {
        if (isLoggedIn()) {
            cout << "Nguoi dung \"" << currentUser->getFullName() << "\" da dang xuat thanh cong." << endl;
        }
        currentUser = nullptr;
    }

    // Lấy thông tin User đang đăng nhập
    User* getCurrentUser() const { return currentUser; }

    // Xuất dữ liệu ra file CSV
    void exportDataCSV(const string& filename) {
       cout<<"No data";
    }
};

#include <iostream>
#include "App.h"
<<<<<<< HEAD

using namespace std;

App::App() : currentUser(nullptr) {}

App::~App() {
    // Xóa toàn bộ user
    for (User* u : users) {
=======
#include "User.h"
#include "FileRepository.h"
using namespace std;

App::App() : currentUser(nullptr) {
    users = FileRepository::loadUsers();
}

App::~App()
{   
    FileRepository::saveData(users);
    // Xóa toàn bộ user
    for (User *u : users)
    {
>>>>>>> master
        delete u;
    }
    // Xóa danh sách con trỏ sau khi đã delete
    users.clear();
}

// Kiểm tra có người dùng nào đang đăng nhập không
<<<<<<< HEAD
bool App::isLoggedIn() const {
=======
bool App::isLoggedIn() const
{
>>>>>>> master
    return currentUser != nullptr;
}

// Đăng ký tài khoản mới (trả về nullptr nếu đăng ký thất bại)
<<<<<<< HEAD
User* App::registerUser(const string& fullName, const string& email, const string& password) {
    // Kiểm tra email đã tồn tại hay chưa
    for (User* u : users) {
        if (u->getEmail() == email) {
=======
User *App::registerUser(const string &fullName, const string &email, const string &password)
{
    // Kiểm tra email đã tồn tại hay chưa
    for (User *u : users)
    {
        if (u->getEmail() == email)
        {
>>>>>>> master
            cout << "Email \"" << email << "\" da duoc dang ky! Vui long nhap email khac." << endl;
            return nullptr;
        }
    }
    // Email chưa tồn tại nên tạo User mới
<<<<<<< HEAD
    User* newUser = new User(fullName, email, password);
=======
    User *newUser = new User(fullName, email, password);
>>>>>>> master
    users.push_back(newUser);
    cout << "Dang ky thanh cong! Ban co the dang nhap ngay bay gio." << endl;
    return newUser;
}

// Đăng nhập User bằng email và mật khẩu
<<<<<<< HEAD
bool App::login(const string& email, const string& password) {
    // Nếu đã có người đăng nhập thì không cho đăng nhập tiếp
    if (isLoggedIn()){
=======
bool App::login(const string &email, const string &password)
{
    // Nếu đã có người đăng nhập thì không cho đăng nhập tiếp
    if (isLoggedIn())
    {
>>>>>>> master
        cout << "Hien tai da co nguoi dang nhap! Vui long dang xuat truoc.";
        return false;
    }

    // Tìm User có email tương ứng và kiểm tra mật khẩu
<<<<<<< HEAD
    for (User* u : users) {
        if (u->getEmail() == email) {
            if (u->checkPassword(password)) {
                currentUser = u;
                cout << "Chao mung " << u->getFullName() << "! Ban da dang nhap thanh cong." << endl;
                return true;
            } else {
=======
    for (User *u : users)
    {
        if (u->getEmail() == email)
        {
            if (u->checkPassword(password))
            {
                currentUser = u;
                cout << "Chao mung " << u->getFullName() << "! Ban da dang nhap thanh cong." << endl;
                return true;
            }
            else
            {
>>>>>>> master
                cout << "Mat khau khong dung! Vui long thu lai." << endl;
                return false;
            }
        }
    }
    cout << "Khong tim thay tai khoan voi email: " << email << endl;
    return false;
}

// Đăng xuất User hiện tại
<<<<<<< HEAD
void App::logout() {
    if (isLoggedIn()) {
=======
void App::logout()
{
    if (isLoggedIn())
    {
>>>>>>> master
        cout << "Nguoi dung \"" << currentUser->getFullName() << "\" da dang xuat thanh cong." << endl;
    }
    currentUser = nullptr;
}

// Lấy thông tin User đang đăng nhập
<<<<<<< HEAD
User* App::getCurrentUser() const { return currentUser; }

// Xuất dữ liệu ra file CSV
void App::exportDataCSV(const string& filename) {
    cout << "No data";
}
=======
User *App::getCurrentUser() const { return currentUser; }

User* App::findUserByEmail(const string& email) {
    for (User* u : users) {
        if (u->getEmail() == email) return u;
    }
    return nullptr;
}

bool App::transferUser(
    int fromAccountId,
    const string& receiverEmail,
    int toAccountId,
    long amount,
    const string& note
) {
    User* sender = currentUser;
    if (!sender) return false;

    User* receiver = findUserByEmail(receiverEmail);
    if (!receiver || receiver->getId() == sender->getId()) return false;

    return sender->transferToOtherUser(
        fromAccountId,
        receiver,
        toAccountId,
        amount,
        note
    );
}
bool App::transferFromUser(
    User* sender,
    int fromAccountId,
    const string& receiverEmail,
    int toAccountId,
    long amount,
    const string& note
) {
    if (!sender) return false;

    User* receiver = findUserByEmail(receiverEmail);
    if (!receiver || receiver->getId() == sender->getId()) return false;

    return sender->transferToOtherUser(fromAccountId, receiver, toAccountId, amount, note);
}

// Xuất dữ liệu ra file CSV
void App::exportDataCSV(const string &filename)
{
    cout << "No data";
}
const std::vector<User*>& App::getAllUsers() const
{
    return users;
}
  
>>>>>>> master

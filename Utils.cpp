#include "Utils.h"
#include <ctime>

using namespace std;

// Lấy ngày hiện tại và chuyển sang string YYYY-MM-DD
string getToday() {
    time_t t = time(NULL);
    tm *timePtr = localtime(&t);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", timePtr);
    return string(buf);
}

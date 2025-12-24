#include "App.h"
#include "Menu.h"
#include "Utils.h"

int main() {
    App app;
    Menu menu(app);
    menu.run();
    return 0;
}

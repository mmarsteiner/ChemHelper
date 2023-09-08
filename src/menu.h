#ifndef CHEMHELPER_MENU_H
#define CHEMHELPER_MENU_H

#include "modules/ChemModule.h"

class Menu {
private:
    int numItems;
    ChemModule *menuItems[10];
    char title[100];
    int selectedItem;

    void checkAndRunItem(int item, bool *exitVar);
    void render();
    void moveCursor(int direction);
public:
    Menu(const char *title);
    ~Menu();

    void addItem(ChemModule *module);
    void start();
};

#endif
#include "menu.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include <ti/getcsc.h>
#include <ti/screen.h>
#include "string.h"
#include "modules/ChemModule.h"
#include "debug.h"


Menu::Menu(const char *title) {
    strcpy(this->title, title);
    numItems = 0;
    selectedItem = 1;
}
Menu::~Menu() {
    for(int i = 0; i < numItems; i++) {
        delete menuItems[i];
    }
}

//not proud of this but i also didnt want to type it a ton of times
void Menu::checkAndRunItem(int item, bool *exitVar) {
    if(exitVar == nullptr || menuItems[item-1] == nullptr) {
        return;
    }
    if(numItems >= item) {
        *exitVar = true;
        menuItems[item-1]->run();
    }
}

void Menu::addItem(ChemModule *module) {
    if(numItems+1 > 10) {
        return;
    }
    menuItems[(++numItems)-1] = module;
}

void Menu::render() {
    os_ClrHomeFull();
    os_DisableCursor();
    os_SetCursorPos(0, 0);
    os_PutStrFull(title);
    for(int i = 0; i < numItems; i++) {
        os_SetCursorPos(i+1, 0);
        os_PutStrFull(menuItems[i]->getModuleName());
    }
    os_SetCursorPos(1, 0);
}

//1 = up, -1 = down
void Menu::moveCursor(int direction) {
    if(direction > 0) {
        if(selectedItem < numItems) {
            os_SetCursorPos(++selectedItem, 0);
        }
    } else {
        if(selectedItem > 1) {
            os_SetCursorPos(--selectedItem, 0);
        }
    }
}

void Menu::start() {
    render();
    bool exit = false;
    while(!exit) {
        uint8_t key = os_GetCSC();
        switch(key) {
            case sk_Clear:
                exit = true;
                break;
            case sk_Up:
                moveCursor(1);
                break;
            case sk_Down:
                moveCursor(-1);
                break;
            case sk_Enter:
                if(selectedItem > 0 && selectedItem < numItems) {
                    exit = true;
                    menuItems[selectedItem - 1]->run();
                }
            case sk_1:
                checkAndRunItem(1, &exit);
                break;
            case sk_2:
                checkAndRunItem(2, &exit);
                break;
            case sk_3:
                checkAndRunItem(3, &exit);
                break;
            case sk_4:
                checkAndRunItem(4, &exit);
                break;
            case sk_5:
                checkAndRunItem(5, &exit);
                break;
            case sk_6:
                checkAndRunItem(6, &exit);
                break;
            case sk_7:
                checkAndRunItem(7, &exit);
                break;
            case sk_8:
                checkAndRunItem(8, &exit);
                break;
            case sk_9:
                checkAndRunItem(9, &exit);
                break;
            case sk_0:
                checkAndRunItem(10, &exit);
                break;
        }
    }
}
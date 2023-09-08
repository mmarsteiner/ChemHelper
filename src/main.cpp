#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>

#include "ptable.h"
#include "menu.h"
#include "modules/MolarMassModule.h"
#include "modules/ChemModule.h"
#include <debug.h>

int main(void)
{
    int *testNull = nullptr;
    dbg_printf("This is what a null pointer looks like: %p\n", testNull);
    dbg_printf("Pre initialization\n");
    Menu mainMenu("Chem Helper");
    dbg_printf("Menu object created\n");
    mainMenu.addItem(new MolarMassModule);
    dbg_printf("Module added to menu\n");
    mainMenu.start();
    dbg_printf("Menu started\n");
    /* Return 0 for success */
    return 0;
}

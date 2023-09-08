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
    Menu mainMenu("Chem Helper");
    mainMenu.addItem(new MolarMassModule);
    mainMenu.start();
    return 0;
}

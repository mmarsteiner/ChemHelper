//
// Created by Matthew on 29/08/2023.
//

#ifndef CHEMHELPER_MOLARMASSMODULE_H
#define CHEMHELPER_MOLARMASSMODULE_H

#include "ChemModule.h"

typedef enum ScanState {
    NORMAL, ALPHA, LOWER
} ScanState;

class MolarMassModule : public ChemModule {
private:
    char name[100] = "1. Molar Mass Finder";
public:
    MolarMassModule();
    ~MolarMassModule();

    char* getModuleName();
    void run();
};


#endif //CHEMHELPER_MOLARMASSMODULE_H

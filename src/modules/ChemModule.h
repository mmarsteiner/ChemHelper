//
// Created by Matthew on 29/08/2023.
//

#ifndef CHEMHELPER_CHEMMODULE_H
#define CHEMHELPER_CHEMMODULE_H


class ChemModule {
private:
    char defaultName[100] = "Default Module Name";
public:
    ChemModule();
    virtual ~ChemModule();

    virtual char* getModuleName();
    virtual void run();
};


#endif //CHEMHELPER_MODULE_H

//
// Created by Matthew on 29/08/2023.
//

#include "ChemModule.h"
#include "MolarMassModule.h"
#include "../ptable.h"
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ti/real.h>
#include <debug.h>

MolarMassModule::MolarMassModule() {}

MolarMassModule::~MolarMassModule() {}

char* MolarMassModule::getModuleName() {
    return name;
}

void float2str(float value, char *str)
{
    real_t tmp_real = os_FloatToReal(value);
    os_RealToStr(str, &tmp_real, 8, 1, 2);
}

//not proud of this but yeah its here
void processChar(char c, char *buff, int *buffWriteIdx, int *currCol, ScanState *scanState) {
    buff[(*buffWriteIdx)++] = c;
    buff[*buffWriteIdx] = '\0';
    os_SetCursorPos(2, 0);
    os_PutStrFull(buff);
    os_SetCursorPos(2, ++(*currCol));
    *scanState = NORMAL;
}

void MolarMassModule::run() {
    os_ClrHomeFull();
    os_DisableCursor();
    os_SetCursorPos(0, 0);
    os_PutStrFull(name);
    os_SetCursorPos(1, 0);
    os_PutStrFull("Input Formula:");
    os_SetCursorPos(2, 0);
    bool exit = false;
    char buff[100];
    int buffWriteIdx = 0;
    int currCol = 0;
    ScanState scanState = NORMAL;
    while(!exit) {
        uint8_t key = os_GetCSC();
        switch(key) {
            case sk_Clear:
                exit = true;
                break;
            case sk_Enter: {
                formula_tree_node *compound = parseFormula(buff);
                double molarMass = compound->getMolarMass();
                char ans[100];
                float2str((float) molarMass, ans);
                os_SetCursorPos(3, 0);
                os_PutStrFull(ans);
                break;
            }
            case sk_Del:
                buff[--buffWriteIdx] = '\0';
                currCol--;
                os_ClrHomeFull();
                os_DisableCursor();
                os_SetCursorPos(0, 0);
                os_PutStrFull(name);
                os_SetCursorPos(1, 0);
                os_PutStrFull("Input Formula:");
                os_SetCursorPos(2, 0);
                os_PutStrFull(buff);
                os_SetCursorPos(2, currCol);
                break;
            case sk_Alpha:
                scanState = ALPHA;
                break;
            case sk_2nd:
                scanState = LOWER;
                break;
            case sk_Math:
                if(scanState == ALPHA) {
                    processChar('A', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('a', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Apps:
                if(scanState == ALPHA) {
                    processChar('B', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('b', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Prgm:
                if(scanState == ALPHA) {
                    processChar('C', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('c', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Recip:
                if(scanState == ALPHA) {
                    processChar('D', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('d', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Sin:
                if(scanState == ALPHA) {
                    processChar('E', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('e', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Cos:
                if(scanState == ALPHA) {
                    processChar('F', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('f', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Tan:
                if(scanState == ALPHA) {
                    processChar('G', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('g', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Power:
                if(scanState == ALPHA) {
                    processChar('H', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('h', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Square:
                if(scanState == ALPHA) {
                    processChar('I', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('i', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Comma:
                if(scanState == ALPHA) {
                    processChar('J', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('j', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_LParen:
                if(scanState == ALPHA) {
                    processChar('K', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('k', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('(', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_RParen:
                if(scanState == ALPHA) {
                    processChar('L', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('l', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar(')', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Div:
                if(scanState == ALPHA) {
                    processChar('M', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('m', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Log:
                if(scanState == ALPHA) {
                    processChar('N', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('n', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_7:
                if(scanState == ALPHA) {
                    processChar('O', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('o', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('7', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_8:
                if(scanState == ALPHA) {
                    processChar('P', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('p', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('8', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_9:
                if(scanState == ALPHA) {
                    processChar('Q', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('q', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('9', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Mul:
                if(scanState == ALPHA) {
                    processChar('R', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('r', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Ln:
                if(scanState == ALPHA) {
                    processChar('S', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('s', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_4:
                if(scanState == ALPHA) {
                    processChar('T', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('t', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('4', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_5:
                if(scanState == ALPHA) {
                    processChar('U', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('u', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('5', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_6:
                if(scanState == ALPHA) {
                    processChar('V', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('v', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('6', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Sub:
                if(scanState == ALPHA) {
                    processChar('W', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('w', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_Store:
                if(scanState == ALPHA) {
                    processChar('X', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('x', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_1:
                if(scanState == ALPHA) {
                    processChar('Y', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('y', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('1', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_2:
                if(scanState == ALPHA) {
                    processChar('Z', buff, &buffWriteIdx, &currCol, &scanState);
                } else if(scanState == LOWER) {
                    processChar('z', buff, &buffWriteIdx, &currCol, &scanState);
                } else {
                    processChar('2', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_3:
                if(scanState == NORMAL) {
                    processChar('3', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            case sk_0:
                if(scanState == NORMAL) {
                    processChar('0', buff, &buffWriteIdx, &currCol, &scanState);
                }
                break;
            default:
                break;
        }
    }
}
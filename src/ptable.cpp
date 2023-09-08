//
// Created by Matthew on 29/08/2023.
//

#include "ptable.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "tice.h"

int getAtomicNumber(const char* symbol) {
    const char elementSymbols[118][3] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Cn","Nh","Fl","Mc","Lv","Ts","Og"};
    for(int i = 0; i < 118; i++) {
        if(strncmp(elementSymbols[i], symbol, 2) == 0) {
            return i + 1;
        }
    }
    return -1;
}

int processNewElementNode(char *buff, int quantity, tree_node_combination* currNode) {
    int atomicNum = getAtomicNumber(buff);
    if(atomicNum == -1) {
        return -1;
    }
    auto *newNode = new tree_node_element(atomicNum, quantity);
    currNode->addChild(newNode);
    buff[0] = '\0';
    return 0;
}

formula_tree_node* parseFormula(const char *in) {
    int inputLen = strlen(in);
    char buff[3] = {'\0'};
    auto *top = new tree_node_combination();
    top->quantity = 1;
    auto *currentNode = top;
    for(int i = 0; i < inputLen; i++) {
        char c = in[i];
        if('A' <= c && c <= 'Z') {
            if(buff[0] == '\0') {
                buff[0] = c;
                buff[1] = '\0';
            } else {
                processNewElementNode(buff, 1, currentNode);
                buff[0] = c;
                buff[1] = '\0';
            }
        }
        else if('a' <= c && c <= 'z') {
            buff[1] = c;
            buff[2] = '\0';
        }
        else if('0' <= c && c <= '9') {
            int num = c - '0';
            if(buff[0] != '\0') {
                processNewElementNode(buff, num, currentNode);
            }
            else if(i > 0 && '0' <= in[i-1] && in[i-1] <= '9') {
                formula_tree_node *lastChild = currentNode->getChildAtIndex(currentNode->numChildren - 1);
                lastChild->quantity = (lastChild->quantity * 10) + c - '0';
            }
            else {
                currentNode->quantity = num;
                if(currentNode->getParent()) {
                    currentNode = (tree_node_combination*) currentNode->getParent();
                } else {
                    //printf("error, number with no preceding element or group\n");
                }
            }
        }
        else if(c == '(') {
            if(buff[0] != '\0') {
                processNewElementNode(buff, 1, currentNode);
            }
            auto *newNode = new tree_node_combination();
            currentNode->addChild(newNode);
            currentNode = newNode;
        }
        else if(c == ')') {
            if(buff[0] != '\0') {
                processNewElementNode(buff, 1, currentNode);
            }
            char nextChar = in[i+1];
            int groupQuantity = 1;
            if('0' <= nextChar && nextChar <= '9') {
                groupQuantity = nextChar - '0';
                i++;
            }
            currentNode->quantity = groupQuantity;
            if(currentNode->getParent()) {
                currentNode = (tree_node_combination*) currentNode->getParent();
            } else {
                //printf("error, unmatched close parenthesis");
            }
        }
    }
    if(buff[0] != '\0') {
        processNewElementNode(buff, 1, currentNode);
    }
    return top;
}

//Constructor implementations

formula_tree_node::formula_tree_node(int quantity) {
    this->quantity = quantity;
    numChildren = 0;
    children = nullptr;
    parent = nullptr;
}

tree_node_combination::tree_node_combination() : formula_tree_node(0) {
    this->type = COMBINATION;
}

tree_node_element::tree_node_element(int atomicNum, int quantity) : formula_tree_node(quantity) {
    this->atomicNum = atomicNum;
    this->type = ELEMENT;
}

//destructor implementations

formula_tree_node::~formula_tree_node() {
    for(int i = 0; i < numChildren; i++) {
        delete children[i];
    }
    free(children);
}

tree_node_element::~tree_node_element() {}
tree_node_combination::~tree_node_combination() {}

//class method implementations

formula_tree_node *formula_tree_node::getParent() {
    return parent;
}

void formula_tree_node::addChild(formula_tree_node *child) {
    children = (formula_tree_node**) realloc(children, ++numChildren * sizeof(formula_tree_node*));
    children[numChildren - 1] = child;
    child->parent = this;
}

void formula_tree_node::removeChildAtIndex(int i) {
    if(i >= numChildren || i < 0) return;
    for(int j = i + 1; j < numChildren; j++) {
        children[j - 1] = children [j];
    }
    delete children[numChildren - 1];
    children = (formula_tree_node**) realloc(children, --numChildren * sizeof(formula_tree_node*));
}

formula_tree_node *formula_tree_node::getChildAtIndex(int i) {
    return children[i];
}

void tree_node_combination::toString(char *out) {
    char buff[100] = "";
    if(quantity != 1) {
        strcat(buff, "(");
    }
    for(int i = 0; i < numChildren; i++) {
        char tmpbuff[100] = "";
        children[i]->toString(tmpbuff);
        strcat(buff, tmpbuff);
    }
    if(quantity != 1) {
        strcat(buff, ")");
        char quantityString[20] = "";
        sprintf(quantityString, "%d", quantity);
        strcat(buff, quantityString);
    }
    strcpy(out, buff);
}

double tree_node_combination::getMolarMass() {
    double total = 0.0;
    for(int i = 0; i < numChildren; i++) {
        total += children[i]->getMolarMass();
    }
    return total * quantity;
}

void tree_node_element::toString(char *out) {
    const char elementSymbols[118][3] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Cn","Nh","Fl","Mc","Lv","Ts","Og"};
    char buff[100] = "";
    strcat(buff, elementSymbols[atomicNum - 1]);
    if(quantity != 1) {
        char quantityString[20] = "";
        sprintf(quantityString, "%d", quantity);
        strcat(buff, quantityString);
    }
    strcpy(out, buff);
}

double tree_node_element::getMolarMass() {
    const double atomicWeights[118] = {1.007,4.002,6.941,9.012,10.811,12.011,14.007,15.999,18.998,20.18,22.99,24.305,26.982,28.086,30.974,32.065,35.453,39.948,39.098,40.078,44.956,47.867,50.942,51.996,54.938,55.845,58.933,58.693,63.546,65.38,69.723,72.64,74.922,78.96,79.904,83.798,85.468,87.62,88.906,91.224,92.906,95.96,98,101.07,102.906,106.42,107.868,112.411,114.818,118.71,121.76,127.6,126.904,131.293,132.905,137.327,138.905,140.116,140.908,144.242,145,150.36,151.964,157.25,158.925,162.5,164.93,167.259,168.934,173.054,174.967,178.49,180.948,183.84,186.207,190.23,192.217,195.084,196.967,200.59,204.383,207.2,208.98,210,210,222,223,226,227,232.038,231.036,238.029,237,244,243,247,247,251,252,257,258,259,262,261,262,266,264,267,268,271,272,285,284,289,288,292,295,294};
    return atomicWeights[atomicNum-1] * quantity;
}






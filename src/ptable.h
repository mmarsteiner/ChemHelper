//
// Created by Matthew on 29/08/2023.
//

#ifndef CHEMHELPER_PTABLE_H
#define CHEMHELPER_PTABLE_H

#define INITIAL_NUM_CHILDREN 10

typedef enum tree_node_type {
    COMBINATION, ELEMENT
} tree_node_type;

class formula_tree_node {
protected:
    formula_tree_node** children;
    formula_tree_node* parent;
public:
    int numChildren;
    int quantity;
    tree_node_type type;
    formula_tree_node* getParent();
    formula_tree_node* getChildAtIndex(int i);
    void removeChildAtIndex(int i);
    void addChild(formula_tree_node* child);

    virtual double getMolarMass() = 0;

    formula_tree_node(int quantity);
    virtual ~formula_tree_node();


    virtual void toString(char *out) = 0;
};

class tree_node_combination : public formula_tree_node {
public:
    void toString(char *out);
    double getMolarMass();
    tree_node_combination();
    ~tree_node_combination();
};

class tree_node_element : public formula_tree_node {
public:
    int atomicNum;
    double getMolarMass();
    void toString(char *out);
    tree_node_element(int atomicNum, int quantity);
    ~tree_node_element();
};

formula_tree_node* parseFormula(const char *in);

int getAtomicNumber(const char* symbol);


#endif //CHEMHELPER_PTABLE_H

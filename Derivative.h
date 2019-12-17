#ifndef PROGRAMMING_LANGUAGE_DERIVATIVE_H
#define PROGRAMMING_LANGUAGE_DERIVATIVE_H

#include "Commons.h"
#include "Node.h"
#include "Tree_t\Tree.cpp"

class Differentiator {

public:

    int diffVarCode;

    Differentiator(int variable) : diffVarCode(variable) {}

    bool isVariable(Tree<Node>* node);

    Tree<Node>* diff(Tree<Node>* node);

    void replaceBy(size_t child, Tree<Node>* node);

    bool optimization(Tree<Node>* tree);

    bool optimizationCalc(Tree<Node>* node);

    bool optimizationZero(Tree<Node>* node);

    bool optimizationOne(Tree<Node>* node);

    void makeNumber(double, Tree<Node>* node);
};

bool Differentiator::isVariable(Tree<Node> *node) {
    if (node->getValue().type == Node::VARIABLE_TYPE && node->getValue().code == diffVarCode) {
        return true;
    }
    if (!node->childIsEmpty(LEFT_CHILD)) {
        return isVariable(node->getChild(LEFT_CHILD));
    }
    if (!node->childIsEmpty(RIGHT_CHILD)) {
        return isVariable(node->getChild(RIGHT_CHILD));
    }
    return false;
}

void Differentiator::makeNumber(double num, Tree<Node> *node) {
    node->removeSubTree(LEFT_CHILD );
    node->removeSubTree(RIGHT_CHILD);
    node->setValue(Node(num));
}

void Differentiator::replaceBy(size_t child, Tree<Node>* node) {
    node->setValue(node->getChild(child)->getValue());
    size_t not_child = (child == RIGHT_CHILD) ? LEFT_CHILD : RIGHT_CHILD;
    Tree<Node>* subtreeCopy = nullptr;

    node->removeSubTree(not_child);
    if (!node->getChild(child)->childIsEmpty(not_child)) {
        subtreeCopy = node->getChild(child)->getChild(not_child)->copySubtree();
        node->connectSubtree(not_child, subtreeCopy);
    }
    if (!node->getChild(child)->childIsEmpty(child)) {
        subtreeCopy = node->getChild(child)->getChild(child)->copySubtree();
        node->removeSubTree(child);
        node->connectSubtree(child, subtreeCopy);
    } else {
        node->removeSubTree(child);
    }
}

///////////Domain Specific Language////////////
#define dL diff(node->getChild(LEFT_CHILD))
#define dR diff(node->getChild(RIGHT_CHILD))
#define L node->getChild(LEFT_CHILD )->copySubtree()
#define R node->getChild(RIGHT_CHILD)->copySubtree()
#define it_is(CODE) (node->getValue().code == CODE)
#define newOperation(op) new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, op))

#define overrideBinaryOperator(op, CODE)\
                                    Tree<Node>* operator op (Tree<Node>& l, Tree<Node>& r) {\
                                        Tree<Node>* res = newOperation(CODE);\
                                        res->connectSubtree(LEFT_CHILD , &l);\
                                        res->connectSubtree(RIGHT_CHILD, &r);\
                                        return res;\
                                    }\
                                    Tree<Node>* operator op (Tree<Node>& l, double num) {\
                                        Tree<Node>* res = newOperation(CODE);\
                                        res->connectSubtree(LEFT_CHILD , &l);\
                                        res->connectSubtree(RIGHT_CHILD, new Tree<Node>(Node(num)));\
                                        return res;\
                                    }\
                                    Tree<Node>* operator op (double num, Tree<Node>& r) {\
                                        Tree<Node>* res = newOperation(CODE);\
                                        res->connectSubtree(RIGHT_CHILD , &r);\
                                        res->connectSubtree(LEFT_CHILD, new Tree<Node>(Node(num)));\
                                        return res;\
                                    }

    overrideBinaryOperator(+, ADDITION)

    overrideBinaryOperator(-, MINUS)

    overrideBinaryOperator(*, MULTIPLY)

    overrideBinaryOperator(/, DIVIDE)

    overrideBinaryOperator(^, POWER)

    Tree<Node>* operator - (Tree<Node>& node) {
        Tree<Node>* minus = newOperation(MINUS);
        minus->growChild(LEFT_CHILD, Node(0));
        minus->connectSubtree(RIGHT_CHILD, &node);
        return minus;
    }

Tree<Node>* Differentiator::diff(Tree<Node>* node) {
    switch (node->getValue().type) {
        case Node::NUMBER_TYPE: {
            return new Tree<Node>(Node(0));
        }
        case Node::VARIABLE_TYPE: {
            if (node->getValue().code == diffVarCode) {
                return new Tree<Node>(Node(1));
            } else {
                return new Tree<Node>(Node(0));
            }
        }
        case Node::SPECIAL_SYMBOLS: {
            if (it_is(ADDITION)) {
                return *dL + *dR;
            }
            if (it_is(MINUS)) {
                return *dL - *dR;
            }
            if (it_is(MULTIPLY)) {
                return *(*dL * *R) + *(*L * *dR);
            }
            if (it_is(DIVIDE)) {
                return *(*(*dL * *R) - *(*L * *dR)) / *(*R ^ 2);
            }
            if (it_is(POWER)) { //L^R
                bool baseIsVar  = isVariable(node->getChild(LEFT_CHILD));
                bool powerIsVar = isVariable(node->getChild(RIGHT_CHILD));
                if (baseIsVar) {
                    return *(*R * *(*L ^ *(*R - 1))) * *dL;
                }else if (!powerIsVar)
                    return new Tree<Node>(Node(0));
            }
            break;
        }
    }
    return nullptr;
}

bool Differentiator::optimization(Tree<Node>* tree) {
    bool modified = true;

    while(modified) {

        modified = false;

        Tree<Node>** sequence = tree->allocTree();

        tree->postorder(sequence);

        for (int i = 0; i < tree->getSize(); ++i) {

            if (sequence[i]->getValue().type == Node::SPECIAL_SYMBOLS) {
                modified = modified || optimizationCalc(sequence[i]);
                modified = modified || optimizationZero(sequence[i]);
                modified = modified || optimizationOne (sequence[i]);
            }

        }
        free(sequence);
    }
}

#define LEFT_IS(arg)  node->getChild(LEFT_CHILD )->getValue().type == Node::NUMBER_TYPE && node->getChild(LEFT_CHILD )->getValue().num == arg
#define RIGHT_IS(arg) node->getChild(RIGHT_CHILD)->getValue().type == Node::NUMBER_TYPE && node->getChild(RIGHT_CHILD)->getValue().num == arg

bool Differentiator::optimizationZero(Tree<Node> *node) {
    if (it_is(ADDITION)) {
        if (LEFT_IS(0)) {
            replaceBy(RIGHT_CHILD, node);
            return true;
        } else if (RIGHT_IS(0)) {
            replaceBy(LEFT_CHILD, node);
            return true;
        }
    }
    if (it_is(MULTIPLY)) {
        if ((LEFT_IS(0)) || (RIGHT_IS(0))) {
            makeNumber(0, node);
            return true;
        }
    }
    if (it_is(MINUS)) {
        if (RIGHT_IS(0)) {
            replaceBy(LEFT_CHILD, node);
            return true;
        }
    }
    if (it_is(POWER)) {
        if (RIGHT_IS(0)) {
            makeNumber(1, node);
            return true;
        }
        if (LEFT_IS(0)) {
            makeNumber(0, node);
            return true;
        }
    }
    if (it_is(DIVIDE)) {
        if (LEFT_IS(0)) {
            makeNumber(0, node);
            return true;
        }
    }
    return false;
}

bool Differentiator::optimizationOne(Tree<Node> *node) {
    if (it_is(MULTIPLY)) {
        if (RIGHT_IS(1)) {
            replaceBy(LEFT_CHILD, node);
            return true;
        }
        if (LEFT_IS(1)) {
            replaceBy(RIGHT_CHILD, node);
            return true;
        }
    }
    if (it_is(DIVIDE)) {
        if (RIGHT_IS(1)) {
            replaceBy(LEFT_CHILD, node);
            return true;
        }
    }
    if (it_is(POWER)) {
        if (RIGHT_IS(1)) {
            replaceBy(LEFT_CHILD, node);
            return true;
        }
        if (LEFT_IS(1)) {
            node->removeSubTree(LEFT_CHILD );
            node->removeSubTree(RIGHT_CHILD);
            node->setValue(Node(1));
            return true;
        }
    }
    return false;
}

#define  computable node->getChild(LEFT_CHILD)->getValue().type == Node::NUMBER_TYPE && node->getChild(RIGHT_CHILD)->getValue().type == Node::NUMBER_TYPE

bool Differentiator::optimizationCalc(Tree<Node> *node) {
    switch (node->getValue().code) {
        case ADDITION:
            if (computable) {
                double value = node->getChild(LEFT_CHILD)->getValue().num + node->getChild(RIGHT_CHILD)->getValue().num;
                makeNumber(value, node);
                return true;
            }
            break;
        case MINUS:
            if (computable) {
                double value = node->getChild(LEFT_CHILD)->getValue().num - node->getChild(RIGHT_CHILD)->getValue().num;
                makeNumber(value, node);
                return true;
            }
            break;
        case MULTIPLY:
            if (computable) {
                double value = node->getChild(LEFT_CHILD)->getValue().num * node->getChild(RIGHT_CHILD)->getValue().num;
                makeNumber(value, node);
                return true;
            }
            break;
        case DIVIDE:
            if (computable) {
                double value = node->getChild(LEFT_CHILD)->getValue().num / node->getChild(RIGHT_CHILD)->getValue().num;
                makeNumber(value, node);
                return true;
            }
            break;
        case POWER:
            if (computable) {
                double value = pow(node->getChild(LEFT_CHILD)->getValue().num, node->getChild(RIGHT_CHILD)->getValue().num);
                makeNumber(value, node);
                return true;
            }
            break;
    }
    return false;
}

#undef computable
#undef dL
#undef dR
#undef L
#undef R
#undef overrideBinaryOperator
#undef overrideUnaryOperator
#undef it_is
#undef newOperation
#undef LEFT_IS
#undef RIGHT_IS

#endif //PROGRAMMING_LANGUAGE_DERIVATIVE_H

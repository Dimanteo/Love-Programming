#ifndef PROGRAMMING_LANGUAGE_NODE_H
#define PROGRAMMING_LANGUAGE_NODE_H

#include <cmath>

struct Node {
    enum TYPE {POISON_TYPE = 0, NUMBER_TYPE = 'N', OPERATION_TYPE = 'O', VARIABLE_TYPE = 'V', SPECIAL_SYMBOLS = 'S'};

    TYPE type;
    union {
        int code;
        double num;
    };

    Node() : type(POISON_TYPE), num(NAN){}

    Node(double number) : type(NUMBER_TYPE), num(number){}

    Node(TYPE objType, int objectCode) : type(objType), code(objectCode) {}

    ~Node() {
        type = POISON_TYPE;
        code = 0;
        num = 0;
    }
};

#endif //PROGRAMMING_LANGUAGE_NODE_H

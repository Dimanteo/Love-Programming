#ifndef PROGRAMMING_LANGUAGE_COMMONS_H
#define PROGRAMMING_LANGUAGE_COMMONS_H

enum STD_CODE {
    DOT_COMA,
    COMA_PARAMETER,
    ASSIGN,
    OP,
    RET,
    IF,
    IF_ELSE,
    WHILE,
    MORE,
    LESS,
    MORE_EQUAL,
    LESS_EQUAL,
    EQUAL,
    NOT_EQUAL,
    ADDITION,
    MINUS,
    DIVIDE,
    MULTIPLY,
    POWER,
    GET,
    PUT,
    DERIV //производная
};

struct STD_object {
    STD_CODE code;
    char ID[8];
};

STD_object STD_ObjList[] {
        {DOT_COMA,       ";"},
        {COMA_PARAMETER, ","},
        {ASSIGN,         "="},
        {OP,             "op"},
        {RET,            "ret"},
        {IF,             "if"},
        {IF_ELSE,        "if-else"},
        {WHILE,          "while"},
        {MORE,           ">"},
        {LESS,           "<"},
        {MORE_EQUAL,     ">="},
        {LESS_EQUAL,     "<="},
        {EQUAL,          "=="},
        {NOT_EQUAL,      "!="},
        {ADDITION,       "+"},
        {MINUS,          "-"},
        {DIVIDE,         "/"},
        {MULTIPLY,       "*"},
        {POWER,          "^"},
        {GET,            "get"},
        {PUT,            "put"},
        {DERIV,          "deriv"}
};

int STD_FUNC_COUNT = sizeof(STD_ObjList) / sizeof(STD_ObjList[0]);

#endif //PROGRAMMING_LANGUAGE_COMMONS_H

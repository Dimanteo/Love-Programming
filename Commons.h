#ifndef PROGRAMMING_LANGUAGE_COMMONS_H
#define PROGRAMMING_LANGUAGE_COMMONS_H
/*const char FUNC_DEF[] = "чтоб не нести вдоль ночных дорог";
const char MAIN[] = "я так привыкла жить одним тобой";
const char BEGIN[] = "и пусть";
const char END[] = "не потеряй его и не сломай";
const char GET[] = "мой нежный мальчик";
const char PUT[] = "сквозь сезы прошептала";
const char WHILE[] = "и повторять лишь одно";
const char IF[] = "знаешь ли ты";
const char ASSIGNMENT[] = "его теперь";
const char RETURN_PREFIX[] = "не потеряй";
const char RETURN_POSTFIX[] = "и не сломай";
const char DIFFER[] = "производную брала не жалея";*/

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
        {ASSIGN,         "=="},
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

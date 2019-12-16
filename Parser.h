#ifndef PROGRAMMING_LANGUAGE_PARSER_H
#define PROGRAMMING_LANGUAGE_PARSER_H

#include <cctype>
#include <mem.h>
#include "Tree_t/Tree.cpp"
#include "Node.h"
#include "Commons.h"
#include "Maksim.h"

#define this_is_cmd(VALUE) (tok_str[tok_ptr].type == Token::CMD && strcmp(Language_CMD[tok_str[tok_ptr].code], VALUE) == 0)
#define this_is(TYPE, CODE) (tok_str[tok_ptr].type == Token::TYPE && tok_str[tok_ptr].code == CODE)
#define syntax_assert fprintf(stderr, "Syntax error\n%d %d\n", tok_str[tok_ptr].type, tok_str[tok_ptr].code);  assert(0);
#define check_assert(cond) \
if (!(cond)) {\
    syntax_assert\
}

const size_t MAX_ID_COUNT = 1000;
int ID_COUNT = 0;
char* ID_VECTOR[MAX_ID_COUNT];

struct LexicalAnalizator {
    static const size_t MAX_NAME_LENGTH = 500;
    char* str;

    struct Token {
        enum TYPE {NUMBER, ID, CMD, SYMBOL};
        TYPE type;
        union {
            int code;
            double num;
        };

        Token (TYPE _type, int _code) : type(_type), code(_code){}
        Token (double value) : type(NUMBER), num(value){}
    };
    Token* tok_str;
    int tok_size = 0;

    LexicalAnalizator(char* data) : str(data) {
        tok_str = (Token*)calloc(strlen(str), sizeof(tok_str[0]));
        tok_size = 0;
    }
    ~LexicalAnalizator() {
        free(tok_str);
        for (int i = 0; i < ID_COUNT; ++i) {
            free(ID_VECTOR[i]);
        }
    }

    void tokenize();

    void tokNum();

    void getStr();

    bool getSTD();

    void dump();

    //<---------Recursive descent--------->

    int tok_ptr = 0;

    Tree<Node>* getG();

    Tree<Node>* getExp();

    Tree<Node>* getDef();

    Tree<Node>* getMain();

    Tree<Node>* getBody();

    Tree<Node>* getVarEnum();

    Tree<Node>* getOp();

    Tree<Node>* getIf();

    Tree<Node>* getWhile();

    Tree<Node>* getAssign();

    Tree<Node>* getGet();

    Tree<Node>* getPut();

    Tree<Node>* getEquation();

    Tree<Node>* getCondition();

    Tree<Node>* getSign();

    Tree<Node>* getAbs();

    Tree<Node>* getT();

    Tree<Node>* getPow();

    Tree<Node>* getP();

    Tree<Node>* getName();

    Tree<Node>* getCall();

    Tree<Node>* getArgEnum();
};

void LexicalAnalizator::tokenize() {

    while (*str != '\0') {

        if (*str == ' ' || *str == '\t') {
            str++;
            continue;
        }

        if (isdigit(*str)) {
            tokNum();
            continue;
        }

        if (strchr("(,)\n+-=*/^><!", *str) != nullptr) {
            tok_str[tok_size++] = Token(Token::SYMBOL, *str);
            str++;
            continue;
        }

        if (isalpha(*str) || iswalpha(*(wchar_t*)str)) {
            getStr();
            continue;
        }
    }
}

void LexicalAnalizator::tokNum() {

    double value = 0;

    do {
        value = value * 10 + (double)(*str - '0');
        str++;
    } while (isdigit(*str));

    if (*str == '.') {
        str++;
        int power = 1;
        while ('0' <= *str && *str <= '9') {
            value = value + (double) (*str - '0') / (power * 10);
            power *= 10;
            str++;
        }
    }

    tok_str[tok_size++] = Token(value);
}

void LexicalAnalizator::getStr() {
    if (getSTD())
        return;
    char buffer[MAX_NAME_LENGTH] = "";
    int offset = 0;
    sscanf(str, "%s%n", buffer, &offset);
    str += offset;
    ID_VECTOR[ID_COUNT] = strdup(buffer);
    tok_str[tok_size++] = Token(Token::ID, ID_COUNT++);
}

bool LexicalAnalizator::getSTD() {
    for (int i = 0; i < LCMD_COUNT; ++i) {
        int length = strlen(Language_CMD[i]);
        if (strncmp(str, Language_CMD[i], length) == 0) {
            tok_str[tok_size++] = Token(Token::CMD, i);
            str += length;
            return true;
        }
    }
    return false;
}

void LexicalAnalizator::dump() {
    FILE* file = fopen("Parser.log", "wb");
    fprintf(file, "Dump begin\n");
    for (int i = 0; i < tok_size; ++i) {
        fprintf(file, "[%d] TYPE = ", i);
        switch (tok_str[i].type) {
            case Token::NUMBER:
                fprintf(file, "number; value = %lg", tok_str[i].num);
                break;
            case Token::ID:
                fprintf(file, "id; value = %s[%d]", ID_VECTOR[tok_str[i].code], tok_str[i].code);
                break;
            case Token::SYMBOL:
                fprintf(file, "symbol; value = %c", tok_str[i].code);
                break;
            case Token::CMD:
                fprintf(file, "command; %s", Language_CMD[tok_str[i].code]);
                break;
        }
        fprintf(file, "\n");
    }
    fprintf(file, "Dump end\n");
    fclose(file);
}

Tree<Node> *LexicalAnalizator::getG() {
    Tree<Node>* exp = getExp();
    check_assert(tok_ptr == tok_size)
    return  exp;
}

Tree<Node>* LexicalAnalizator::getExp() {
    Tree<Node>* start = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, DOT_COMA));
    Tree<Node>* global = start;
    while (this_is_cmd(LFUNC_DEF)) {
        global->growChild(RIGHT_CHILD, Node(Node::SPECIAL_SYMBOLS, DOT_COMA));
        global->getChild(RIGHT_CHILD)->connectSubtree(LEFT_CHILD, getDef());
        global = global->getChild(RIGHT_CHILD);
    }
    check_assert(this_is_cmd(LMAIN))
    start->connectSubtree(LEFT_CHILD, getMain());
    while (this_is_cmd(LFUNC_DEF)) {
        global->growChild(RIGHT_CHILD, Node(Node::SPECIAL_SYMBOLS, DOT_COMA));
        global->getChild(RIGHT_CHILD)->connectSubtree(LEFT_CHILD, getDef());
        global = global->getChild(RIGHT_CHILD);
    }
    return start;
}

Tree<Node> *LexicalAnalizator::getDef() {
    if (this_is_cmd(LFUNC_DEF)) {
        tok_ptr++;
        check_assert(tok_str[tok_ptr].type == Token::ID)
        Tree<Node>* def = new Tree<Node>(Node(Node::OPERATION_TYPE, tok_str[tok_ptr].code));
        tok_ptr++;
        def->connectSubtree(LEFT_CHILD, getVarEnum());
        def->connectSubtree(RIGHT_CHILD, getBody());
        return def;
    } else {
        syntax_assert
    }
}

Tree<Node> *LexicalAnalizator::getMain() {
    check_assert(this_is_cmd(LMAIN))
    tok_ptr++;
    ID_VECTOR[ID_COUNT] = strdup("main");
    Tree<Node>* def = new Tree<Node>(Node(Node::OPERATION_TYPE, ID_COUNT++));
    check_assert(this_is(SYMBOL, '\n'))
    tok_ptr++;
    def->connectSubtree(RIGHT_CHILD, getBody());
    return def;
}

Tree<Node> *LexicalAnalizator::getVarEnum() {
    check_assert(this_is(SYMBOL, '('))
    tok_ptr++;
    check_assert(tok_str[tok_ptr].type == Token::ID)
    Tree<Node>* arg = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, COMA_PARAMETER));
    arg->growChild(RIGHT_CHILD, Node(Node::VARIABLE_TYPE, tok_str[tok_ptr].code));
    tok_ptr++;
    Tree<Node>* nextArg = arg;
    while (this_is(SYMBOL, ',')) {
        tok_ptr++;
        check_assert(tok_str[tok_ptr].type == Token::ID)
        nextArg = nextArg->growChild(LEFT_CHILD, Node(Node::SPECIAL_SYMBOLS, COMA_PARAMETER));
        nextArg->growChild(RIGHT_CHILD, Node(Node::VARIABLE_TYPE, tok_str[tok_ptr].code));
        tok_ptr++;
    }
    check_assert(this_is(SYMBOL, ')'))
    return arg;
}

Tree<Node> *LexicalAnalizator::getBody() {
    check_assert(this_is_cmd(LBEGIN))
    tok_ptr++;
    check_assert(this_is(SYMBOL, '\n'))
    tok_ptr++;
    Tree<Node> *Op = nullptr;
    while (!this_is_cmd(LEND)) {
        while (this_is(SYMBOL, '\n'))
            tok_ptr++;
        if (this_is_cmd(LEND))
            return nullptr;
        Op = getOp();
        check_assert(this_is(SYMBOL, '\n'))
        tok_ptr++;
    }
    check_assert(this_is_cmd(LEND))
    tok_ptr++;
    return Op;
}

Tree<Node> *LexicalAnalizator::getOp() {
    if (this_is_cmd(LIF)) {
        return  getIf();
    } else if (this_is_cmd(LWHILE)) {
        return getWhile();
    } else if (this_is_cmd(LGET)) {
        return getGet();
    } else if (this_is_cmd(LPUT)) {
        return getPut();
    } else if (tok_str[tok_ptr].code == Token::ID) {
        return getAssign();
    }
    syntax_assert
}

Tree<Node> *LexicalAnalizator::getAssign() {
    check_assert(tok_str[tok_ptr].type == Token::ID)
    Tree<Node>* id = new Tree<Node>(Node(Node::VARIABLE_TYPE, tok_str[tok_ptr].code));
    tok_ptr++;
    check_assert(this_is_cmd(LASSIGNMENT))
    tok_ptr++;
    Tree<Node>* ass = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, ASSIGN));
    ass->connectSubtree(LEFT_CHILD, id);
    ass->connectSubtree(RIGHT_CHILD, getEquation());
    return ass;
}

Tree<Node> *LexicalAnalizator::getGet() {
    check_assert(this_is_cmd(LGET))
    tok_ptr++;
    Tree<Node>* get = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, GET));
    check_assert(tok_str[tok_ptr].type == Token::ID)
    get->growChild(LEFT_CHILD, Node(Node::VARIABLE_TYPE, tok_str[tok_ptr].code));
    tok_ptr++;
    return get;
}

Tree<Node> *LexicalAnalizator::getPut(){
    if (!this_is_cmd(LPUT)) {
        syntax_assert
    }
    tok_ptr++;
    Tree<Node>* put = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, PUT));
    if (tok_str[tok_ptr].type != Token::ID) {
        syntax_assert
    }
    put->growChild(LEFT_CHILD, Node(Node::VARIABLE_TYPE, tok_str[tok_ptr].code));
    return put;
}

Tree<Node> *LexicalAnalizator::getIf() {
    check_assert(this_is_cmd(LIF))
    tok_ptr++;
    Tree<Node>* branch = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, IF));
    branch->connectSubtree(LEFT_CHILD, getCondition());
    branch->growChild(RIGHT_CHILD, Node(Node::SPECIAL_SYMBOLS, IF_ELSE));
    branch->getChild(RIGHT_CHILD)->connectSubtree(LEFT_CHILD, getBody());
    if (this_is_cmd(LELSE)) {
        tok_ptr++;
        branch->getChild(RIGHT_CHILD)->connectSubtree(RIGHT_CHILD, getBody());
    }
    return branch;
}

Tree<Node> *LexicalAnalizator::getWhile() {
    check_assert(this_is_cmd(LWHILE))
    tok_ptr++;
    Tree<Node>* cycle = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, WHILE));
    cycle->connectSubtree(LEFT_CHILD, getCondition());
    cycle->connectSubtree(RIGHT_CHILD, getBody());
    return cycle;
}

Tree<Node> *LexicalAnalizator::getCondition() {
    check_assert(this_is(SYMBOL, '('))
    tok_ptr++;
    Tree<Node>* left = getEquation();
    Tree<Node>* cmp = nullptr;
    check_assert(tok_str[tok_ptr].type == Token::SYMBOL)
    switch (tok_str[tok_ptr].code) {
        case '>':
            tok_ptr++;
            if (this_is(SYMBOL, '=')) {
                tok_ptr++;
                cmp = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, MORE_EQUAL));
            } else {
                cmp = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, MORE));
            }
            break;
        case '<':
            tok_ptr++;
            if (this_is(SYMBOL, '=')) {
                tok_ptr++;
                cmp = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, LESS_EQUAL));
            } else {
                cmp = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, LESS));
            }
            break;
        case '=':
            tok_ptr++;
            check_assert(this_is(SYMBOL, '='))
            tok_ptr++;
            cmp = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, EQUAL));
            break;
        case '!':
            tok_ptr++;
            check_assert(this_is(SYMBOL, '='))
            tok_ptr++;
            cmp = new Tree<Node> (Node(Node::SPECIAL_SYMBOLS, NOT_EQUAL));
            break;
        default:
            syntax_assert
    }
    cmp->connectSubtree(LEFT_CHILD, left);
    cmp->connectSubtree(RIGHT_CHILD, getEquation());
    return cmp;
}

Tree<Node> *LexicalAnalizator::getEquation() {
    return getSign();
}

Tree<Node>* LexicalAnalizator::getSign() {
    Tree<Node>* minus = nullptr;
    if (this_is(SYMBOL, '-')) {
        minus = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, MINUS));
        tok_ptr++;
    }
    Tree<Node>* abs = getAbs();
    if (minus != nullptr) {
        minus->connectSubtree(RIGHT_CHILD, abs);
        minus->growChild(LEFT_CHILD, Node(0));
        return minus;
    } else {
        return abs;
    }
}

Tree<Node>* LexicalAnalizator::getAbs() {
    Tree<Node>* val = getT();
    while (this_is(SYMBOL, '+') || this_is(SYMBOL, '-')) {
        int code = tok_str[tok_ptr].code;
        str++;
        Tree<Node>* val1 = getT();
        if (code == '+') {
            Tree<Node>* ret_val = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, ADDITION));
            ret_val->connectSubtree(LEFT_CHILD, val);
            ret_val->connectSubtree(RIGHT_CHILD, val1);
            val = ret_val;
        } else if (code == '-') {
            Tree<Node>* ret_val = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, MINUS));
            ret_val->connectSubtree(LEFT_CHILD, val);
            ret_val->connectSubtree(RIGHT_CHILD, val1);
            val = ret_val;
        }
    }
    return val;
}

Tree<Node> *LexicalAnalizator::getT() {
    Tree<Node>* val = getPow();
    while  (this_is(SYMBOL, '*') || this_is(SYMBOL, '/')) {
        int code = tok_str[tok_ptr].code;
        tok_ptr++;
        Tree<Node>* val1 = getPow();
        if (code == '*') {
            Tree<Node>* ret_val = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, MULTIPLY));
            ret_val->connectSubtree(LEFT_CHILD, val);
            ret_val->connectSubtree(RIGHT_CHILD, val1);
            val = ret_val;
        } else if (code == '/') {
            Tree<Node>* ret_val = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, DIVIDE));
            ret_val->connectSubtree(LEFT_CHILD, val);
            ret_val->connectSubtree(RIGHT_CHILD, val1);
            val = ret_val;
        }
    }
    return val;
}

Tree<Node> *LexicalAnalizator::getPow() {
    Tree<Node>* val = getP();
    if (this_is(SYMBOL, '^')) {
        tok_ptr++;
        Tree<Node>* val1 = getP();
        Tree<Node>* ret_val = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, DIVIDE));
        ret_val->connectSubtree(LEFT_CHILD, val);
        ret_val->connectSubtree(RIGHT_CHILD, val1);
        val = ret_val;
    }
    return val;
}

Tree<Node> *LexicalAnalizator::getP() {
    if (this_is(SYMBOL, '(')) {
        tok_ptr++;
        Tree<Node>* val = getEquation();
        check_assert(this_is(SYMBOL, ')'))
        tok_ptr++;
        return val;
    } else if (tok_str[tok_ptr].type == Token::NUMBER) {
        return new Tree<Node>(Node(tok_str[tok_ptr++].num));
    } else {
        return getName();
    }
}

Tree<Node> *LexicalAnalizator::getName() {
    check_assert(tok_str[tok_ptr].type == Token::ID)
    if (tok_str[tok_ptr + 1].type == Token::SYMBOL) {
        return getCall();
    } else {
        Tree<Node>* name = new Tree<Node> (Node(Node::VARIABLE_TYPE, tok_str[tok_ptr++].code));
        return name;
    }
}

Tree<Node> *LexicalAnalizator::getCall() {
    check_assert(tok_str[tok_ptr].type == Token::ID)
    Tree<Node>* function = new Tree<Node> (Node(Node::OPERATION_TYPE, tok_str[tok_ptr].code));
    function->connectSubtree(LEFT_CHILD, getArgEnum());
    return function;
}

Tree<Node> *LexicalAnalizator::getArgEnum() {
    check_assert(this_is(SYMBOL, '('))
    tok_ptr++;
    Tree<Node>* arg = new Tree<Node>(Node(Node::SPECIAL_SYMBOLS, COMA_PARAMETER));
    arg->connectSubtree(RIGHT_CHILD, getEquation());
    Tree<Node>* nextArg = arg;
    while (this_is(SYMBOL, ',')) {
        tok_ptr++;
        nextArg = nextArg->growChild(LEFT_CHILD, Node(Node::SPECIAL_SYMBOLS, COMA_PARAMETER));
        nextArg->connectSubtree(RIGHT_CHILD, getEquation());
    }
    check_assert(this_is(SYMBOL, '('))
    tok_ptr++;
    return arg;
}


#endif //PROGRAMMING_LANGUAGE_PARSER_H

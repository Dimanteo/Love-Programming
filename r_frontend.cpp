#include "Commons.h"
#include "Maksim.h"
#include "CodeGenerator.h"

void makeLanguageFile(const char fileName[], Tree<Node>* tree);

void generateCode(FILE* file, Tree<Node>* node);

void functionGenerator(FILE* file, Tree<Node>* node);

void stdGenerator(FILE* file, Tree<Node>* node);

void makeDefinition(FILE* file, Tree<Node>* node);

void makeCall(FILE* file, Tree<Node>* node);

int main() {
    Tree<Node>* tree = makeAST("../Maksim.love");

    makeLanguageFile("../RoboMaksim.txt", tree);

    delete(tree);
}

void makeLanguageFile(const char *fileName, Tree<Node> *tree) {
    FILE* file = fopen(fileName, "wb");

    generateCode(file, tree);

    fclose(file);
}

void generateCode(FILE* file, Tree<Node>* node) {

    if (node == nullptr)
        return;

    switch (node->getValue().type) {

        case Node::VARIABLE_TYPE:
            fprintf(file, "%s", VARIABLES[node->getValue().code].ID);
            break;

        case Node::OPERATION_TYPE:
            functionGenerator(file, node);
            break;

        case Node::SPECIAL_SYMBOLS:
            stdGenerator(file, node);
            break;

        case Node::NUMBER_TYPE:
            fprintf(file, "%g", node->getValue().num);
            break;
    }
}

void functionGenerator(FILE *file, Tree<Node> *node) {
    if (node->getParent()->getValue().type == Node::SPECIAL_SYMBOLS && node->getParent()->getValue().code == DOT_COMA) {
        makeDefinition(file, node);
    } else {
        makeCall(file, node);
    }
}

void makeDefinition(FILE *file, Tree<Node> *node) {

    if (strcmp(FUNCTIONS[node->getValue().code].ID, "$main") == 0) {

        fprintf(file, "%s\n%s\n", LMAIN, LBEGIN);

    } else {

        fprintf(file, "%s %s (", LFUNC_DEF, FUNCTIONS[node->getValue().code].ID + 1);

        Tree<Node> *parameter = node;

        while (!parameter->childIsEmpty(LEFT_CHILD)) {

            parameter = parameter->getChild(LEFT_CHILD);

            fprintf(file, "%s", VARIABLES[parameter->getChild(RIGHT_CHILD)->getValue().code].ID);

            if (!parameter->childIsEmpty(LEFT_CHILD))
                fprintf(file, ", ");
        }

        fprintf(file, ")\n%s\n", LBEGIN);
    }

    generateCode(file, node->getChild(RIGHT_CHILD));

    fprintf(file, "%s\n", LEND);
}

void makeCall(FILE *file, Tree<Node> *node) {
    fprintf(file, "%s (", FUNCTIONS[node->getValue().code].ID + 1);

    Tree<Node>* parameter = node;

    while (!parameter->childIsEmpty(LEFT_CHILD)) {

        parameter = parameter->getChild(LEFT_CHILD);

        generateCode(file, parameter->getChild(RIGHT_CHILD));

        if (!parameter->childIsEmpty(LEFT_CHILD))
            fprintf(file, ", ");
    }
    fprintf(file, ")");
}

void stdGenerator(FILE *file, Tree<Node> *node) {
    switch (node->getValue().code) {

        case DOT_COMA:
            generateCode(file, node->getChild(LEFT_CHILD));
            fprintf(file, "\n");
            generateCode(file, node->getChild(RIGHT_CHILD));
            break;

        case ASSIGN:
            generateCode(file, node->getChild(LEFT_CHILD));
            fprintf(file, " %s ", LASSIGNMENT);
            generateCode(file, node->getChild(RIGHT_CHILD));
            break;

        case OP:
            generateCode(file, node->getChild(LEFT_CHILD));
            fprintf(file, "\n");
            generateCode(file, node->getChild(RIGHT_CHILD));
            break;

        case RET:
            fprintf(file, "%s ", LRETURN);
            generateCode(file, node->getChild(LEFT_CHILD));
            break;

        case IF:
            fprintf(file, "%s (", LIF);
            generateCode(file, node->getChild(LEFT_CHILD));
            fprintf(file, ")\n%s\n", LBEGIN);
            generateCode(file, node->getChild(RIGHT_CHILD));
            fprintf(file, "%s\n", LEND);
            break;

        case IF_ELSE:
            generateCode(file, node->getChild(LEFT_CHILD));
            if (!node->childIsEmpty(RIGHT_CHILD)) {
                fprintf(file, "%s\n%s\n%s\n", LEND, LELSE, LBEGIN);
                generateCode(file, node->getChild(RIGHT_CHILD));
            }
            break;

        case WHILE:
            fprintf(file, "%s (", LWHILE);
            generateCode(file, node->getChild(LEFT_CHILD));
            fprintf(file, ")\n%s\n", LBEGIN);
            generateCode(file, node->getChild(RIGHT_CHILD));
            fprintf(file, "%s\n", LEND);
            break;

        case GET:
            fprintf(file, "%s ", LGET);
            generateCode(file, node->getChild(LEFT_CHILD));
            break;

        case PUT:
            fprintf(file, "%s ", LPUT);
            generateCode(file, node->getChild(LEFT_CHILD));
            break;

        case DERIV:
            fprintf(file, "%s ", LDIFFER);
            generateCode(file, node->getChild(LEFT_CHILD));
            fprintf(file, " %s ", LDIFFERVAL);
            generateCode(file, node->getChild(RIGHT_CHILD));
            break;

#define inorder(CODE, str) \
        case CODE:\
            generateCode(file, node->getChild(LEFT_CHILD));\
            fprintf(file,str);\
            generateCode(file, node->getChild(RIGHT_CHILD));\
            break;

        inorder(MORE,      " > ")
        inorder(LESS,      " < ")
        inorder(MORE_EQUAL," >= ")
        inorder(LESS_EQUAL," <= ")
        inorder(EQUAL,     " == ")
        inorder(NOT_EQUAL, " != ")
        inorder(ADDITION,  " + ")
        inorder(MINUS,     " - ")
        inorder(DIVIDE,    " / ")
        inorder(MULTIPLY,  " * ")
        inorder(POWER,     "^")

#undef inorder
    }
}

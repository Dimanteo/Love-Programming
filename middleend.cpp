#include "CodeGenerator.h"

void printASTnode(FILE* file, Tree<Node>* node);

void makeASTfile(const char fileName[], Tree<Node> *tree);

int main() {
    Tree<Node>* tree = makeAST("Maksim.love");

    Differentiator optimizer(0);

    makeDump("Tree.log", "AST_middle_IN.png", tree);

    optimizer.optimization(tree);

    makeDump("Tree.log", "AST_middle_OUT.png", tree);

    makeASTfile("Maksim.love", tree);

    delete(tree);
}

void printASTnode(FILE* file, Tree<Node>* node) {
    switch (node->getValue().type) {

        case Node::NUMBER_TYPE:
            fprintf(file, "{%g}", node->getValue().num);
            return;

        case Node::SPECIAL_SYMBOLS:
            fprintf(file, "{%s", STD_ObjList[node->getValue().code].ID);
            break;

        case Node::VARIABLE_TYPE:
            fprintf(file, "{%s}", VARIABLES[node->getValue().code].ID);
            return;
        case Node::OPERATION_TYPE:
            fprintf(file, "{%s", FUNCTIONS[node->getValue().code].ID);
            break;
    }

    if (!node->childIsEmpty(LEFT_CHILD))
        printASTnode(file, node->getChild(LEFT_CHILD));
    else
        fprintf(file, "@");

    if (!node->childIsEmpty(RIGHT_CHILD))
        printASTnode(file, node->getChild(RIGHT_CHILD));
    else
        fprintf(file, "@");

    fprintf(file, "}");
}

void makeASTfile(const char fileName[], Tree<Node> *tree) {
    FILE* file = fopen(fileName, "wb");

    printASTnode(file, tree);

    fclose(file);

    for (int i = 0; i < FUNCTION_COUNT; ++i) {
        free(FUNCTIONS[i].ID);
    }
    for (int i = 0; i < VARIABLE_COUNT; ++i) {
        free(VARIABLES[i].ID);
    }
}

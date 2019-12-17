#include "Commons.h"
#include "Node.h"
#include "My_Headers/txt_files.h"
#include "Tree_t/Tree.cpp"
#include "Parser.h"

/////////Специализация шаблона/////////////
template <>
void Tree<Node>::valuePrint(FILE *file) {
    switch (value.type) {

        case Node::NUMBER_TYPE:
            fprintf(file, "%lf", value.num);
            break;

        case Node::VARIABLE_TYPE:
            fprintf(file, "%s", ID_VECTOR[value.code]);
            break;

        case Node::OPERATION_TYPE:
            fprintf(file, "$%s", ID_VECTOR[value.code]);
            break;

        case Node::SPECIAL_SYMBOLS:
            fprintf(file, "%s", STD_ObjList[value.code].ID);
    }
}

template<>
void Tree<Node>::valueDestruct() {
    value.~Node();
}

/////////Методы frontend/////////////

Tree<Node>* parseLangToAST(const char *inputFile);

void makeASTfile(const char fileName[], Tree<Node> *tree);

void printASTnode(FILE* file, Tree<Node>* node);

void makeTransDump(const char textDump[], const char pngDump[], Tree<Node>* AST);

int main() {
    Tree<Node>* AST = parseLangToAST("../Maksim.txt");

    makeASTfile("../Maksim.love", AST);

    return 0;
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
            fprintf(file, "{%s}", ID_VECTOR[node->getValue().code]);
            return;
        case Node::OPERATION_TYPE:
            fprintf(file, "{$%s", ID_VECTOR[node->getValue().code]);
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

    for (int i = 0; i < ID_COUNT; ++i) {
        free(ID_VECTOR[i]);
    }
}

Tree<Node> *parseLangToAST(const char *inputFile) {
    size_t size = 0;
    char* buffer = read_file_to_buffer_alloc(inputFile, "r", &size);

    LexicalAnalizator analizator = LexicalAnalizator(buffer);

    analizator.tokenize();

    analizator.dump();

    free(buffer);

    Tree<Node>* AST = analizator.getG();

    makeTransDump("Tree.log", "FrontAST.png", AST);

    return AST;
}

void makeTransDump(const char *textDump, const char *pngDump, Tree<Node> *AST) {
    Tree<Node>** seq = AST->allocTree();

    AST->postorder(seq);

    FILE* dump = fopen(textDump, "wb");

    AST->treeDump(dump, OK_STATE, "frontend", __FILE__, __PRETTY_FUNCTION__, __LINE__, seq);
    AST->graphDump(pngDump, seq);

    fclose(dump);
    free(seq);
}

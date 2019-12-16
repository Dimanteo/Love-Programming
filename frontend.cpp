#include <iostream>
#include "Commons.h"
#include "Node.h"
#include "My_Headers/txt_files.h"
#include "Tree_t/Tree.cpp"
#include "Parser.h"

Tree<Node>* parseLangToAST(char* buffer);

void makeASTFile(Tree<Node>* tree);

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

int main() {
    size_t size_buffer = 0;
    char* buffer = read_file_to_buffer_alloc("../Maksim.txt", "r", &size_buffer);

    LexicalAnalizator analizator = LexicalAnalizator(buffer);

    analizator.tokenize();

    analizator.dump();
    free(buffer);

    Tree<Node>* AST = analizator.getG();

    Tree<Node>** seq = AST->allocTree();
    AST->postorder(seq);
    FILE* dump = fopen("Tree.log", "wb");
    AST->treeDump(dump, OK_STATE, "frontend", __FILE__, __PRETTY_FUNCTION__, __LINE__, seq);
    fclose(dump);
    AST->graphDump("FrontAST.png", seq);
    free(seq);

    return 0;
}


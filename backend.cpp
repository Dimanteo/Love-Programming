#include "CodeGenerator.h"

int main() {

    Tree<Node>* tree = makeAST("Maksim.love");

    makeDump("Tree.log", "AST_backend_IN.png", tree);

    makeAssembler("asm.txt", tree);

    makeDump("Tree.log", "AST_backend_OUT.png", tree);

    for (int i = 0; i < FUNCTION_COUNT; ++i) {
        free(FUNCTIONS[i].ID);
    }
    for (int i = 0; i < VARIABLE_COUNT; ++i) {
        free(VARIABLES[i].ID);
    }
    delete (tree);
}

#include "lexer.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    ASTNodeArray context;

    tokenize("3 + 4 * 5", &context);

    AST tree = parse(&context);

    printf("Hola\n");

    return EXIT_SUCCESS;
}

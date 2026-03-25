#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char buf[256];
    printf("Insert a valid mathematical expression: ");
    
    int c;
    int pos = 0;
    while ((c = getc(stdin)) != '\n' && c != EOF) {
        buf[pos] = c;
        pos++;
    }
    buf[pos] = '\0';

    ASTNodeArray context;
    tokenize(buf, &context);

    AST tree = parse(&context);
    int64_t result = evaluate(tree.head);


    printf("El resultado es: %" PRIi64 "\n", result);
    return EXIT_SUCCESS;
}

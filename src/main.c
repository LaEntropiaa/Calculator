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

    EvaluatorResult result = evaluate(parse(tokenize(buf)));
    if (!result.is_valid) {
        puts("Error checando expresion");
    }

    printf("El resultado es: %" PRIi64 "\n", result.val);
    return EXIT_SUCCESS;
}

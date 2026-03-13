#include "lexer.h"

typedef struct {
    ASTNode *head;
} AST;

typedef enum {
    PARSER_NUD, // Null Denotation
    PARSER_LED, // Left Denotation
} ParserState;

size_t node_lbp(Operator op);
size_t node_rbp(Operator op);
AST parse(ASTNodeArray arr);

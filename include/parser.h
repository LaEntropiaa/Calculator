#include "lexer.h"

typedef struct {
    ASTNode *head;
} AST;

typedef struct {
    ASTNodeArray *arr;
    size_t pos;
} ASTNodeSlice;

ASTNode *nud(ASTNodeSlice *slice, size_t pos);
ASTNode *led(ASTNodeSlice *slice, size_t pos, size_t right_precedence);

size_t node_lbp(ASTNode node);
size_t node_rbp(ASTNode node);
AST parse(ASTNodeArray arr);

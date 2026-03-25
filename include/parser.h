#include "lexer.h"

typedef struct {
    ASTNode *head;
} AST;

typedef struct {
    ASTNodeArray *arr;
    size_t pos;
} ASTNodeSlice;

ASTNode ASTNodeSlice_peek(ASTNodeSlice slice);
ASTNode ASTNodeSlice_next(ASTNodeSlice slice);

ASTNode *nud(ASTNodeSlice *slice);
ASTNode *led(ASTNodeSlice *slice, size_t right_precedence);

size_t node_lbp(ASTNode node);
size_t node_rbp(ASTNode node);
AST parse(ASTNodeArray arr);
AST parse_expr(ASTNodeSlice *arr);

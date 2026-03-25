#include "lexer.h"
#include <stdint.h>

typedef struct {
    ASTNode *head;
} AST;

typedef struct {
    ASTNodeArray *arr;
    size_t pos;
} ASTNodeSlice;

ASTNode ASTNodeSlice_peek(ASTNodeSlice *slice);
ASTNode ASTNodeSlice_next(ASTNodeSlice *slice);
bool ASTNodeSlice_is_valid(ASTNodeSlice *slice);

ASTNode *nud(ASTNodeSlice *slice);
ASTNode *led(ASTNodeSlice *slice, size_t right_precedence);

uint8_t node_lbp(ASTNode node);
uint8_t node_rbp(ASTNode node);
AST parse(ASTNodeArray *arr);
ASTNode *parse_expr(ASTNodeSlice *slice, uint8_t min_bp);

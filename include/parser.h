#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "arena.h"
#include <stdint.h>

typedef struct {
    ASTNode *head;
} AST;

typedef struct {
    ASTNodeArray *arr;
    size_t pos;
} ASTNodeSlice;

typedef struct {
    Arena arena;
    ASTNode *tree;
} ParseResult;

ASTNode ASTNodeSlice_peek(ASTNodeSlice *slice);
ASTNode ASTNodeSlice_next(ASTNodeSlice *slice);
bool ASTNodeSlice_is_valid(ASTNodeSlice *slice);

ASTNode *nud(ASTNodeSlice *slice);
ASTNode *led(ASTNodeSlice *slice, size_t right_precedence);

uint8_t node_lbp(ASTNode node);
uint8_t node_rbp(ASTNode node);

ParseResult parse(ASTNodeArray *arr);
ASTNode *parse_expr(ASTNodeSlice *slice, Arena *arena, uint8_t min_bp);

#endif // !PARSER_H

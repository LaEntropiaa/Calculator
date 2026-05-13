#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "arena.h"
#include "arraylist.h"
#include <stdint.h>

typedef struct {
    ASTNode *head;
} AST;

typedef enum {
    PARSER_OK = 0,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_MISSING_OPERAND,
    PARSER_UNMATCHED_PAREN,
    PARSER_OUT_OF_MEMORY,
} ParserErr;

typedef struct {
    bool is_valid;
    union {
        ParserErr err;
        struct {
            Arena arena;
            ASTNode *tree;
        };
    };
} ParseResult;

ASTNode *nud(ArraySlice *slice);
ASTNode *led(ArraySlice *slice, size_t right_precedence);

uint8_t prefix_lbp(ASTNode node);
uint8_t prefix_rbp(ASTNode node);
uint8_t infix_lbp(ASTNode node);
uint8_t infix_rbp(ASTNode node);

ParseResult parse(TokenizeResult tokens);
ASTNode *parse_expr(ArraySlice *slice, Arena *arena, uint8_t min_bp);

#endif // !PARSER_H

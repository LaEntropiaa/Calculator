#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "arena.h"
#include "arraylist.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    NODE_INT,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_PARENTHESIS,
} NodeType;

typedef struct Node {
    NodeType type;
    union {
        int64_t num;
        struct {
            Operator op;
            struct Node *left;
            struct Node *right;
        }binary;
        struct {
            Operator op;
            struct Node *to;
        }unary;
        Operator par;
    };
} Node;

typedef enum {
    PARSER_OK = 0,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_MISSING_OPERAND,
    PARSER_UNMATCHED_PAREN,
    PARSER_OUT_OF_MEMORY,
    PARSER_INVALID_TOKENIZE,
    PARSER_UNEXPECTED_EOF,
} ParserErr;

typedef struct {
    bool is_valid;
    union {
        ParserErr err;
        struct {
            Arena arena;
            Node *tree;
        };
    };
} ParserResult;

typedef struct  {
    bool is_valid;
    union {
        ParserErr err;
        Node *node;
    };
} TreeResult;

typedef struct {
    bool is_valid;
    union {
        ParserErr err;
        Node node;
    };
} NodeResult;

typedef struct {
    bool is_valid;
    union {
        ParserErr err;
        uint8_t num;
    };
} ParserU8Result;

TreeResult nud(ArraySlice *slice, Arena *arena, Token token); // Null denotation
TreeResult led(ArraySlice *slice, Arena *arena, Node *left, Token token); // Left denotation

ParserU8Result prefix_rbp(Token token);
ParserU8Result postfix_lbp(Token token);
ParserU8Result infix_lbp(Token token);
ParserU8Result infix_rbp(Token token);

ParserResult parse(TokenizeResult tokens);
TreeResult parse_expr(ArraySlice *slice, Arena *arena, uint8_t min_bp);

#endif // !PARSER_H

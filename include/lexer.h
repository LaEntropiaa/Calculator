#ifndef LEXER_H
#define LEXER_H

#include "arraylist.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// For identifing
typedef enum {
    NODE_INTEGER,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_PARENTHESIS,
} ASTNodeType;

// For classify operators
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_FACTORIAL,
    OP_START_PAR,
    OP_END_PAR,
} Operator;

typedef enum {
    LEXER_OK = 0,
    LEXER_INT_OVERFLOW,
    LEXER_FAILED_NUMBER_CONVERSION,
    LEXER_NOT_RECOGNIZED_SYMBOL,
    LEXER_EMPTY_INPUT,
    LEXER_BUF_OVERFLOW,
} LexerErr;

// Can be thought as tokens, they will be used by the parser.
typedef struct ASTNode {
    ASTNodeType type;
    union {
        int64_t integer;
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            Operator op;
        } binary;
        struct {
            struct ASTNode *val;
            Operator op;
        } unary;
        struct {
            struct ASTNode *val;
            Operator op;
        } parenthesis;
    } data;
} ASTNode;

typedef struct {
    bool is_valid;
    union {
        LexerErr err;
        ArrayList *arr;
    };
} TokenizeResult;

typedef struct {
    bool is_valid;
    union {
        LexerErr err;
        ASTNode node;
    };
} ASTNodeResult;

typedef struct {
    bool is_valid;
    union {
        LexerErr err;
        int64_t number;
    };
} LexerI64Result;

// Lexer funtions as well as few functionality
TokenizeResult tokenize(const char* input);
ASTNodeResult tokenize_number(const char* input, size_t *offset);
LexerI64Result string_to_integer(const char buf[]);
bool isoperator(int c);
Operator char_to_operator(int c);
char operator_to_char(Operator op);

#endif // !LEXER_H

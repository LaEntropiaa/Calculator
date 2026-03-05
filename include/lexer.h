#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>

// For identifing
typedef enum {
    NODE_NUMBER,
    NODE_OPERATOR,
} ASTNodeType;

// For classify operators
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
} Operator;

typedef enum {
    ARRAY_OK = 0,
    ARRAY_EMPTY,
    ARRAY_OUT_OF_BOUNDS,
    ARRAY_NULL_ARG,
    ARRAY_ALLOC,
} ASTNodeArrayErr;

typedef enum {
    LEXER_OK = 0,
    LEXER_FAILED_NUMBER_CONVERSION,
    LEXER_NOT_RECOGNIZED_SYMBOL,
    LEXER_EMPTY_INPUT,
    LEXER_NULL_ARG,
    LEXER_WRONG_SYNTAX,
} LexerErr;

// Can be thought as tokens, they will be used by the parser.
typedef struct ASTNode {
    ASTNodeType type;
    union {
        double number;
        struct ASTNode *left;
        struct ASTNode *right;
        Operator op;
    } operator;
} ASTNode;

// I prefer ot have a dynamic array for storing the "tokens"
typedef struct {
    size_t len;
    size_t cap;
    ASTNode *data;
} ASTNodeArray;

// Basic array functionality
ASTNodeArray ASTNodeArray_init(size_t size);
void ASTNodeArray_free(ASTNodeArray *arr);
ASTNodeArrayErr ASTNodeArray_push(ASTNodeArray arr, ASTNode node);
ASTNodeArrayErr ASTNodeArray_get(const ASTNodeArray arr, size_t index, ASTNode *out);
// Out in pop can be NULL so it doesn't return anything
ASTNodeArrayErr ASTNodeArray_pop(ASTNodeArray arr, size_t index, ASTNode *out);

// Lexer funtions as well as few functionality
LexerErr tokenize(const char* input, ASTNodeArray *out);
LexerErr tokenize_number(const char* input, size_t *offset, ASTNode *out);
LexerErr string_to_number(const char* str, double *number);

#endif // !LEXER_H

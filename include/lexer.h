#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// For identifing
typedef enum {
    NODE_INTEGER,
    NODE_BINARY_OP,
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
    ARRAY_NULL,
    ARRAY_EMPTY,
    ARRAY_OUT_OF_BOUNDS,
    ARRAY_NULL_ARG,
    ARRAY_ALLOC,
} ASTNodeArrayErr;

typedef enum {
    LEXER_OK = 0,
    LEXER_INT_OVERFLOW,
    LEXER_FAILED_NUMBER_CONVERSION,
    LEXER_NOT_RECOGNIZED_SYMBOL,
    LEXER_EMPTY_INPUT,
    LEXER_NULL_ARG,
    LEXER_WRONG_SYNTAX,
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
    } data;
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
ASTNodeArrayErr ASTNodeArray_push(ASTNodeArray *arr, ASTNode node);
ASTNodeArrayErr ASTNodeArray_get(const ASTNodeArray *arr, size_t index, ASTNode *out);
// Out in pop can be NULL so it doesn't return anything
ASTNodeArrayErr ASTNodeArray_pop(ASTNodeArray *arr, size_t index, ASTNode *out);
size_t ASTNodeArray_len(ASTNodeArray *arr);

// Lexer funtions as well as few functionality
LexerErr tokenize(const char* input, ASTNodeArray *out);
LexerErr tokenize_number(const char* input, size_t *offset, ASTNode *out);
LexerErr string_to_integer(const char buf[], int64_t *number);
bool isoperator(int c);
Operator char_to_operator(int c);
char operator_to_char(Operator op);

#endif // !LEXER_H

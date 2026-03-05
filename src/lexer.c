#include "lexer.h"
#include <stdlib.h>
#include <strings.h>

#define NODE_ARRAY_DEFAULT_SIZE 64
// Helps  state machine for the lexer :)
typedef enum {
    WAIT_FOR_NUMBER,
    WAIT_FOR_OPERATOR,
} LexerState;

ASTNodeArray ASTNodeArray_init(size_t size) {
    ASTNodeArray new;
    new.len = 0;      // if 0 then use default
    new.cap = size == 0 ? NODE_ARRAY_DEFAULT_SIZE : size;
    new.data = malloc(new.cap * sizeof(ASTNode));
    return new;
}

void ASTNodeArray_free(ASTNodeArray *arr) {
    free(arr->data);
    arr->cap = 0;
    arr->len = 0;
}

ASTNodeArrayErr ASTNodeArray_get(const ASTNodeArray arr, size_t index, ASTNode *out) {
    if (out == NULL) {
        return ARRAY_NULL_ARG;
    }

    if (arr.len == 0) {
        return ARRAY_EMPTY;
    }

    if (index >= arr.len) {
        return ARRAY_OUT_OF_BOUNDS;
    }

    *out = arr.data[index];

    return ARRAY_OK;
}

ASTNodeArrayErr ASTNodeArray_push(ASTNodeArray arr, ASTNode node) {
    if (arr.len >= arr.cap) {
        size_t new_cap = arr.cap * 2;
        ASTNode *tmp = realloc(arr.data, new_cap * sizeof(ASTNode));
        if (tmp == NULL) {
            return ARRAY_ALLOC;
        }
        arr.data = tmp;
        arr.cap = new_cap;
    }

    arr.data[arr.len] = node;
    arr.len += 1;

    return ARRAY_OK;
}

ASTNodeArrayErr ASTNodeArray_pop(ASTNodeArray arr, size_t index, ASTNode *out) {
    if (arr.len == 0) {
        return ARRAY_EMPTY;
    }

    if (index >= arr.len) {
        return ARRAY_OUT_OF_BOUNDS;
    }
    
    if (out != NULL) {
        ASTNode node_to_delete = arr.data[index];
        *out = node_to_delete;
    }

    for (size_t i = index; i < arr.len - 1; i++) {
        arr.data[index] = arr.data[index + 1];
    }

    return ARRAY_OK;
}

#include "lexer.h"
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <limits.h>

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

ASTNodeArrayErr ASTNodeArray_get(const ASTNodeArray *arr, size_t index, ASTNode *out) {
    if (arr == NULL) {
        return ARRAY_NULL;
    }

    if (out == NULL) {
        return ARRAY_NULL_ARG;
    }

    if (arr->len == 0) {
        return ARRAY_EMPTY;
    }

    if (index >= arr->len) {
        return ARRAY_OUT_OF_BOUNDS;
    }

    *out = arr->data[index];

    return ARRAY_OK;
}

ASTNodeArrayErr ASTNodeArray_push(ASTNodeArray *arr, ASTNode node) {
    if (arr == NULL) {
        return ARRAY_NULL;
    }

    if (arr->len >= arr->cap) {
        size_t new_cap = arr->cap * 2;
        ASTNode *tmp = realloc(arr->data, new_cap * sizeof(ASTNode));
        if (tmp == NULL) {
            return ARRAY_ALLOC;
        }
        arr->data = tmp;
        arr->cap = new_cap;
    }

    arr->data[arr->len] = node;
    arr->len = arr->len + 1;

    return ARRAY_OK;
}

ASTNodeArrayErr ASTNodeArray_pop(ASTNodeArray *arr, size_t index, ASTNode *out) {
    if (arr == NULL) {
        return ARRAY_NULL;
    }

    if (arr->len == 0) {
        return ARRAY_EMPTY;
    }

    if (index >= arr->len) {
        return ARRAY_OUT_OF_BOUNDS;
    }

    if (arr->cap / 4 > arr->len) {
        size_t new_cap = arr->cap / 2;
        ASTNode *tmp = realloc(arr->data, new_cap * sizeof(ASTNode));
        if (tmp == NULL) {
            return ARRAY_ALLOC;
        }
        arr->data = tmp;
        arr->cap = new_cap;
    }
    
    if (out != NULL) {
        ASTNode node_to_delete = arr->data[index];
        *out = node_to_delete;
    }

    for (size_t i = index; i < arr->len - 1; i++) {
        arr->data[index] = arr->data[index + 1];
    }

    return ARRAY_OK;
}

size_t ASTNodeArray_len(ASTNodeArray *arr) {
    if (arr == NULL) {
        return 0;
    }
    return arr->len;
}

// CURRENTLY, it only supports ints, not clear how floating
// point is implemented but i'll figure it out
LexerErr string_to_number(const char *input, size_t *offset, double *number) {
    char buf[128] = { '\0' };
    size_t buf_pos = 0;

    size_t current = *offset;
    while (isdigit(input[current])) {
        buf[buf_pos] = input[current];
        
        if (buf_pos >= sizeof(buf)) {
            return LEXER_BUF_OVERFLOW;
        }
        current++;
        buf_pos++;
    }
    
    int c = 0;
    long count = 0;
    while (buf[c] != '\0') {
        
        int digit = buf[c] - '0';

        if (count > (INT_MAX - digit) / 10) {
            return LEXER_FAILED_NUMBER_CONVERSION;
        }
        count = count * 10;
        count += digit;
        
        c++;
    }

    *number = (double) count;
    *offset = current;
    return LEXER_OK;
}

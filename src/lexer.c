#include "lexer.h"
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <limits.h>

typedef enum {
    WAIT_FOR_NUMBER,
    WAIT_FOR_OPERATOR
} LexerState;


LexerErr tokenize(const char *input, ASTNodeArray *out) {
    size_t offset = 0;
    LexerState state = WAIT_FOR_NUMBER;
    ASTNodeArray arr = ASTNodeArray_init(0); // 0 defaults to 64

    while (input[offset] != '\n' && input[offset] != '\0') {
        int current = input[offset];

        if (isdigit(current)) {
            if (state != WAIT_FOR_NUMBER) {
                ASTNodeArray_free(&arr);
                return LEXER_WRONG_SYNTAX;
            }
            ASTNode new_node;
            LexerErr result = tokenize_number(input, &offset, &new_node);

            if (result != LEXER_OK) {
                ASTNodeArray_free(&arr);
                return result;
            }

            ASTNodeArray_push(&arr, new_node);
            state = WAIT_FOR_OPERATOR;
        } else if (isoperator(current)) {
            if (state != WAIT_FOR_OPERATOR) {
                return LEXER_WRONG_SYNTAX;
            }
            ASTNode new_node = {
                .type = NODE_BINARY_OP,
                .data.binary.op = char_to_operator(current),
                .data.binary.right = NULL,
                .data.binary.left = NULL,
            };

            ASTNodeArray_push(&arr, new_node);
            state = WAIT_FOR_NUMBER;
        } else if (isspace(current)) {
            // Nothing...
        } else {
            ASTNodeArray_free(&arr);
            return LEXER_NOT_RECOGNIZED_SYMBOL;
        }

        offset++;
    }

    if (arr.len < 1) {
        return LEXER_EMPTY_INPUT;
    }

    *out = arr;
    return LEXER_OK;
}

// CURRENTLY, it only supports ints, not clear how floating
// point is implemented but i'll figure it out
LexerErr tokenize_number(const char *input, size_t *offset, ASTNode *out) {
    char buf[128] = { '\0' };
    size_t buf_pos = 0;
    bool is_integer = true; // Will later be used to differentiate fractions

    size_t current = *offset;
    while (isdigit(input[current])) {
        buf[buf_pos] = input[current];
        
        if (buf_pos >= sizeof(buf)) {
            return LEXER_BUF_OVERFLOW;
        }
        current++;
        buf_pos++;
    }

    ASTNode new_node;
    if (is_integer) {
        new_node.type = NODE_INTEGER;
        LexerErr status = string_to_integer(buf, &new_node.data.integer);
        if (status == LEXER_OK) {
            *out = new_node;
        }
        *offset = current;
        return status;
    }

    return LEXER_FAILED_NUMBER_CONVERSION;
}

LexerErr string_to_integer(const char *buf, int64_t *number) {
    int c = 0;
    int64_t count = 0;
    while (buf[c] != '\0') {
        
        int digit = buf[c] - '0';

        if (count > (INT64_MAX - digit) / 10) {
            return LEXER_INT_OVERFLOW;
        }
        count = count * 10;
        count += digit;
        
        c++;
    }

    *number = count;
    return LEXER_OK;
}

bool isoperator(int c) {
    switch (c) {
        case '+':
        case '-':
        case '/':
        case '*':
            return true;
        default:
            return false;
    }
}

Operator char_to_operator(int c) {
    switch (c) {
        case '+':
            return OP_ADD;
            break;
        case '-':
            return OP_SUB;
            break;
        case '*':
            return OP_MUL;
            break;
        case '/':
            return OP_DIV;
            break;
        default: // I mean shouldn't be used, we assume
            return -1;
    }
}

char operator_to_char(Operator op) {
    switch (op) {
        case OP_ADD:
            return '+';
        case OP_SUB:
            return '-';
        case OP_MUL:
            return '*';
        case OP_DIV:
            return '/';
    }
}

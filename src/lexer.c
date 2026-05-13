#include "lexer.h"
#include "arraylist.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <limits.h>

typedef enum {
    WAIT_FOR_NUMBER,
    WAIT_FOR_OPERATOR
} LexerState;


TokenizeResult tokenize(const char *input) {
    ArrayList *arr = arraylist_init(64, sizeof(ASTNode));
    size_t offset = 0;

    while (
        input[offset] != '\n' ||
        input[offset] != EOF ||
        input[offset] != '\0') {

        if (isdigit(input[offset])) {
            ASTNodeResult result = tokenize_number(input, &offset);

            if (!result.is_valid) {
                return (TokenizeResult) {.is_valid = false, .err = result.err};
            }

            arraylist_push_back(arr, &result.node);
        } else if (isoperator(input[offset])) {
            ASTNode op_node = {
                .type = NODE_BINARY_OP,
                .data.binary.op = char_to_operator(input[offset]),
                .data.binary.left = NULL,
                .data.binary.right = NULL,
            };
            
            arraylist_push_back(arr, &op_node);
        } else if (isspace(input[offset])) {
            // Nothing...
        } else {
            return (TokenizeResult) {
                .is_valid = false,
                .err = LEXER_NOT_RECOGNIZED_SYMBOL};
        }

        offset++;
    }

    if (arraylist_size(arr) < 1) {
        return (TokenizeResult) {.is_valid = false, .err = LEXER_EMPTY_INPUT};
    }

    return (TokenizeResult) {.is_valid = true, .arr = arr};
}

// CURRENTLY, it only supports ints, not clear how floating
// point is implemented but i'll figure it out
ASTNodeResult tokenize_number(const char *input, size_t *offset) {
    char buf[64] = { '\0' };
    size_t buf_pos = 0;
    bool is_integer = true; // Will later be used to differentiate fractions

    // read number
    size_t current = *offset;
    while (isdigit(input[current])) {
        buf[buf_pos] = input[current];
        
        if (buf_pos >= sizeof(buf)) {
            return (ASTNodeResult) {
                .is_valid = false,
                .err = LEXER_BUF_OVERFLOW};
        }

        current++;
        buf_pos++;
    }

    ASTNode new_node;
    if (is_integer) {
        new_node.type = NODE_INTEGER;
        LexerI64Result status = string_to_integer(buf);


        if (!status.is_valid) {
            return (ASTNodeResult) {.is_valid = false, .err = status.err};
        }

        new_node.data.integer = status.number;

        *offset = current;
        return (ASTNodeResult) {.is_valid = true, .node = new_node};
    }

    return (ASTNodeResult) {
        .is_valid = false,
        .err = LEXER_FAILED_NUMBER_CONVERSION};
}

LexerI64Result string_to_integer(const char *buf) {
    int c = 0;
    int64_t count = 0;

    while (buf[c] != '\0') {

        // Extracts number from char
        int digit = buf[c] - '0';

        if (count > (INT64_MAX - digit) / 10) {
            return (LexerI64Result) {
                .is_valid = false,
                .err = LEXER_INT_OVERFLOW};
        }

        count = count * 10;
        count += digit;
        
        c++;
    }

    return (LexerI64Result) {.is_valid = true, .number = count};
}

bool isoperator(int c) {
    switch (c) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '^':
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
        case '^':
            return OP_POW;
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
        case OP_POW:
            return '^';
    }
}

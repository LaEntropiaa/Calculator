#include "lexer.h"
#include "arraylist.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef enum {
    WAIT_FOR_NUMBER,
    WAIT_FOR_OPERATOR
} LexerState;


TokenizeResult tokenize(const char *input) {
    ArrayList *arr;
    arraylist_init(&arr, 64, sizeof(Token));
    size_t offset = 0;

    while (input[offset] != '\0') {

        if (isdigit(input[offset])) {
            TokenResult result = tokenize_number(input, &offset);

            if (!result.is_valid) {
                arraylist_destroy(&arr);
                return (TokenizeResult) {.is_valid = false, .err = result.err};
            }

            arraylist_push_back(arr, &result.token);
        } else if (isoperator(input[offset])) {
            Token op_node = {
                .type = TOKEN_OPERATOR,
                .op = char_to_operator(input[offset]),
            };
            
            arraylist_push_back(arr, &op_node);
        } else if (isspace(input[offset])) {
            // Nothing...
        } else {
            arraylist_destroy(&arr);
            return (TokenizeResult) {
                .is_valid = false,
                .err = LEXER_NOT_RECOGNIZED_SYMBOL};
        }

        offset++;
    }

    if (arraylist_size(arr) < 1) {
        arraylist_destroy(&arr);
        return (TokenizeResult) {.is_valid = false, .err = LEXER_EMPTY_INPUT};
    }

    return (TokenizeResult) {.is_valid = true, .arr = arr};
}

// CURRENTLY, it only supports ints, not clear how floating
// point is implemented but i'll figure it out
TokenResult tokenize_number(const char *input, size_t *offset) {
    char buf[64] = { '\0' };
    size_t buf_pos = 0;
    bool is_integer = true; // Will later be used to differentiate fractions

    // read number
    size_t current = *offset;
    while (isdigit(input[current])) {
        if (buf_pos >= sizeof(buf) - 1) {
            return (TokenResult) {
                .is_valid = false,
                .err = LEXER_BUF_OVERFLOW};
        }

        buf[buf_pos] = input[current];

        current++;
        buf_pos++;
    }

    Token new_token;
    if (is_integer) {
        new_token.type = TOKEN_INTEGER;
        LexerI64Result result = string_to_integer(buf);


        if (!result.is_valid) {
            return (TokenResult) {.is_valid = false, .err = result.err};
        }

        new_token.num = result.num;

        *offset = current - 1;
        return (TokenResult) {.is_valid = true, .token = new_token};
    }

    return (TokenResult) {
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

    return (LexerI64Result) {.is_valid = true, .num = count};
}

bool isoperator(int c) {
    switch (c) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '^':
        case '!':
        case '(':
        case ')':
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
        case '!':
            return OP_FACTORIAL;
            break;
        case '(':
            return OP_START_PAR;
            break;
        case ')':
            return OP_END_PAR;
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
        case OP_FACTORIAL:
            return '!';
        case OP_START_PAR:
            return '(';
        case OP_END_PAR:
            return ')';
        default:
            return EOF;
    }
}

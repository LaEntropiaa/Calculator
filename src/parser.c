#include "parser.h"
#include "lexer.h"
#include <stdlib.h>

size_t node_lbp(Operator op) {
    switch (op) {
        case OP_ADD:
        case OP_SUB:
            return 10;
            break;
        case OP_DIV:
        case OP_MUL:
            return 20;
    }
}

size_t node_rbp(Operator op) {
    switch (op) {
        case OP_ADD:
        case OP_SUB:
            return 10;
            break;
        case OP_DIV:
        case OP_MUL:
            return 20;
    }
}

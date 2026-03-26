#include "evaluator.h"
#include "lexer.h"
#include <stdint.h>
#include <stdlib.h>

int64_t evaluate(ASTNode *tree) {
    if (tree->type == NODE_BINARY_OP) {
        Operator op = tree->data.binary.op;
        ASTNode *left = tree->data.binary.left;
        ASTNode *right = tree->data.binary.right;
        
        free(tree);

        switch (op) {
            case OP_ADD:
                return evaluate(left) + evaluate(right);
            case OP_SUB:
                return evaluate(left) - evaluate(right);
            case OP_MUL:
                return evaluate(left) * evaluate(right);
            case OP_DIV:
                return evaluate(left) / evaluate(right);

        }
    } else {
        int64_t return_val = tree->data.integer;
        free(tree);
        return return_val;
    }
}


#include "evaluator.h"
#include "lexer.h"
#include <stdint.h>

int64_t evaluate(ASTNode *tree) {
    if (tree->type == NODE_BINARY_OP) {
        switch (tree->data.binary.op) {
            case OP_ADD:
                return evaluate(tree->data.binary.left) + evaluate(tree->data.binary.right);
            case OP_SUB:
                return evaluate(tree->data.binary.left) - evaluate(tree->data.binary.right);
            case OP_MUL:
                return evaluate(tree->data.binary.left) * evaluate(tree->data.binary.right);
            case OP_DIV:
                return evaluate(tree->data.binary.left) / evaluate(tree->data.binary.right);

        }
    } else {
        return tree->data.integer;
    }
}


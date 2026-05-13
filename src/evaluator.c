#include "evaluator.h"
#include "arena.h"
#include "lexer.h"
#include "parser.h"
#include <stdint.h>
#include <math.h>


int64_t evaluate_tree(ASTNode *tree) {
    if (tree->type == NODE_BINARY_OP) {
        Operator op = tree->data.binary.op;
        ASTNode *left = tree->data.binary.left;
        ASTNode *right = tree->data.binary.right;
        
        switch (op) {
            case OP_ADD:
                return evaluate_tree(left) + evaluate_tree(right);
            case OP_SUB:
                return evaluate_tree(left) - evaluate_tree(right);
            case OP_MUL:
                return evaluate_tree(left) * evaluate_tree(right);
            case OP_DIV:
                return evaluate_tree(left) / evaluate_tree(right);
            case OP_POW:
                return pow(evaluate_tree(left), evaluate_tree(right));
        }
    }

    int64_t return_val = tree->data.integer;
    return return_val;
}

int64_t evaluate(ParseResult context) {
    int64_t result = evaluate_tree(context.tree);
    arena_destroy(&context.arena);

    return result;
}

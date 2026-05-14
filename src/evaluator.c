#include "evaluator.h"
#include "arena.h"
#include "lexer.h"
#include "parser.h"
#include <stdbool.h>
#include <stdint.h>
#include <math.h>


EvaluatorResult evaluate_tree(Node *tree) {
    if (tree->type == NODE_BINARY_OP) {
        return evaluate_binary(tree);
    } else if (tree->type == NODE_UNARY_OP) {
        return evaluate_unary(tree);
    }

    return (EvaluatorResult) {
        .is_valid = true,
        .val = tree->num,
    };
}

EvaluatorResult evaluate_binary(Node *tree) {
    Operator op = tree->binary.op;
    Node *left = tree->binary.left;
    Node *right = tree->binary.right;
    
    EvaluatorResult left_result = evaluate_tree(left);
    EvaluatorResult right_result = evaluate_tree(right);
    if (!left_result.is_valid || !right_result.is_valid) {
        return left_result;
    }

    switch (op) {
        case OP_ADD:
            return (EvaluatorResult) {
                .is_valid = true,
                .val = left_result.val + right_result.val,
            };
        case OP_SUB:
            return (EvaluatorResult) {
                .is_valid = true,
                .val = left_result.val - right_result.val,
            };
        case OP_MUL:
            return (EvaluatorResult) {
                .is_valid = true,
                .val = left_result.val * right_result.val,
            };
        case OP_DIV: 
            return (EvaluatorResult) {
                .is_valid = true,
                .val = left_result.val / right_result.val,
            };
        case OP_POW: 
            return (EvaluatorResult) {
                .is_valid = true,
                .val = pow(left_result.val, right_result.val),
            };
        default:
            return (EvaluatorResult) {
                .is_valid = false,
                .err = EVALUATOR_INVALID_TREE,
            };
    }
}

EvaluatorResult evaluate_unary(Node *tree) {
    Operator op = tree->unary.op;
    Node *to = tree->unary.to;

    EvaluatorResult result = evaluate_tree(to);
    if (!result.is_valid) {
        return result;
    }

    switch (op) {
        case OP_ADD:
            return result;
        case OP_SUB:
            return (EvaluatorResult) {
                .is_valid = true,
                .val = -result.val,
            };
        case OP_FACTORIAL:
            return (EvaluatorResult) {
                .is_valid = true,
                .val = tgamma(result.val + 1),
            };
        default:
            return (EvaluatorResult) {
                .is_valid = false,
                .err = EVALUATOR_INVALID_TREE,
            };
    }
}

EvaluatorResult evaluate(ParserResult context) {
    if (!context.is_valid) {
        return (EvaluatorResult) {
            .is_valid = false,
            .err = EVALUATOR_INVALID_PARSING,
        };
    }

    EvaluatorResult result = evaluate_tree(context.tree);

    arena_destroy(&context.arena);

    return result;
}

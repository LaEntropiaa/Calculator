#include "evaluator.h"
#include "arena.h"
#include "lexer.h"
#include "parser.h"
#include <stdbool.h>
#include <stdint.h>
#include <math.h>


EvaluatorResult evaluate_tree(Node *tree) {
    if (tree->type == NODE_BINARY_OP) {
        Operator op = tree->binary.op;
        Node *left = tree->binary.left;
        Node *right = tree->binary.right;
        
        switch (op) {
            case OP_ADD: {
                EvaluatorResult left_result = evaluate_tree(left);
                EvaluatorResult right_result = evaluate_tree(right);
                if (!left_result.is_valid || !right_result.is_valid) {
                    return left_result;
                }
                return (EvaluatorResult) {
                    .is_valid = true,
                    .val = left_result.val + right_result.val,
                };
            }
            case OP_SUB: {
                EvaluatorResult left_result = evaluate_tree(left);
                EvaluatorResult right_result = evaluate_tree(right);
                if (!left_result.is_valid || !right_result.is_valid) {
                    return left_result;
                }
                return (EvaluatorResult) {
                    .is_valid = true,
                    .val = left_result.val - right_result.val,
                };
            }
            case OP_MUL: {
                EvaluatorResult left_result = evaluate_tree(left);
                EvaluatorResult right_result = evaluate_tree(right);
                if (!left_result.is_valid || !right_result.is_valid) {
                    return left_result;
                }
                return (EvaluatorResult) {
                    .is_valid = true,
                    .val = left_result.val * right_result.val,
                };
            }
            case OP_DIV: {
                EvaluatorResult left_result = evaluate_tree(left);
                EvaluatorResult right_result = evaluate_tree(right);
                if (!left_result.is_valid || !right_result.is_valid) {
                    return left_result;
                }
                return (EvaluatorResult) {
                    .is_valid = true,
                    .val = left_result.val / right_result.val,
                };
            }
            case OP_POW: {
                EvaluatorResult left_result = evaluate_tree(left);
                EvaluatorResult right_result = evaluate_tree(right);
                if (!left_result.is_valid || !right_result.is_valid) {
                    return left_result;
                }
                return (EvaluatorResult) {
                    .is_valid = true,
                    .val = pow(left_result.val, right_result.val),
                };
            }
            default:
                return (EvaluatorResult) {
                    .is_valid = false,
                    .err = EVALUATOR_INVALID_TREE,
                };
        }
    } else if (tree->type == NODE_UNARY_OP) {
        
    }

    int64_t return_val = tree->data.integer;
    return return_val;
}

EvaluatorResult evaluate(ParseResult context) {
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

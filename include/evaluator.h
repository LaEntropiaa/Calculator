#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "lexer.h"
#include "parser.h"
#include <stdint.h>

typedef enum {
    EVALUATOR_OK,
    EVALUATOR_MATH_ERR,
    EVALUATOR_INVALID_PARSING,
    EVALUATOR_INVALID_TREE, // just to shut up the compiler with the swithces
} EvaluatorErr;

typedef struct {
    bool is_valid;
    union {
        int64_t val;
        EvaluatorErr err;
    };
} EvaluatorResult;

EvaluatorResult evaluate(ParserResult context);
EvaluatorResult evaluate_tree(Node *tree);

#endif // !EVALUATOR_H

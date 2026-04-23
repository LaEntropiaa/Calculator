#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "lexer.h"
#include "parser.h"
#include <stdint.h>

int64_t evaluate(ParseResult context);
int64_t evaluate_tree(ASTNode *tree);

#endif // !EVALUATOR_H

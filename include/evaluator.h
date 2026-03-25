#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "lexer.h"
#include <stdint.h>

uint64_t evaluate(ASTNode *tree);

#endif // !EVALUATOR_H

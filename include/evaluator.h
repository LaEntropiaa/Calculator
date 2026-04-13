#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "lexer.h"
#include "arena.h"
#include <stdint.h>

int64_t evaluate(ASTNode *tree);

#endif // !EVALUATOR_H

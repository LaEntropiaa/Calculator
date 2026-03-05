#include "lexer.h"

// Helps  state machine for the lexer :)
typedef enum {
    WAIT_FOR_NUMBER,
    WAIT_FOR_OPERATOR,
} LexerState;


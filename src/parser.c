#include "parser.h"
#include "arraylist.h"
#include "lexer.h"
#include "arena.h"
#include <cmocka.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

ParserU8Result prefix_rbp(Token token) {
    if (token.type == TOKEN_INTEGER) {
        return (ParserU8Result) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_TOKEN,
        };
    }
    switch (token.op) {
        case OP_SUB:
        case OP_ADD:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 30,
            };
        default:
            return (ParserU8Result) {
                .is_valid = false,
                .err = PARSER_UNEXPECTED_TOKEN,
            };
    }
}

ParserU8Result postfix_lbp(Token token) {
    if (token.type != TOKEN_OPERATOR) {
        return (ParserU8Result) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_TOKEN,
        };
    }

    switch (token.op) {
        case OP_FACTORIAL:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 40,
            };
        default:
            return (ParserU8Result) {
                .is_valid = false,
                .err = PARSER_UNEXPECTED_TOKEN,
            };
    }
}

ParserU8Result infix_lbp(Token token) {
    if (token.type != TOKEN_OPERATOR) {
        return (ParserU8Result) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_TOKEN,
        };
    }

    switch (token.op) {
        case OP_ADD:
        case OP_SUB:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 10,
            };
        case OP_DIV:
        case OP_MUL:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 20,
            };
        case OP_POW:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 51,
            };
        default:
            return (ParserU8Result) {
                .is_valid = false,
                .err = PARSER_UNEXPECTED_TOKEN,
            };
    }
}

ParserU8Result infix_rbp(Token token) {
    if (token.type != TOKEN_OPERATOR) {
        return (ParserU8Result) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_TOKEN,
        };
    }

    switch (token.op) {
        case OP_ADD:
        case OP_SUB:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 11,
            };
        case OP_DIV:
        case OP_MUL:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 21,
            };
        case OP_POW:
            return (ParserU8Result) {
                .is_valid = true,
                .num = 50,
            };
        default:
            return (ParserU8Result) {
                .is_valid = false,
                .err = PARSER_UNEXPECTED_TOKEN,
            };
    }
}

TreeResult led(
    ArraySlice *slice,
    Arena *arena,
    Node *left,
    Token token
) {
    arena_ensure_capacity(
        arena,
        sizeof(Node),
        alignof(Node)
    );

    Node *node = arena_unwrap_pointer(
        arena_alloc(
            arena,
            sizeof(Node),
            alignof(Node)
        )
    );

    switch (token.op) {

        // Binary operators
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_POW: {
            node->type = NODE_BINARY_OP;
            node->binary.op = token.op;

            ParserU8Result rbp_result = infix_rbp(token);
            if (!rbp_result.is_valid) {
                return (TreeResult) {
                    .is_valid = false,
                    .err = rbp_result.err,
                };
            }

            TreeResult right = parse_expr(
                slice,
                arena,
                rbp_result.num
            );

            if (!right.is_valid) {
                return right;
            }

            node->binary.left = left;
            node->binary.right = right.node;

            return (TreeResult) {
                .is_valid = true,
                .node = node,
            };
        }

        // Postfix operators
        case OP_FACTORIAL: {
            node->type = NODE_UNARY_OP;
            node->unary.op = token.op;
            node->unary.to = left;

            return (TreeResult) {
                .is_valid = true,
                .node = node,
            };
        }

        default:
            return (TreeResult) {
                .is_valid = false,
                .err = PARSER_UNEXPECTED_TOKEN,
            };
    }
}

TreeResult nud(ArraySlice *slice, Arena *arena, Token token) {
    arena_ensure_capacity(
        arena,
        sizeof(Node), 
        alignof(Node)
    );

    Node *node = arena_unwrap_pointer(
        arena_alloc(
            arena, 
            sizeof(Node),
            alignof(Node)
        )
    );

    if (token.type == TOKEN_INTEGER) {
        node->type = NODE_INT;
        node->num = token.num;

        return (TreeResult) {
            .is_valid = true,
            .node = node,
        };
    }

    switch (token.op) {
        case OP_START_PAR: {
            TreeResult expr = parse_expr(slice, arena, 0);
            if (!expr.is_valid) {
                return expr;
            }

            Token end_par;
            if (arrayslice_next(slice, &end_par) != ARRLIST_OK) {
                return (TreeResult) {
                    .is_valid = false,
                    .err = PARSER_UNMATCHED_PAREN,
                };
            }

            if (end_par.type != TOKEN_OPERATOR ||
                end_par.op != OP_END_PAR) {
                return (TreeResult) {
                    .is_valid = false,
                    .err = PARSER_UNMATCHED_PAREN,
                };
            }

            return expr;
        }
        case OP_ADD:

        case OP_SUB: {
            node->type = NODE_UNARY_OP;
            node->unary.op = token.op;

            ParserU8Result rbp_result = prefix_rbp(token);
            if (!rbp_result.is_valid) {
                return (TreeResult) {
                    .is_valid = false,
                    .err = rbp_result.err,
                };
            }

            TreeResult right = parse_expr(
                slice,
                arena,
                rbp_result.num
            );

            if (!right.is_valid) {
                return right;
            }

            node->unary.to = right.node;

            return (TreeResult) {
                .is_valid = true,
                .node = node,
            };
        }
        default:
            return (TreeResult) {
                .is_valid = false,
                .err = PARSER_UNEXPECTED_TOKEN,
        };
    }
}



ParserResult parse(TokenizeResult tokens) {
    if (!tokens.is_valid) {
        return (ParserResult) {
            .is_valid = false,
            .err = PARSER_INVALID_TOKENIZE,
        };
    }

    ArraySlice *context;
    arraylist_slice(&context, tokens.arr, 0, arraylist_size(tokens.arr));
    Arena *arena;
    arena_init(&arena, sizeof(Node) * arraylist_size(tokens.arr));

    TreeResult result = parse_expr(context, arena, 0);
    if (!result.is_valid) {
        arena_destroy(&arena);
        arraylist_destroy(&tokens.arr);
        return (ParserResult) {
            .is_valid = false,
            .err = result.err,
        };
    }

    arraylist_destroy(&tokens.arr);
    return (ParserResult) {
        .is_valid = true,
        .arena = arena,
        .tree = result.node};
}

TreeResult parse_expr(ArraySlice *slice, Arena *arena, uint8_t min_bp) {
    Token current_token;

    if (arrayslice_next(slice, &current_token) != ARRLIST_OK) {
        return (TreeResult) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_EOF,
        };
    }

    TreeResult left_result = nud(slice, arena, current_token);

    if (!left_result.is_valid) {
        return left_result;
    }

    Node *left_side = left_result.node;

    while (arrayslice_is_valid(slice)) {
        Token operator_token;
        arrayslice_peek(slice, &operator_token);

        if (operator_token.type != TOKEN_OPERATOR) {
            break;
        }

        ParserU8Result postfix_lbp_result = postfix_lbp(operator_token);

        if (postfix_lbp_result.is_valid) {
            if (postfix_lbp_result.num < min_bp) {
                break;
            }

            arrayslice_next(slice, NULL);

            TreeResult result = led(slice, arena, left_side, operator_token);

            if (!result.is_valid) {
                return result;
            }

            left_side = result.node;

            continue;
        }

        // Path for infix basically
        ParserU8Result lbp_result = infix_lbp(operator_token);

        if (!lbp_result.is_valid) {
            break;
        }

        if (lbp_result.num < min_bp) {
            break;
        }

        arrayslice_next(slice, NULL);

        TreeResult result = led(slice, arena, left_side, operator_token);

        if (!result.is_valid) {
            return result;
        }

        left_side = result.node;
    }

    // Final: return left side
    return (TreeResult){
        .is_valid = true,
        .node = left_side,
    };
}

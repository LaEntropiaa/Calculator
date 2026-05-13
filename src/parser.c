#include "parser.h"
#include "arraylist.h"
#include "lexer.h"
#include "arena.h"
#include <cmocka.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

ParserU8Result prefix_rbp(Node node) {
    if (node.type != NODE_UNARY_OP) {
        return (ParserU8Result) {
            .is_valid = false,
            .err =  PARSER_UNEXPECTED_TOKEN
        };
    }

    switch (node.unary.op) {
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

ParserU8Result postfix_lbp(Node node) {
    if (node.type != NODE_UNARY_OP) {
        return (ParserU8Result) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_TOKEN,
        };
    }

    switch (node.unary.op) {
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

ParserU8Result infix_lbp(Node node) {
    if (node.type != NODE_BINARY_OP) {
        return (ParserU8Result) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_TOKEN,
        };
    }

    switch (node.binary.op) {
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

ParserU8Result infix_rbp(Node node) {
    if (node.type != NODE_BINARY_OP) {
        return (ParserU8Result) {
            .is_valid = false,
            .err = PARSER_UNEXPECTED_TOKEN,
        };
    }

    switch (node.binary.op) {
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

Node token_to_node(Token token) {
    if (token.type == TOKEN_INTEGER) {
        return (Node) {
            .type = NODE_INT,
            .num = token.num,
        };
    }

    switch (token.op) {
        case OP_ADD:
            return (Node) {
                .type = NODE_BINARY_OP,
                .binary.op = token.op,
            };
        case OP_SUB:
            return (Node) {
                .type = NODE_BINARY_OP,
                .binary.op = token.op,
            };
        case OP_MUL:
            return (Node) {
                .type = NODE_BINARY_OP,
                .binary.op = token.op,
            };
        case OP_DIV:
            return (Node) {
                .type = NODE_BINARY_OP,
                .binary.op = token.op,
            };
        case OP_POW:
            return (Node) {
                .type = NODE_UNARY_OP,
                .binary.op = token.op,
            };
        case OP_FACTORIAL:
            return (Node) {
                .type = NODE_UNARY_OP,
                .binary.op = token.op,
            };
        case OP_START_PAR:
            return (Node) {
                .type = NODE_PARENTHESIS,
                .binary.op = token.op,
            };
        case OP_END_PAR:
            return (Node) {
                .type = NODE_PARENTHESIS,
                .binary.op = token.op,
            };
    }
}

ParserResult parse(TokenizeResult tokens) {
    ArraySlice *context = arraylist_slice(tokens.arr, 0, arraylist_size(tokens.arr));
    Arena arena = arena_init(sizeof(Node) * arraylist_size(tokens.arr)).arena;

    NodeResult result = parse_expr(context, &arena, 0);
    if (!result.is_valid) {
        return (ParserResult) {
            .is_valid = false,
            .err = result.err,
        };
    }

    return (ParserResult) {
        .is_valid = true,
        .arena = arena,
        .tree = result.node};
}

NodeResult parse_expr(ArraySlice *slice, Arena *arena, uint8_t min_bp) {
    arena_ensure_capacity(
        arena,
        sizeof(Node),
        alignof(Node)
    ); // shouldn't fail but if it does then what a shame

    // Get pointer in the arena
    Node *left_side = arena_unwrap_pointer(
        arena_alloc(
            arena,
            sizeof(Node),
            alignof(Node)
        )
    );

    // convert token to node :)
    Token current_token;
    arrayslice_next(slice, &current_token);
    *left_side = token_to_node(current_token);

    if (left_side->type == NODE_PARENTHESIS &&
        left_side->par == OP_START_PAR) {
        NodeResult result = parse_expr(slice, arena, 0);

        if (!result.is_valid) {
            return result;
        }

        left_side = result.node;

        // We dont really need to convert to node
        // parenthesis are there just to change up
        // the bp
        Token end_par;
        arrayslice_next(slice, &end_par);

        if (end_par.type != TOKEN_OPERATOR ||
            end_par.op != OP_END_PAR) {
            return (NodeResult) {
                .is_valid = false,
                .err = PARSER_UNMATCHED_PAREN,
            };
        }
        return (NodeResult) {
            .is_valid = true,
            .node = left_side,
        };
    }

    // If prefix
    if (left_side->type == NODE_UNARY_OP) {
        ParserU8Result rbp_result = prefix_rbp(*left_side);
        if (!rbp_result.is_valid) {
            return (NodeResult) {
                .is_valid = false,
                .err = rbp_result.err,
            };
        }

        NodeResult righ_side_result = parse_expr(slice, arena, rbp_result.num);
        if (!righ_side_result.is_valid) {
            return righ_side_result;
        }

        left_side->unary.to = righ_side_result.node;
    }

    while (true) {
        if (!arrayslice_is_valid(slice)) {
            break;
        }

        Token operator_token;
        arrayslice_peek(slice, &operator_token);
        if (operator_token.type != TOKEN_OPERATOR) {
            return (NodeResult) {
                .is_valid = false,
                .err = PARSER_MISSING_OPERAND,
            };
        }
        Node operator_node = token_to_node(operator_token);

        // temporary for bad error handling
        ParserU8Result postfix_lbp_result = postfix_lbp(operator_node);
        if (postfix_lbp_result.is_valid) {
            if (postfix_lbp_result.num < min_bp) {
                break;
            }

            // allocate operator
            arrayslice_next(slice, NULL);
            arena_ensure_capacity(
                arena,
                sizeof(Node),
                alignof(Node));
            Node *new_node = arena_unwrap_pointer(
                arena_alloc(
                    arena, 
                    sizeof(Node), 
                    alignof(Node)
                )
            );
            *new_node = operator_node;


            new_node->unary.to = left_side;

            left_side = new_node;
            continue;
        }

        ParserU8Result rbp_result = infix_rbp(operator_node);
        ParserU8Result lbp_result = infix_lbp(operator_node);
        if (!rbp_result.is_valid || !lbp_result.is_valid) {
            return (NodeResult) {
                .is_valid = false,
                .err = PARSER_UNEXPECTED_TOKEN,
            };
        }

        if (rbp_result.is_valid && lbp_result.is_valid) {
            // If lbp is LESS then stop recursion,
            // we found the next smaller binding power
            // or the one with more precedence
            if (lbp_result.num < min_bp) {
                break;
            }

            // If NOT, then we continue wtching ahead
            // for the next one but taking our current 
            // concern that is rbp of the current operator
            arrayslice_next(slice, NULL);
            NodeResult right_side_result = parse_expr(slice, arena, rbp_result.num);
            if (!right_side_result.is_valid) {
                return right_side_result;
            }

            arena_ensure_capacity(
                arena,
                sizeof(Node),
                alignof(Node));
            Node *new_node = arena_unwrap_pointer(
                arena_alloc(
                    arena, 
                    sizeof(Node), 
                    alignof(Node)
                )
            );
            *new_node = operator_node;

            new_node->binary.left = left_side;
            new_node->binary.right = right_side_result.node;

            left_side = new_node;

            continue;
        }

        break;
    }


    // Final: return left side
    return (NodeResult){
        .is_valid = true,
        .node = left_side,
    };
}

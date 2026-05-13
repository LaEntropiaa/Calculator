#include "parser.h"
#include "arraylist.h"
#include "lexer.h"
#include "arena.h"
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t prefix_rbp(ASTNode node) {
    if (node.type == NODE_INTEGER) {
        return 0;
    }

    switch (node.data.unary.op) {
        case OP_SUB:
        case OP_ADD:
            return 5;
        default:
            return -1;
    }
}

uint8_t infix_lbp(ASTNode node) {
    if (node.type == NODE_INTEGER) {
        return 0;
    }

    switch (node.data.binary.op) {
        case OP_ADD:
        case OP_SUB:
            return 10;
            break;
        case OP_DIV:
        case OP_MUL:
            return 20;
        case OP_POW:
            return 31;
        default:
            return 0;
    }
}

uint8_t infix_rbp(ASTNode node) {
    if (node.type == NODE_INTEGER) {
        return 0;
    }

    switch (node.data.binary.op) {
        case OP_ADD:
        case OP_SUB:
            return 11;
            break;
        case OP_DIV:
        case OP_MUL:
            return 21;
        case OP_POW:
            return 30;
        default:
            return 0;
    }
}

ParseResult parse(TokenizeResult tokens) {
    ArraySlice *context = arraylist_slice(tokens.arr, 0, arraylist_size(tokens.arr));
    Arena arena = arena_init(sizeof(ASTNode) * arraylist_size(tokens.arr)).arena;

    return (ParseResult) {
        .is_valid = true,
        .arena = arena,
        .tree = parse_expr(context, &arena, 0)};
}

ASTNode *parse_expr(ArraySlice *slice, Arena *arena, uint8_t min_bp) {
    // First: Consume a first number
    arena_ensure_capacity(
        arena,
        sizeof(ASTNode),
        alignof(ASTNode)
    ); // shouldn't fail but if it does then what a shame

    // Get pointer in the arena
    ASTNode *left_side = arena_unwrap_pointer(
        arena_alloc(
            arena,
            sizeof(ASTNode),
            alignof(ASTNode)
        )
    );

    // Should check if is Integer or number
    arrayslice_next(slice, left_side);

    while (true) {
        // Second: Get next one and checn bp
        if (!arrayslice_is_valid(slice)) {
            break;
        }

        ASTNode operator;
        // Here should chekc if is operator not some bs
        // Third, get operator and binding powers
        arrayslice_peek(slice, &operator);
        uint8_t rbp = infix_rbp(operator);
        uint8_t lbp = infix_lbp(operator);

        // If lbp is LESS then stop recursion,
        // we found the next smaller binding power
        // or the one with more precedence
        if (lbp < min_bp) {
            break;
        }


        // If NOT, then we continue wtching ahead
        // for the next one but taking our current 
        // concern that is rbp of the current operator
        arrayslice_next(slice, NULL);
        ASTNode *right_side = parse_expr(slice, arena, rbp);

        arena_ensure_capacity(
            arena,
            sizeof(ASTNode),
            alignof(ASTNode));
        ASTNode *new_node = arena_unwrap_pointer(
            arena_alloc(
                arena, 
                sizeof(ASTNode), 
                alignof(ASTNode)
            )
        );
        *new_node = operator;

        new_node->data.binary.left = left_side;
        new_node->data.binary.right = right_side;

        left_side = new_node;
    }


    // Final: return left side
    return left_side;
}




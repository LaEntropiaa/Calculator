#include "parser.h"
#include "arraylist.h"
#include "lexer.h"
#include "arena.h"
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t node_lbp(ASTNode node) {
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
        default:
            return 0;
    }
}

uint8_t node_rbp(ASTNode node) {
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
    arena_ensure_capacity(
        arena,
        sizeof(ASTNode),
        alignof(ASTNode)
    );

    ASTNode *left_side = arena_unwrap_pointer(
        arena_alloc(
            arena,
            sizeof(ASTNode),
            alignof(ASTNode)
        )
    );

    arrayslice_next(slice, left_side);

    while (true) {
        if (!arrayslice_is_valid(slice)) {
            break;
        }

        ASTNode operator;
        arrayslice_peek(slice, &operator);
        uint8_t rbp = node_rbp(operator);
        uint8_t lbp = node_lbp(operator);

        if (lbp < min_bp) {
            break;
        }

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


    return left_side;
}




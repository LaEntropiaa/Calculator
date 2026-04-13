#include "parser.h"
#include "lexer.h"
#include "arena.h"
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


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

ASTNode ASTNodeSlice_next(ASTNodeSlice *slice) {
    return slice->arr->data[slice->pos++];
}

ASTNode ASTNodeSlice_peek(ASTNodeSlice *slice) {
    return slice->arr->data[slice->pos];
}

bool ASTNodeSlice_is_valid(ASTNodeSlice *slice) {
    if (slice->arr->len < 1) {
        return false;
    }
    if (slice->pos >= slice->arr->len) {
        return false;
    }

    return true;
}

ParseResult parse(ASTNodeArray *arr) {
    AST tree;
    ASTNodeSlice context = {
        .arr = arr,
        .pos = 0,
    };
    Arena arena = arena_init(sizeof(ASTNode) * arr->len).arena;

    tree.head = parse_expr(&context, &arena, 0);
    return (ParseResult) {.arena = &arena, .tree = tree};
}

ASTNode *parse_expr(ASTNodeSlice *slice, Arena *arena, uint8_t min_bp) {
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

    *left_side = ASTNodeSlice_next(slice);

    while (true) {
        if (!ASTNodeSlice_is_valid(slice)) {
            break;
        }

        ASTNode operator = ASTNodeSlice_peek(slice);
        uint8_t rbp = node_rbp(operator);
        uint8_t lbp = node_lbp(operator);

        if (lbp < min_bp) {
            break;
        }

        ASTNodeSlice_next(slice);
        ASTNode *right_side = parse_expr(slice, arena, rbp);

        ASTNode *new_node = malloc(sizeof(ASTNode));
        *new_node = operator;

        new_node->data.binary.left = left_side;
        new_node->data.binary.right = right_side;

        left_side = new_node;
    }


    return left_side;
}




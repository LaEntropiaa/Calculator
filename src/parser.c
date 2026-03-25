#include "parser.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
            return 30;
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
            return 20;
            break;
        case OP_DIV:
        case OP_MUL:
            return 40;
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
    if (slice->pos >= slice->arr->len - 1) {
        return false;
    }

    return true;
}

AST parse(ASTNodeArray *arr) {
    AST tree;
    ASTNodeSlice context = {
        .arr = arr,
        .pos = 0,
    };

    tree.head = parse_expr(&context, 0);
    return tree;
}

ASTNode *parse_expr(ASTNodeSlice *slice, uint8_t min_bp) {
    ASTNode *left_side = malloc(sizeof(ASTNode));
    *left_side = ASTNodeSlice_next(slice);

    while (true) {
        if (ASTNodeSlice_is_valid(slice)) {
            break;
        }

        ASTNode operator = operator = ASTNodeSlice_peek(slice);
        uint8_t rbp = node_rbp(operator);
        uint8_t lbp = node_lbp(operator);

        if (lbp < min_bp) {
            break;
        }

        ASTNodeSlice_next(slice);
        ASTNode *right_side = parse_expr(slice, rbp);

        ASTNode *new_node = malloc(sizeof(ASTNode));
        *new_node = operator;

        new_node->data.binary.left = left_side;
        new_node->data.binary.right = right_side;

        left_side = new_node;
    }


    return left_side;
}

void print_AST(AST tree) {
    print_node(tree.head, 0);
}

void print_node(ASTNode *node, int depth) {
    if (!node) return;

    if (node->type == NODE_BINARY_OP) {
        print_node(node->data.binary.right, depth + 1);
    }

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    switch (node->type) {
        case NODE_INTEGER:
            printf("%ld\n", node->data.integer);
            break;

        case NODE_BINARY_OP:
            printf("%c\n", operator_to_char(node->data.binary.op));
            break;
    }

    if (node->type == NODE_BINARY_OP) {
        print_node(node->data.binary.left, depth + 1);
    }
}


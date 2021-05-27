#include "stack.h"

#include "lang.h"

#include <stdlib.h>
#include <string.h>

stack_t stack_init(size_t max_capacity) {
    return (stack_t) {
        .size = 0,
        .capacity = max_capacity * sizeof(cell_t),
        .cells = (cell_t*) calloc(max_capacity, sizeof(cell_t)),
    };
}

cell_t stack_pop(stack_t *stack) {
    if (stack->size) {
        stack->size -= sizeof(cell_t);
        return *(stack->cells + stack->size);
    } else {
        cell_t cell;
        bzero(&cell, sizeof(cell_t));
        return cell;
    }
}

void stack_push(stack_t *stack, cell_t val) {
    if (stack->size == stack->capacity)
        error("Stack overflow");

    *(stack->cells + stack->size) = val;
    stack->size += sizeof(cell_t);
}
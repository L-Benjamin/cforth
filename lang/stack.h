#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>
#include <stdlib.h>

/* === cell_t === */

/* A cell of the stack. */
typedef union {
    int64_t i;
    double  f;
    char    c;
} cell_t;

/* === stack_t === */

/* A struct to hold a complete stack. */
typedef struct {
    size_t size, capacity;
    cell_t *cells;
} stack_t;

/* Initialize a new stack with the given initial capacity, in cells. */
stack_t stack_init(size_t max_capacity);

/* Pop the last element of the stack and returns a pointer to it. */
cell_t stack_pop(stack_t *stack);

/* Push a new element onto the top of the stack. */
void stack_push(stack_t *stack, cell_t cell);

#endif
#ifndef __WORD_H__
#define __WORD_H__

#include "stack.h"

struct list;

/* === word_data_t === */

/* The data associated to a word. */
typedef union {
    cell_t cell;
    char *str;
    struct list *list;
    struct list *lists[2];
} word_data_t;

/* === word_behaviour_t === */

/* 
 * The behaviour of a word: 
 * - act is called when the word is read, and should modify the stack accoringly to the
 *   word's behaviour and data.
 * - free_data is called upon freeing the word and should free the data given.
 *   May be NULL if unecessary.
 */
typedef struct {
    void (*act)(stack_t *stack, word_data_t *data);
    void (*free_data)(word_data_t *data);
} word_behaviour_t;

/* === word_t === */

/* 
 * A word, with it's name, behaviour and data.
 * It's name may be NULL, in that case the word will be anonymous
 * and should not be shared.
 * If it is not, calling word_free will have no effect on the word.
 */
typedef struct {
    const char *name;
    word_behaviour_t behaviour;
    word_data_t data;
} word_t;

/* 
 * Initialize a new undefined word with the given name.
 * The name should be NULL unless the word is made to be inserted
 * in the dictionary.
 */
word_t *word_init(const char *name);

/* Make the word act upon the stack. */
void word_act(word_t *word, stack_t *stack);

/* Define a word with it's associated behaviour and data. */
void word_define(word_t *word, word_behaviour_t behaviour, word_data_t data);

/* Delete the word if necessary. */
void word_free(word_t *word);

#endif
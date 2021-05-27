#ifndef __DICT_H__
#define __DICT_H__

#include "word.h"

/* === dict_t === */

/* A dictionary, a collection of words indexed by names. */
typedef struct {
    size_t size, capacity;
    word_t **words;
} dict_t;

/* 
 * Create a new empty dictionary.
 */
dict_t dict_init(void);

/* 
 * Get the word corresponding to that name.
 * Create and insert a new undefined word with that name if it does not exist yet.
 */
word_t *dict_get(dict_t *dict, const char *name);

#endif
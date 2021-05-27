#ifndef __LIST_H__
#define __LIST_H__

#include "word.h"

#include <stdlib.h>

/* === list_t === */

/* A fixed list of words. */
typedef struct list {
    size_t size;
    word_t *words[];
} list_t;

/* Get the size of the list. */
size_t list_size(const list_t *list);

/* Get the ith word of the list. */
word_t *list_get(const list_t *list, size_t i);

/* Free the list. */
void list_free(list_t *list);

/* === wbuf_t === */

/* A dynamic buffer of words. */
typedef struct {
    size_t size, capacity;
    const word_t **words;
} wbuf_t;

/* Initialize a new buffer. */
wbuf_t wbuf_init(void);

/* Push a word onto the buffer. */
void wbuf_push(wbuf_t *wbuf, const word_t *word);

/* Dump the last n words in a new list. */
list_t *wbuf_dumpn(wbuf_t *wbuf, size_t n);

#endif
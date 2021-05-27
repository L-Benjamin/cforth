#include "list.h"

#include <stdlib.h>
#include <string.h>

#define WBUF_INITIAL_CAPACITY 128

size_t list_size(const list_t *list) {
    return list->size;
}

word_t *list_get(const list_t *list, size_t i) {
    return list->words[i];
}

void list_free(list_t *list) {
    size_t i = 0;
    for (; i < list->size; i++)
        word_free(list->words[i]);
    free(list);
}

wbuf_t wbuf_init(void) {
    return (wbuf_t) {
        .size = 0,
        .capacity = WBUF_INITIAL_CAPACITY,
        .words = (const word_t**) malloc(WBUF_INITIAL_CAPACITY * sizeof(word_t*)),
    };
}

void wbuf_push(wbuf_t *wbuf, const word_t *word) {
    if (wbuf->size == wbuf->capacity) {
        wbuf->capacity *= 2;
        wbuf->words = (const word_t**) malloc(wbuf->capacity * sizeof(word_t*));
    }

    wbuf->words[wbuf->size++] = word;
}

list_t *wbuf_dumpn(wbuf_t *wbuf, size_t n) {
    wbuf->size -= n;

    list_t *list = (list_t*) malloc(sizeof(list_t) + n * sizeof(word_t*));
    list->size = n;
    memcpy(list->words, &wbuf->words[wbuf->size], n * sizeof(word_t*));
    
    return list;
}
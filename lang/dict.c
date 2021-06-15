#include "dict.h"

#include <stdlib.h>
#include <string.h>

/* The load factor of the hashmap. */
#define LOAD_FACTOR 0.75f

/* The initial capacity of a hashmap. */
#define INITIAL_CAPACITY 1024

/* The string hash function. */
static size_t strhash(const char *str) {
    size_t hash = 5381, i = 0;

	while (str[i])
		hash += (hash << 5) + str[i++];

    return hash;
}

dict_t dict_init(void) {
    return (dict_t) {
        .size = 0,
        .capacity = INITIAL_CAPACITY,
        .words = (word_t**) calloc(INITIAL_CAPACITY, sizeof(word_t*)),
    };
}

static void insert(dict_t *dict, word_t *word) {
    if ((float) dict->size >= (float) dict->capacity * LOAD_FACTOR) {
        size_t old_capacity = dict->capacity, i = 0;
        word_t **old_words = dict->words;

        dict->size = 0;
        dict->capacity *= 2;
        dict->words = (word_t**) calloc(dict->capacity, sizeof(word_t*));

        for (; i < old_capacity; i++)
            if (old_words[i])
                insert(dict, old_words[i]);

        free(old_words);
    }

    size_t i = strhash(word->name) % dict->capacity;
    word_t *w;

    while ((w = dict->words[i]))
        i = (i + 1) % dict->capacity;

    dict->size++;
    dict->words[i] = word;
}

word_t *dict_get(dict_t *dict, const char *name) {
    size_t i = strhash(name) % dict->capacity;
    word_t *w;

    while ((w = dict->words[i])) {
        if (strcmp(w->name, name) == 0)
            return w;
        i = (i + 1) % dict->capacity;
    }

    w = word_init(name);
    insert(dict, w);
    return w;
}
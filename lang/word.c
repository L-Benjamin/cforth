#include "word.h"

#include "lang.h"

#include <stdlib.h>
#include <string.h>

void undefined_act(stack_t *stack, word_data_t *data) {
    error("Undefined word");
}

const word_behaviour_t undefined = {
    .act = undefined_act, 
    .free_data = NULL,
};

word_t *word_init(const char *name) {
    word_t *word = (word_t*) malloc(sizeof(word_t));
    char* name_clone = NULL;
    if (name) {
        name_clone = (char*) malloc((strlen(name) + 1) * sizeof(char));
        strcpy(name_clone, name);
    }
    word->name = name_clone;
    word->behaviour = undefined;
    return word;
}

void word_act(word_t *word, stack_t *stack) {
    word->behaviour.act(stack, &word->data);
}

void word_define(word_t *word, word_behaviour_t behaviour, word_data_t data) {
    word->behaviour = behaviour;
    word->data = data;
}

void word_free(word_t *word) {
    if (!word->name) {
        if (word->behaviour.free_data)
            word->behaviour.free_data(&word->data);
        free(word);
    }
}
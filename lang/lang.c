#include "lang.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
char BUFFER[BUFFER_SIZE];

#define ACTFN(name) void name##_actfn(stack_t *stack, word_data_t *data)
#define FREEFN(name) void name##_freefn(word_data_t *data)
#define DEFINE(name, sym) w = dict_get(dict, sym); word_define(w, (word_behaviour_t) {.act = name##_actfn, .free_data = NULL}, nodata);
#define BEHAVIOUR(name) (word_behaviour_t) {.act = name##_actfn, .free_data = name##_freefn}

/* === error function === */

void error(const char *msg) {
    fprintf(stderr, "Error: %s.\n", msg);
    exit(EXIT_FAILURE);
}

/* === literals === */

ACTFN(value) {
    stack_push(stack, data->cell);
}

static void make_value(word_t *word, cell_t cell) {
    word_define(word, (word_behaviour_t) {.act = value_actfn, .free_data = NULL}, (word_data_t) {.cell = cell});
}

word_t *make_int(const char *src) {
    int64_t i;
    sscanf(src, "%ld", &i);
    
    word_t *word = word_init(NULL);
    make_value(word, (cell_t) {.i = i});
    return word;
}

word_t *make_float(const char *src) {
    double f;
    sscanf(src, "%lf", &f);

    word_t *word = word_init(NULL);
    make_value(word, (cell_t) {.f = f});
    return word;
}

ACTFN(string) {
    size_t len = strlen(data->str), i = 0;
    for (; i < len+1; i++)
        stack_push(stack, (cell_t) {.c = data->str[len - i]});
}

FREEFN(string) {
    free(data->str);
}

#define ESCAPE_SEQUENCE(c1, c2) case c1: str[j++] = c2; break;

word_t *make_str(const char *src) {
    size_t len = strlen(src);
    char *str = (char*) malloc((len + 1) * sizeof(char));

    size_t i = 0, j = 0;
    while (src[++i] != '\"') {
        if (!src[i]) error("Invalid string litteral");

        if (src[i] == '\\') {
            switch (src[++i]) {
                ESCAPE_SEQUENCE('q', '\"')
                ESCAPE_SEQUENCE('a', '\a')
                ESCAPE_SEQUENCE('b', '\\')
                ESCAPE_SEQUENCE('t', '\t')
                ESCAPE_SEQUENCE('n', '\n')
                ESCAPE_SEQUENCE('0', '\0')
                default:
                    error("Invalid escape sequence");
            }
        } else str[j++] = src[i];
    }

    word_t *word = word_init(NULL);
    word_define(word, BEHAVIOUR(string), (word_data_t) {.str = str});
    return word;
}

/* === sentence === */

ACTFN(sentence) {
    size_t i = 0;
    for (; i < list_size(data->list); i++)
        word_act(list_get(data->list, i), stack);
}

FREEFN(sentence) {
    list_free(data->list);
}

void make_sentence(word_t *word, list_t *list) {
    word_define(word, BEHAVIOUR(sentence), (word_data_t) {.list = list});
}

/* === if === */

ACTFN(if) {
    list_t *list;
    if (stack_pop(stack).i)
        list = data->lists[0];
    else
        list = data->lists[1];

    if (list) {
        size_t i = 0; 
        for (; i < list_size(list); i++)
            word_act(list_get(list, i), stack);
    }
        
}

FREEFN(if) {
    list_free(data->lists[0]);
    list_free(data->lists[1]);
}

word_t *make_if(list_t *iftrue, list_t *iffalse) {
    word_t *word = word_init(NULL);
    word_define(word, BEHAVIOUR(if), (word_data_t) {.lists = {iftrue, iffalse}});
    return word;
}

/* === while === */

ACTFN(while) {
    while (stack_pop(stack).i) {
        size_t i = 0;
        for (; i < list_size(data->list); i++)
            word_act(list_get(data->list, i), stack);
    }
}

FREEFN(while) {
    list_free(data->list);
}

word_t *make_while(list_t *list) {
    word_t *word = word_init(NULL);
    word_define(word, BEHAVIOUR(while), (word_data_t) {.list = list});
    return word;
}

/* === integer arithmetic === */

ACTFN(add) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i + stack_pop(stack).i,
    });
}

ACTFN(sub) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i - stack_pop(stack).i,
    });
}

ACTFN(mul) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i * stack_pop(stack).i,
    });
}

ACTFN(div) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i / stack_pop(stack).i,
    });
}

ACTFN(neg) {
    stack_push(stack, (cell_t) {
        .i = -stack_pop(stack).i,
    });
}

ACTFN(inc) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i + 1,
    });
}

ACTFN(dec) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i - 1,
    });
}

/* === floating-point arithmetic === */

ACTFN(add_float) {
    stack_push(stack, (cell_t) {
        .f = stack_pop(stack).f + stack_pop(stack).f,
    });
}

ACTFN(sub_float) {
    stack_push(stack, (cell_t) {
        .f = stack_pop(stack).f - stack_pop(stack).f,
    });
}

ACTFN(mul_float) {
    stack_push(stack, (cell_t) {
        .f = stack_pop(stack).f * stack_pop(stack).f,
    });
}

ACTFN(div_float) {
    stack_push(stack, (cell_t) {
        .f = stack_pop(stack).f / stack_pop(stack).f,
    });
}

ACTFN(neg_float) {
    stack_push(stack, (cell_t) {
        .f = -stack_pop(stack).f,
    });
}

/* === io === */

static void write_string(stack_t *stack) {
    size_t n = strlen(BUFFER), i = 0;
    for (; i < n + 1; i++)
        stack_push(stack, (cell_t) {.c = BUFFER[n - i]});
}

static void read_string(stack_t *stack) {
    size_t i = 0; char c;
    do {
        BUFFER[i++] = (c = stack_pop(stack).c);
    } while(c);
}

ACTFN(in) {
    scanf("%s", BUFFER);
    write_string(stack);
}

ACTFN(put) {
    printf("%c", stack_pop(stack).c);
}

ACTFN(out) {
    read_string(stack);
    printf("%s", BUFFER);
}

ACTFN(strtoi) {
    read_string(stack);
    cell_t cell;
    sscanf(BUFFER, "%ld", &cell.i);
    stack_push(stack, cell);
}

ACTFN(strtof) {
    read_string(stack);
    cell_t cell;
    sscanf(BUFFER, "%lf", &cell.f);
    stack_push(stack, cell);
}

ACTFN(itostr) {
    sprintf(BUFFER, "%ld", stack_pop(stack).i);
    write_string(stack);
}

ACTFN(ftostr) {
    sprintf(BUFFER, "%lf", stack_pop(stack).f);
    write_string(stack);
}

ACTFN(endl) {
    printf("\n");
}

/* === comparisons === */

ACTFN(eq) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i == stack_pop(stack).i,
    });
}

ACTFN(neq) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i != stack_pop(stack).i,
    });
}

ACTFN(lt) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i < stack_pop(stack).i,
    });
}

ACTFN(gt) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i > stack_pop(stack).i,
    });
}

ACTFN(leq) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i <= stack_pop(stack).i,
    });
}

ACTFN(geq) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i >= stack_pop(stack).i,
    });
}

/* === logical === */

ACTFN(and) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i & stack_pop(stack).i,
    });
}

ACTFN(or) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i | stack_pop(stack).i,
    });
}

ACTFN(xor) {
    stack_push(stack, (cell_t) {
        .i = stack_pop(stack).i ^ stack_pop(stack).i,
    });
}

ACTFN(not) {
    stack_push(stack, (cell_t) {
        .i = ~stack_pop(stack).i,
    });
}

/* === stack handling === */

ACTFN(dup) {
    cell_t cell = stack_pop(stack);
    stack_push(stack, cell);
    stack_push(stack, cell);
}

ACTFN(pop) {
    stack_pop(stack);
}

ACTFN(swap) {
    cell_t c1 = stack_pop(stack), c2 = stack_pop(stack);
    stack_push(stack, c1);
    stack_push(stack, c2);
}

/* === initializer === */

void fill_dict(dict_t *dict) {
    word_data_t nodata;
    bzero(&nodata, sizeof(word_data_t));

    word_t *w;

    DEFINE(add, "+");
    DEFINE(sub, "-");
    DEFINE(mul, "*");
    DEFINE(div, "/");
    DEFINE(neg, "neg");
    DEFINE(inc, "inc");
    DEFINE(dec, "dec");

    DEFINE(add_float, "+.");
    DEFINE(sub_float, "-.");
    DEFINE(mul_float, "*.");
    DEFINE(div_float, "/.");
    DEFINE(neg_float, "neg.");

    DEFINE(in,  "in");
    DEFINE(out, "out");
    DEFINE(put, "put");
    DEFINE(strtoi, "strtoi");
    DEFINE(strtof, "strtof");
    DEFINE(itostr, "itostr");
    DEFINE(ftostr, "ftostr");
    DEFINE(endl, "endl");

    DEFINE(eq,  "=");
    DEFINE(neq, "!=");
    DEFINE(lt,  "<");
    DEFINE(gt,  ">");
    DEFINE(leq, "<=");
    DEFINE(geq, ">=");

    DEFINE(and, "and");
    DEFINE(or,  "or");
    DEFINE(xor, "xor");
    DEFINE(not, "not");

    DEFINE(dup,  "dup");
    DEFINE(pop,  "pop");
    DEFINE(swap, "swap");
}

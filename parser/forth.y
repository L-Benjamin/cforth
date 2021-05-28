%{
#include "lang.h"
#include "word.h"

#include <stdio.h>

#include "lex.yy.c"

#define INITIAL_STACK_SIZE 2048

dict_t DICT;
stack_t STACK;
wbuf_t WBUF;

int yyerror(char *msg);

extern FILE* yyin;
%}

%union {
    word_t *word;
    size_t size;
}

/* Literal */
%token INT
%token FLOAT
%token STRING

/* Flow control */
%token DEF          
%token END_DEF      
%token IF
%token ELSE
%token END
%token WHILE

/* Identifier */
%token ID

/* Nonterminals */
%start program
%type<word> word
%type<size> sentence
%type<word> id

%%

program     :                                       {}        
            |   program word                        { word_act($2, &STACK); word_free($2); }
            |   program DEF id sentence END_DEF     { make_sentence($3, wbuf_dumpn(&WBUF, $4)); }
;

word        :   IF sentence ELSE sentence END       { list_t *iffalse = wbuf_dumpn(&WBUF, $4); $$ = make_if(wbuf_dumpn(&WBUF, $2), iffalse); }
            |   IF sentence END                     { $$ = make_if(wbuf_dumpn(&WBUF, $2), NULL); }
            |   WHILE sentence END                  { $$ = make_while(wbuf_dumpn(&WBUF, $2)); }
            |   INT                                 { $$ = make_int(yytext); }
            |   FLOAT                               { $$ = make_float(yytext); }
            |   STRING                              { $$ = make_str(yytext); }
            |   id
;

sentence    :                                       { $$ = 0; }
            |   sentence word                       { wbuf_push(&WBUF, $2); $$ = $1+1; }
;

id          :   ID                                  { $$ = dict_get(&DICT, yytext); }
;

%%

int yyerror(char *msg) {
    error("Invalid syntax");
}

int main(int argc, char **argv) {
    DICT = dict_init();
    fill_dict(&DICT);

    STACK = stack_init(INITIAL_STACK_SIZE);
    WBUF = wbuf_init();

    if (argc != 2) error("Invalid number of arguments");

    FILE* in = fopen(argv[1], "r");

    if (!in) error("Can't open source file");

    yyin = in;
    yyparse();

    return EXIT_SUCCESS;
}
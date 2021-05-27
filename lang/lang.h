#ifndef __LANG_H__
#define __LANG_H__

#include "dict.h"
#include "list.h"

/* Print the error message and exit the program. */
void error(const char *msg);

/* Create a new anonymous word that pushes an int (parsed from src) to the stack. */
word_t *make_int(const char *src);

/* Create a new anonymous word that pushes a float (parsed from src) to the stack. */
word_t *make_float(const char *src);

/* Create a new anonymous word that pushes a string (parsed from src) to the stack. */
word_t *make_str(const char *src);

/* 
 * Create a new anonymous word that when invoked, performs the words
 * in the ifftrue list if the last value in the stack has a truthy
 * value, or performs the words in the iffalse list.
 */
word_t *make_if(list_t *iftrue, list_t *iffalse);

/* 
 * Create a new anonymous word that when invoked, performs the words
 * in the list if the last value in the stack has a truthy, and
 * keeps performing them until the last value on the stack becomes falthy.
 */
word_t *make_while(list_t *list);

/* 
 * Create a new anonymous word that when invoked, performs the words
 * in the list.
 */
void make_sentence(word_t *word, list_t *list);

/* Initialize the language's facilities. */
void fill_dict(dict_t *dict);

#endif
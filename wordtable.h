#ifndef wordtable_H
#define wordtable_H

#include "utility.h"
#define WORD_SIZE 14 /*size of word (in bits)*/
/** pointer to A specific word in the table  . */
typedef struct word* word_table;

/* A single table entry */
typedef struct word {
	/* Next word in table */
	word_table next;
	/* the word translated to binary */
	char bin_word[WORD_SIZE];
	/* word address */
	int address;
	/* word title */
	char *string;
} word;

void add_word(word_table * tab, char *bin_word, int address, char * string);
void free_word_table(word_table tab);
void add_value_to_address(word_table tab,int C);
word_table last_word  (word_table tab);
void add_int(word_table * tab,int val,int address,sentence_type type);
#endif
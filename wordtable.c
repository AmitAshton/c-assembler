

#include "wordtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_word(word_table * tab, char *bin_word, int address, char * string) {

	char * temp_string;
	word_table prev_entry, curr_entry, new_entry;
	/* allocate memory for new entry */
	new_entry =(word *) malloc(sizeof(word));
	temp_string = (char *) malloc(strlen(string) + 1);
	strcpy(temp_string,string);
	(*new_entry).string = temp_string;
	strcpy((*new_entry).bin_word,bin_word);
	(*new_entry).address = address;
	/* if the table's null, set the new entry as the head. */
	if ((*tab) == NULL) {
		new_entry->next = (*tab);
		(*tab) = new_entry; 
		return;
	}

	/* Insert the new table entry */
	curr_entry = (*tab)->next;
	prev_entry = (*tab);
	while (curr_entry != NULL) {
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
	}

	new_entry->next = curr_entry;
	prev_entry->next = new_entry;
	return;
}    

void free_word_table(word_table tab) {
	word_table prev_entry, curr_entry = tab;
	while (curr_entry != NULL) {
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
		free(prev_entry->bin_word);
		free(prev_entry->string);
		free(prev_entry);
	}
}

void add_value_to_address(word_table tab,int C)
{
word_table curr_entry;

for (curr_entry = tab; curr_entry != NULL; curr_entry = (*curr_entry).next) 
(*curr_entry).address+=C;
}
word_table last_word  (word_table tab)
{
	word_table last = tab;
	while((*last).next!=NULL)
	last = (*last).next;
	return last;
}
void add_int(word_table * tab,int val,int address,sentence_type type)
{
char bin_val[WORD_SIZE];
char *val_str = (char*) malloc (snprintf(NULL, 0, "%d", val)+1);

sprintf(val_str, "%d", val);
if(type==TYPE_DATA)
{
strcpy(bin_val,decimal_to_binary(val,bin_val,13));
add_word(tab,bin_val,address,val_str);
}
if(type==TYPE_OPERATION)
{
strcpy(bin_val,decimal_to_binary(val,bin_val,11));
add_word(tab,bin_val,address,val_str);
}
return;
}

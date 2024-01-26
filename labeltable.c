#include "labeltable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void add_label(label_table * tab, char *name, int value, symbol_type type)/*adds the specified label stats to the label table*/
{
	char *temp_name;
	label_table prev_entry, curr_entry, new_entry;
	/* allocate memory for new entry */
	new_entry = (label_table) malloc(sizeof(label));
	/* Prevent "Aliasing" of pointers. Don't worry-when we free the list, we also free these allocated char ptrs */
	temp_name = (char *) malloc(strlen(name) + 1);
	strcpy(temp_name, name);
	new_entry->name = temp_name;/*copies label name*/
	new_entry->value = value;/*copies label value*/
	new_entry->type = type;/*copies label type*/
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

void free_label_table(label_table tab)/*frees the table*/
{
	label_table prev_entry, curr_entry = tab;
	while (curr_entry != NULL)
	{
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
		free(prev_entry->name);/*frees name pointer*/
		free(prev_entry);/*frees entire structure*/
	}
}
void add_value_by_type(label_table tab, int to_add, symbol_type type)/*adds value to all the labels with the specified type*/
{
	label_table curr_entry;
	/* for each entry, add value to_add if same type */
	for (curr_entry = tab; curr_entry != NULL; curr_entry = curr_entry->next)
	{
		if (curr_entry->type == type) {
			curr_entry->value += to_add;
		}
	}
}


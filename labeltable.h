#define EXTERN_VAL 0	
typedef enum symbol_type {
	CODE_SYMBOL,
	DATA_SYMBOL,
	EXTERNAL_SYMBOL,
	ENTRY_SYMBOL
} symbol_type;

/* pointer to A specific label in the table  . */
typedef struct label* label_table;

/* A single table entry */
typedef struct label {
	/* Next label in table */
	label_table next;
	/* Address of the label */
	int value;
	/* symbol name */
	char *name;
	/* Symbol type */
	symbol_type type;
} label;

void add_label(label_table tab, char *name, int value, symbol_type type);/*adds the specified label stats to the label table*/
void free_label_table(label_table tab);/*frees the table*/
void add_value_by_type(label_table tab, int to_add, symbol_type type);/*adds value to all the labels with the specified type*/
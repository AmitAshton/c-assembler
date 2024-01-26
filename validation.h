#ifndef VALIDATION_H
#define VALIDATION_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*max characters for a single line*/
#define MAX_CHAR_LINE 82

/*max characters for a label*/
#define MAX_CHAR_LABEL 32

/*flag for errors*/
extern bool error_found;


/*operation saved assembly words*/
extern char *OPERATIONS[];
extern char *JUMP_OPERATIONS[];

/*instructions saved assembly words*/
extern char *INSTRUCTIONS[];

/*registers saved assembly words*/
extern char *REGISTERS[];


bool validation(char * filename);
int how_many_operands(char word[], int index);
bool is_label(char *word[], int i, char *filename);
bool is_operation(char *words[], int i, char *filename, int label_exists);
bool is_instruction(char *words[], int i, char *filename, int label_exists);

#endif
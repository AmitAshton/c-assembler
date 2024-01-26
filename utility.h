#ifndef utility_H
#define utility_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
/*max length of a sentence in the assembly code*/
#define MAXCHARS 81
/*max length of a macro name. 75 = MAXCHARS - number of chars in "mcro " */
#define MCRMAX 75
#define GET_TO_FIRST_LETTER(string) \
	for (;*string!='\0' && (*string == '\t' || *string == ' '|| *string == ','||*string == ':'); (++(string)))\
;
#define READ_UNTIL_SIGN(string,word,i) \
	word = (char*)malloc(MAXCHARS);\
	for (;*string!='\0' && *string != '\t' && *string != ' '&&*string!=','&&*string!=')'&&*string != ':' ; ++i,++string)\
	{\
		*(word+i)=*(string);\
	}\
	*(word+i)='\0';\
	i=0;

#define NAME_FEATURE 0
#define CODING_FEATURE 1
#define OPERAND_NUMBER_FEATURE 2
#define OPERATIONS_ROW 16
#define REGISTERS_ROW 8
#define OPCODE_SIZE 4
#define LAST_BIT 14
#define ONE_DIM_ARRAY_SIZE 2
#define ZERO_WORD "00000000000000"
#define INITIAL_ADDRESS 100
#define MAX_WORDS 156

extern char * operations [][OPERATIONS_ROW];/*a table of the operations and their different features*/

extern char * registers[][REGISTERS_ROW];/*a table of the registers and their binary coding*/

extern char * datawords[];/*includes ".string" and ".data" commands*/

extern char * authorizations[];/*includes ".entry" and ".extern" commands*/

/*types of sentences*/
typedef enum {
	TYPE_DATA,
	TYPE_AUTHORIZATION,
	TYPE_OPERATION,
	TYPE_LABEL
} sentence_type;

/*specifies if there was already a register operand*/
typedef enum {
	FIRST,
	ADDED,
	NOT_ADDED
} register_status;


bool skippable_line (char* str);/*checks if line is skippable(empty/note line)*/
char * decimal_to_binary(int num,char * bin_word, int i);/*returns string with the binary representation of the int it gets*/
bool one_dim_scan(char * arr[],char * requested);/*scan an one dimensional array and returns true if the strings it gets exists in the array*/
char* operations_scan(char * requested,int feature);/*scans the operations table and returns the requated feature of the requested operation*/
char* registers_scan(char * requested,int feature);/*scans the registers table and returns the requated feature of the requested register*/
char * find_addressing(char * operand);/*returns the addressing method of the operand it gets*/
sentence_type find_type(char * word);/*returns the type of the sentence it gets it's command as a parameter*/

#endif
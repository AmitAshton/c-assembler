/*
This file handles all functions and neccessery stuff in oreder to validate the input assembly file,
After this file functions will be called, the user will get all the errors detailed in the standart output,
Also, output files won't be created if there are any errors. 
YOTAM AMIT
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "validation.h"
#include "utility.h"

#define MAX_COMMAND_CHAR 8

char *OPERATIONS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
char *JUMP_OPERATIONS[] = {"jmp", "bne", "jsr"};
char *INSTRUCTIONS[] = {".data", ".string", ".entry", ".extern"};
char *REGISTERS[]  = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

bool error_found;

/*
Main function to handle the validation of the input file
*/
bool validation(char *filename){

    /* openning file for reading */
    FILE *fileptr = fopen(filename, "r");
    /* char array for all words in line withous whitespaces */
    char *words[MAX_CHAR_LINE];
    /* line */
    char line[MAX_CHAR_LINE];
    /* line rows counter */
    int i = 1;
    int k;
    char *token;

    error_found = false;

    /* itinerary over the file */
    while(fgets(line, MAX_CHAR_LINE, fileptr) != NULL){
        
        int j;
        for (j = 0;line[j]=='\n'; i++);
        
        /* max 80 characters per line check */
        if (strlen(line) >MAX_CHAR_LINE-2)
        {
            printf("Error in file: %s. Line number %d : Line is longer than 80 characters.\n", filename, i);
            error_found = true; /* trigger error flag */
        }

        /* checks if the line is blank or a comment */
        if(skippable_line(line))
        {
            i++;
            continue;
        }


        /* cutting line into words */
        k = 0;
        token = strtok(line, " \t\n");
        words[k] = token;
        while (token != NULL)
        {
            token = strtok(NULL, " \t\n");
            words[++k] = token;
        }
        /* checking if first word is a label */
        if (is_label(words, i, filename))
        {
            /* checking if next word is an operation */
            if (is_operation(words, i, filename, 1))
            {
                /* line is valid, continue to next line */
                i++;
                continue;
            }
            else
            {
                /* checking if next word is an instruction */
                if (is_instruction(words, i, filename, 1))
                {
                    /* line is valid, continue to next line */
                    i++;
                    continue;
                }
                else
                {
                    /* line has only label - error throw */
                    printf("Error in file: %s. Line number %d : Line has only valid label. instruction or operation commands are missing or unvalid.\n",filename, i);
                    error_found = true; /*trigger error flag */
                }
            }
        }        
        else
        {
            /* checking if first word is an operation */
            if (is_operation(words, i, filename, 0))
            {
                /* line is valid, continue to next line */
                i++;
                continue;
            }
            else
            {
                /* checking if first word is an instruction */
                if (is_instruction(words, i, filename, 0))
                {
                    /* line is valid, continue to next line */
                    i++;
                    continue;
                }
                else
                {
                    /* line is missing any labels, instructions or operations - error throw */
                    printf("Error in file: %s. Line number %d : Line is not following the assembly rules.\n",filename, i);
                    error_found = true; /* trigger error flag */
                }
            }   
        }

        /* incrementing line counter */
        i++;
    }

    /* closing the file */
    fclose(fileptr);
    
    /* if an error was found, return true and files wont be created, else continue to first pass */
    return error_found;
}

/*
This functions is being called by the is_operation method,
and it checks how many oprands sholud a specific operation get.
*/
int how_many_operands(char word[], int index){
        /* first operations on the operations array are the 2 operands operations */
        if (index < 4)
        {
            return 2;
        }
        if(index == 9 || index == 10 || index == 13)
        {
            return 2;
        }

        /* flag for the jumping operations */
        if (index >= 4 && index < 14)
        {
                return 1; /* 1 operand operation then return 1 */
        }
        if (index >= 14)
        {
            return 0; /* for 0 opernads operations*/
        }
        return 0;
}

/*
This function check if a current word is a valid label,
according to the assembly rules
int i - line counter
*/
bool is_label(char *words[], int i, char *filename){
    int k, j, b;
    char word[MAX_CHAR_LABEL];
    strcpy(word, words[0]);

    for(j=0;word[j]!='\0';j++);
    if (word[j-1]== ':')
    {
        if (j > 31)
        {
            printf("Error in file: %s. Line number %d : Label name is greater than 30 characters.\n",filename, i);
            error_found = true; /* trigger error flag */
            return false;
        }
        
        /* checks if label contains only digits and letters */
        for (k = 0; k < j-1; k++)
        {
            if (!isalpha(word[k]) && !isdigit(word[k]))
            {
                printf("Error in file: %s. Line number %d : Label name includes characters who aren't digits or letters.\n",filename, i);
                error_found = true; /* trigger error flag */
                return false;
            }
        }
        word[strlen(word)-1] = '\0';
        for(j=0;OPERATIONS[j]!=NULL;j++);
        /* checks if label is a saved operation word */
        for (b = 0; b < j; b++)
        {
            if (strcmp(word, OPERATIONS[b]) == 0)
            {
                printf("Error in file: %s. Line number %d : Label name can't be an operation word in assembly.\n",filename, i);
                error_found = true; /* trigger error flag */
                return false;
            }
        }

        for(j=0;REGISTERS[j]!=NULL;j++);
        for (b = 0; b < j; b++)
        {
            if (strcmp(word, REGISTERS[b]) == 0)
            {
                printf("Error in file: %s. Line number %d : Label name can't be a register name in assembly.\n",filename, i);
                error_found = true; /* trigger error flag */
                return false;
            }
            
        }
    }
    else
    {
        return false;
    }
    /* checks if first char is a letter or not */
    if (!isalpha(word[0]))
    {
        return false;
    }
    return true;
}

/*
This function check if the current line is a valid operation line,
according to the assembly rules
int i - line counter
int label_exist - whether this line has a label or not
*/
bool is_operation(char *words[], int i, char *filename, int label_exists){     

    int j, b;
    bool found;
    char op[MAX_COMMAND_CHAR];
    int indexOp;
    int operands_left;
    char first_operand[MAX_CHAR_LABEL]; 
    char second_opernad[MAX_CHAR_LABEL];

    found = false;
    for(j=0;OPERATIONS[j]!=NULL;j++);
    /* checks if word is a saved operation word */
    for (b = 0; b < j; b++)
    {
        if (strcmp(words[0+label_exists], OPERATIONS[b]) == 0)
        {
            strcpy(op, OPERATIONS[b]);
            indexOp = b;
            found = true;
            break;
        }
            
    }

    /* word is not an operation word */
    if(found != true)
    {
        return false;
    }

    operands_left = how_many_operands(op, indexOp);

    
    for(j=0;words[j]!=NULL;j++);
    /* if the operation has no operands and the amount of words is bigger then what needed throw error*/
    if (operands_left == 0 && j != 1+label_exists)
    {
        printf("Error in file: %s. Line number %d : Expected 0 operands.\n",filename, i);
        error_found = true; /*trigger error flag*/
        return false;
    }else{
        return true;
    }

    for(j=0;words[j]!=NULL;j++);
    /* if operation has one operand and the amount of words is bigger then what needed throw error */
    if ((operands_left == 1 && j != 2+label_exists) || (operands_left == 1 && strchr(first_operand, ',') != NULL))
    {
        printf("Error in file: %s. Line number %d : Expected only one operand.\n",filename, i);
        error_found = true; /* trigger error flag */
        return false;
    }else{
        return true;
    }

    /* storing first operand and second operand words */ 
    strcpy(first_operand,words[1+label_exists]);
    for(j=0;words[j]!=NULL;j++);
    strcpy(second_opernad,words[j-1]);

    
    for(j=0;second_opernad[j]!='\0';j++);
    /* if there is a comma before the first operand or after the last operand throw error */
    if (first_operand[0]==',' || second_opernad[j-1]==',')
    {
        printf("Error in file: %s. Line number %d : Comma ',' at unexpected places.\n",filename, i);
        error_found = true; /*trigger error flag*/
        return false;
    }


    if (operands_left == 2)
    {
        return true;
    }
    else
    {
        printf("Error in file: %s. Line number %d : Comma ',' at unexpected places.\n",filename, i);
        error_found = true;  /*trigger error flag  */
        return false;
        
    }
    return true;
}

/*
This function check if the current line is a valid instructions line,
according to the assembly rules
int i - line counter
int label_exist - whether this line has a label or not
*/
bool is_instruction(char *words[], int i, char *filename, int label_exists){

    int b, j, k, y;
    bool found;
    char first_operand[MAX_CHAR_LABEL];
    char last_opernad[MAX_CHAR_LABEL];
    char word[MAX_CHAR_LABEL];
    char data_string[MAX_CHAR_LABEL];
    int counter_nums;
    int counter_comma;

    found = false;
    for(j=0;INSTRUCTIONS[j]!=NULL;j++);
    /* checks if word is a saved instruction word */
    for (b = 0; b < j; b++)
    {
        if (strcmp(words[0+label_exists], INSTRUCTIONS[b]) == 0)
        {
            found = true;
            break;
        }
    }

    if(!found)
    {
        return false;
    }
    /* storing first and last operands */
    strcpy(first_operand, words[1+label_exists]);
    for(j=0;words[j]!=NULL;j++);
    strcpy(last_opernad, words[j - 1]);
    for(j=0;last_opernad[j]!='\0';j++);

    /* if there is a comma before first operand or after last operand throw error */
    if (first_operand[0]== ','||last_opernad[j - 1]==',')
    {
        printf("Error in file: %s. Line number %d : Comma ',' at unexpected places.\n",filename, i);
        error_found = true; /* trigger error flag */
        return false;
    }

    /* if instruction is .string cehck quotes validation */
    if (strcmp(INSTRUCTIONS[b], ".string") == 0)
    {
        strcpy(first_operand, words[1+label_exists]);
        for(j=0;last_opernad[j]!='\0';j++);
        if (first_operand[0]!='"' && last_opernad[j - 1]!='"')
        {
            printf("Error in file: %s. Line number %d : Expected one string in qoutes.\n",filename, i);
            error_found = true; /* trigger error flag */
            return false;
        }
    }

    for(j=0;words[j]!=NULL;j++);
    /* if instruction is .entryor .entry check amount of words(only one parameter) */
    if (strcmp(INSTRUCTIONS[b], ".entry") == 0 || strcmp(INSTRUCTIONS[b], ".extern") == 0)
    {
        if (j != 2+label_exists)
        {
            printf("Error in file: %s. Line number %d : Expected one label name.\n",filename, i);
            error_found = true; /* trigger error flag */
            return false;
        }
        strcpy(word, words[1+label_exists]);
        for(j=0;OPERATIONS[j]!=NULL;j++);
        /* checks if label is a saved operation word */
        for (y = 0; y < j; y++)
        {
            if (strcmp(word, OPERATIONS[y]) == 0)
            {
                printf("Error in file: %s. Line number %d : Label name can't be an operation word in assembly.\n",filename, i);
                error_found = true; /* trigger error flag */
                return false;
            }
        }

        for(j=0;INSTRUCTIONS[j]!=NULL;j++);
        for (y = 0; y < j; y++)
        {
            if (strcmp(word, INSTRUCTIONS[y]) == 0)
            {
                printf("Error in file: %s. Line number %d : Label name can't be an instruction word in assembly.\n",filename, i);
                error_found = true; /* trigger error flag */
                return false;
            }
        }

        for(j=0;REGISTERS[j]!=NULL;j++);
        for (y = 0; y < j; y++)
        {
            if (strcmp(word, REGISTERS[y]) == 0)
            {
                printf("Error in file: %s. Line number %d : Label name can't be a register name in assembly.\n",filename, i);
                error_found = true; /* trigger error flag */
                return false;
            }
            
        }
    }

    /* 
    if instruction is .data check commas validation:
    counter_nums need to be always larger by maximum 1 or equal to counter_comma
    if not there is an error in syntax
    */
    if (strcmp(INSTRUCTIONS[b], ".data") == 0)
    {
        counter_nums = 0;
        counter_comma = 0;

        for (k = 1+label_exists; words[k] != NULL; k++)
        {
           strcpy(data_string, words[k]);
            for (j = 0; data_string[j] != '\0'; j++)
            {
                if (data_string[j] == '+' || data_string[j] == '-')
                {                   
                    continue;
                }
                if (isdigit(data_string[j]))
                {
                    counter_nums++;
                    for (; data_string[j] != '\0'; j++)
                    {
                        if (isdigit(data_string[j]))
                        {
                            continue;
                        }else{
                            break;
                        }
                    }
                }
                
                if (data_string[j] == ',')
                {
                    counter_comma++;
                }
                if (counter_nums < counter_comma || counter_nums - counter_comma > 1){
                    printf("Error in file: %s. Line number %d : There should be exactly one comma after every number except the last number and before the first one.\n",filename, i);
                    error_found = true;  /* trigger error flag */
                    return false;
                }
            }
        }
    }

    return true;
}

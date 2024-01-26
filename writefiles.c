/*
This file handles all functions and neccessery stuff in oreder to procces the second pass and create the output files,
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "validation.h"
#include "utility.h"
#include "writefiles.h"
#include "wordtable.h"
#include "labeltable.h"

/*
This function creats the .ob file and writes into it the IC, DC,
and for every word an addres in base 10 and a speacial binary word
*/
void write_ob_file(word_table w_tab, int final_ic, int final_dc, char *filename){

    FILE *obfp;
    int i;

    /* creating and opening file */
    obfp = fopen(strcat(filename, ".ob"), "w");
    
    /* printing in the first line the IC, DC */
    fprintf(obfp, "%d  %d\n", final_ic, final_dc);
    
    /* for each word in memory, print his addres in base 10 */
    for (; w_tab != NULL; w_tab = w_tab->next){
        fprintf(obfp, "%d  ", w_tab->address);
            
        /* for each bit in the binary base of the word, replace 0 by . and 1 by / */
        for (i=0; i < WORD_SIZE; i++)
        {
            if (strcmp(w_tab->bin_word[i], "0"))
            {
                fprintf(obfp, ".");
            }
            
            if (strcmp(w_tab->bin_word[i], "1"))
            {
                fprintf(obfp, "/");
            }
        }
        /* moving to next word in memory, a new line in file*/
        fprintf(obfp, "\n");
    }

    fclose(obfp);
}

/*
This function creats the .ext and .ent file if there are any .entry and .extern in file,
and writes into it the label name, and the word addres in base 10
*/
void write_ext_ent_files(word_table w_tab, label_table l_tab, char *filename){

    bool externFound = false;
    bool entryFound = false;
    FILE *extfp;
    FILE *entfp;

    /* checking if there are any extern or entry symbols in the label table, if there are'nt it's not creating anything */
    for (; l_tab!= NULL; l_tab = l_tab->next)
    {
        if (l_tab->type == EXTERNAL_SYMBOL)
        {
            externFound = true;
        }
        if (l_tab->type == ENTRY_SYMBOL)
        {
            entryFound = true;
        }
    }

    /* creating and opening .ext file if there are any .extern symbols in file*/
    if (externFound)
    {
        
        extfp = fopen(strcat(filename, ".ext"), "w");

        for (; l_tab != NULL; l_tab = l_tab->next)
        {
            if (l_tab->type == EXTERNAL_SYMBOL)
            {
                for (; w_tab != NULL; w_tab = w_tab->next)
                {
                    if (strcmp(l_tab->name, w_tab->string) == 0)
                    {
                        /* printing for each label that ha a type of .extern symbol, his name and his addres*/
                        fprintf(extfp, "%s  %d\n", l_tab->name, l_tab->value);
                    }
                    
                }
                
            }
            
        }

        fclose(extfp);
    }

    /* creating and opening .ent file if there are any .entry symbols in file*/
    if (entryFound)
    {
        entfp = fopen(strcat(filename, ".ent"), "w");

        /* printing for each label that has a type of .entry symbol, his name and his addres*/
        for (; l_tab != NULL; l_tab = l_tab->next)
        {
            if (l_tab->type == ENTRY_SYMBOL)
            {
                fprintf(entfp, "%s  %d\n", l_tab->name, l_tab->value);
            }
        }

        fclose(entfp);
    }
    
}
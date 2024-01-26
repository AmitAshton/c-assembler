#include "utility.h"
#include "labeltable.h"
#include "wordtable.h"
#include "validation.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void write_ob_file(word_table w_tab, int final_ic, int final_dc, char *filename);
void write_ext_ent_files(word_table w_tab, label_table l_tab, char *filename);

#include "utility.h"


#include <string.h>
#include <stdbool.h>
#include <stdio.h>

char * operations [][OPERATIONS_ROW] =
{{"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"},
{"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"},
{"2","2","2","2","1","1","2","1","1","1","1","1","1","1","0","0"}};

char * registers[][REGISTERS_ROW] =
{{"r0","r1","r2","r3","r4","r5","r6","r7"},
{"000000","000001","000010","000011","000100","000101","000110","000111"}};

char * datawords[] =
{".data",".string"};

char * authorizations[] =
{".entry",".extern"};

bool skippable_line (char* str)
{
GET_TO_FIRST_LETTER(str)
if(*str=='\0'||*str==';')
return true;

return false;
}

bool one_dim_scan(char * arr[],char * requested)
{
int i =0;

while(i<ONE_DIM_ARRAY_SIZE)
{
if (strcmp(arr[i],requested)==0) return true;
i++;
}

return false;
}

char* registers_scan(char * requested,int feature)
{
int j;

for(j=0;j<REGISTERS_ROW;j++)
if ((strcmp(registers[NAME_FEATURE][j],requested))==0) return registers[feature][j];

return "";
}

char* operations_scan(char * requested,int feature)

{
int j;

for(j=0;j<OPERATIONS_ROW;j++)
if ((strcmp(operations[NAME_FEATURE][j],requested)==0)) return operations[feature][j];

return "";
}

char * decimal_to_binary(int num,char * bin_word,int i)
{
int clone = num;
bool toflip;

if(num<0) num*=-1;

strcpy(bin_word,ZERO_WORD);
 
    if (num == 0) return bin_word;
     
for ( ;num > 0; )
{
        bin_word[i--] = num % 2 + '0';
        num /= 2;
}

if(clone<0)
for(i=13,toflip = false;i>=0;i--)
{
if(bin_word[i]=='1'&&!toflip)
{
toflip = true;
continue;
}
if(toflip) (bin_word[i]=(bin_word[i]=='0')? '1' : '0');
}
return bin_word;
}


char * find_addressing(char * operand)
{
if(*operand=='#') return "00";

if(strlen(registers_scan(operand,NAME_FEATURE))!=0) return "11";

if(strchr(operand,'(')!=NULL) return "10";

return "01";
}

sentence_type find_type(char * word)
{

if(one_dim_scan(datawords,word)) return TYPE_DATA;

if(one_dim_scan(authorizations,word)) return TYPE_AUTHORIZATION;

if(strlen(operations_scan(word,NAME_FEATURE))!=0) return TYPE_OPERATION;

return TYPE_LABEL;

}


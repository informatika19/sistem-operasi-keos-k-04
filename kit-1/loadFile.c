#include <stdio.h>
#include "kernel.c"

void returnStringFile(char *path, char*buffer)
{
    FILE *f; 
    
    char str[502];
    f = fopen(path,"r"); 
    while(fgets(str,502,f)!=NULL){
        strcat1(buffer,str);
        
    }
    fclose(f);
    // printf("%s",buffer);
}

// menggabungkan array 2 ke array 1 sampai null character pada array 2
void strcat1(char *str1, char *str2) {
  int i = 0;
  while (*str1 != '\0') { str1++; }
  while (str2[i] != '\0') {
    str1[i++] = str2[i];
  }
  str1[i] = '\0';
}

void loadFile(){
    char path[20];
    scanf("%s",&path);
    
    char *ppath = "test.txt";
    char buffer[502];
    returnStringFile(ppath, buffer);
    //printf("%s",buffer);
    
    return 0;
}


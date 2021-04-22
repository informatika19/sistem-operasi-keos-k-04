#ifndef __TEXT_H__
#define __TEXT_H__

void printNum(int c);
void printString(char *str);
void printStringLength(char *str, int length);
void printStringToNumLength(char *str, int length);
void readString(char *str);
void printFile(char* buff, int file_idx);
void printIsiFile(char* buffer, int file_idx);
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
int strcmp(char *str1, char *str2);
void strcat(char *str1, char *str2);
int strlen(char *str);
int copy_arr(char *arr, char *str);
void copy_arr_length(char *arr, char *str, int length);
void asciiPrint();
#endif
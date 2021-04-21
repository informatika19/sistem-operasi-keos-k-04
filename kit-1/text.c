// membaca input, masukkan pada string
void readString(char *str) {
  char *cc = str-1;
  do {
    cc++;
    *cc = interrupt(0x16, 0, 0, 0, 0);  // input dimasukkan ke dalam variabel string lewat pointer
    interrupt(0x10, 0x0e00+*cc,0,0,0);
  } while(*cc != 13 && *cc != '\n');  // input bukan enter
  *cc = '\0'; // masukkan NULL character ke akhir string
}

void printString(char *str) {
  int ah = 0x0e00; // 0Eh: write string pada interrupt 0x10, code Ox0e dikali 16*16 
                       // untuk setup AH
  int i = 0; //untuk iterate, berguna untuk offset memori
  while (*(str+i) != '\0'){ // while hingga akhir string
    int al = *(str+i); 
    if (*(str+i) == '\n'){
      interrupt(0x10,ah+13,0,0,0); // 13 = ASCII untuk CR / enter
    }
    interrupt(0x10,ah+al,0,0,0); // Call interrupt
    i++; // iterate
  }
}

// mencetak string sampai panjang length
void printStringLength(char *str, int length){
  int ah = 0x0e00; // 0Eh: write string pada interrupt 0x10, code Ox0e dikali 16*16 
                       // untuk setup AH
  int i = 0; //untuk iterate, berguna untuk offset memori
  do {
    int al = *(str+i); 
    if (*(str+i) == '\n'){
      interrupt(0x10,ah+13,0,0,0); // 13 = ASCII untuk CR / enter
    }
    interrupt(0x10,ah+al,0,0,0); // Call interrupt
    i++; // iterate
  } while (i < length);  // while hingga length
  
}

// mencetak string sampai panjang length dalam bentuk hexa
void printStringToNumLength(char *str, int length) { 
  int i = 0; //untuk iterate, berguna untuk offset memori
  do {
    printNum(str[i]);
    printString("|");
    i++; // iterate
  } while (i < length); // while hingga length
}

void printFile(char* buff, int file_idx) {
  // print parent file
  printNum(buff[file_idx]);
  // print index pada sectors
  printNum(buff[file_idx+1]);
  // print nama file
  printStringLength(&buff[file_idx+2], 16);
}

void printIsiFile(char* buff, int file_idx) {
  printStringLength(&buff[file_idx], 16);
}

// mencetak angka
void printNum(int c) {
  char cc = div(c, 16);
  if (cc >= 0 && cc<= 9)
    cc += '0';
  else if (cc >= 0xA && cc <= 0xF)
    cc += -0xA+'A';
  printStringLength(&cc, 1);

  cc = mod(c, 16);
  if (cc >= 0 && cc<= 9)
    cc += '0';
  else if (cc >= 0xA && cc <= 0xF)
    cc += -0xA+'A';
  printStringLength(&cc, 1);
}

void clear(char *buffer, int length){
  int i = 0;
  while(i < length){
    buffer[i++]='\0';
  }
}

// membandingkan dua array sampai jika ada null character pada array 1 atau array 2
int strcmp (char *str1, char *str2) {
  int i = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i++] != str2[i]) {return 0;};
  }
  return 1;
}


// membandingkan dua array sampai jika ada null character pada array 1 atau array 2. Tambah syarat panjang string
int strcmpfitlen (char *str1, char *str2) {
  int i = 0;
  if (strlen(str1) == strlen(str2)){
    while (str1[i] != '\0' && str2[i] != '\0') {
      if (str1[i++] != str2[i]) {return 0;};
    }
    return 1;
  }
  else {return 0;}
}

// menggabungkan array 2 ke array 1 sampai null character pada array 2
void strcat(char *str1, char *str2) {
  int i = 0;
  while (*str1 != '\0') { str1++; }
  while (str2[i] != '\0') {
    str1[i++] = str2[i];
  }
  str1[i] = '\0';
}

// menghitung panjang array
int strlen(char *str) {
  int i = 0;
  while (str[i] != '\0') {
    i++;
  }
  return i;
}


// void asciiPrint() {
  // printString("╔╗────╔═══╦═══╗\n");
  // printString("║║────║╔═╗║╔═╗║\n");
  // printString("║║╔╦══╣║─║║╚══╗\n");
  // printString("║╚╝╣║═╣║─║╠══╗║\n");
  // printString("║╔╗╣║═╣╚═╝║╚═╝║\n");
  // printString("╚╝╚╩══╩═══╩═══╝\n");
  // printString("|OOOOOOOO|\n");
  // printString("|OOOOOOOO|\n");
  // printString("|OOOOOOOO|\n");
  // printString("|OOOOOOOO|\n");
// }


// copy array 2 ke array 1 sampai null character pada array 2
int copy_arr(char *arr, char *str) {
  int i=0;
  while (str[i] != '\0')
    arr[i++] = str[i];
  return i;
}

// copy array 2 ke array 1 sebanyak length
void copy_arr_length(char *arr, char *str, int length) {
  int i=0;
  do {
    arr[i++] = str[i];
  } while (i < length);
}

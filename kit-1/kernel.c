// /* Ini deklarasi fungsi */
void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0

int main() {
  //Test
  char Test[3];
  Test[0] = 'T';
  Test[1] = 'H';
  Test[2] = 'E';
  printString(&Test);
  
  makeInterrupt21();
  while (1);
}

void printString(char *string){
  int ah = 0x0e*16*16; // 0Eh: write string pada interrupt 0x10, code Ox0e dikali 16*16 
                       // untuk setup AH
  int i = 0; //untuk iterate, berguna untuk offset memori
  while (*(string+i) != '\0'){ // while hingga akhir string
    int al = *(string+i); 
    interrupt(0x10,ah+al,0,0,0); // Call interrupt
    i++; // iterate
  }
  interrupt(0x10,ah+13,0,0,0); //+13 untuk membaca 'enter'.
}

void readString(char* string){
  int ah= 0*16*16; // AH di set 00 untuk read character.
  int i = 0;
  int al;
  while(*(string+i) != 13){ //Input bukan enter
    al = *(string+i);
    interrupt(0x16, ah+al, 0, 0, 0);
    i++; //iterate
  }
  interrupt(0x16, ah+al, 0, 0, 0);
}

void handleInterrupt21 (int AX, int BX, int CX, int DX){
  switch (AX) {
    case 0x0:
      printString(BX);
      break;
    case 0x1:
      readString(BX);
      break;
    default:
      printString("Invalid interrupt");
  }
}


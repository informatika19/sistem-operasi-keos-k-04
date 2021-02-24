// /* Ini deklarasi fungsi */
void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
void asciiPrint();

int main() {
  char Test[30];
  makeInterrupt21();

  //asciiPrint();
  printString("Your name:");
  readString(&Test);

  printString("\n");
  printString("Selamat datang, ");
  printString(&Test);
  
  while (1);
}

void printString(char *string){
  int ah = 0x0e*16*16; // 0Eh: write string pada interrupt 0x10, code Ox0e dikali 16*16 
                       // untuk setup AH
  int i = 0; //untuk iterate, berguna untuk offset memori
  while (*(string+i) != '\0'){ // while hingga akhir string
    int al = *(string+i); 
    if (*(string+i) == '\n'){
      interrupt(0x10,ah+13,0,0,0); // 13 = ASCII untuk CR / enter
    }
    interrupt(0x10,ah+al,0,0,0); // Call interrupt
    i++; // iterate
  }
}

void readString(char *string) {
  char *cc = string;
  while(*(cc-1) != 13 && *(cc-1) != '\n') {  // input bukan enter
    *cc = interrupt(0x16, 0, 0, 0, 0);  // input dimasukkan ke dalam variabel string lewat pointer
    cc++;
  }
  *(cc-1) = '\0'; // masukkan NULL character ke akhir string
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

// void asciiPrint() {
//   printString("╔╗────╔═══╦═══╗\n");
//   printString("║║────║╔═╗║╔═╗║\n");
//   printString("║║╔╦══╣║─║║╚══╗\n");
//   printString("║╚╝╣║═╣║─║╠══╗║\n");
//   printString("║╔╗╣║═╣╚═╝║╚═╝║\n");
//   printString("╚╝╚╩══╩═══╩═══╝\n");
// }


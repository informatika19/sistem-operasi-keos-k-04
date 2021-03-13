// /* Ini deklarasi fungsi */
void handleInterrupt21 (int AX, int BX, int CX, int DX);
int initialize_array(char *arr, char *string);
void initialize_array_length(char *arr, char *string, int length);
void printNum(int c);
void printString(char *string);
void printStringLength(char *string, int length);
void printStringToNumLength(char *string, int length);
void readString(char *string);
void writeString();
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
void asciiPrint();

int div(int x, int y);
int mod(int x, int m);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
// void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void writeFile(char *buffer, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void printFile(char* buffer, int file_idx);
void printIsiFile(char* buffer, int file_idx);

int main() {
  char Test[512];
  char read[512];
  Test[initialize_array(Test, "QWERTYUIOP")] = '\0';
  
  // interrupt(0x10, 0x00*16*16+0x01, 0, 0, 0);
  // interrupt(0x10, 0x00*16*16+0x01, 0*16*16+0x01, 0, 0);

  makeInterrupt21();

  // asciiPrint();
  printString("Your name:");
  readString(Test);

  printString("\n");
  printString("Selamat datang, ");
  printString(Test);
  printString("\n");
  clear(read, 512);
  readSector(read, 11);

  printString("---------------------\n");
  // interrupt(0x13, 0x02*16*16+0x01, 0x1000, 0x0*16*16+0x02, 0x0*16*16+0x0);
  writeSector(Test, 11);
  clear(read, 100);
  readSector(read, 11);
  printStringLength(read, 20);
  printString("\n");
  printString("Finished Test\n");

  clear(read, 16);
  initialize_array(read, "Isi File");
  writeFile(read, 0x01);

  printString("Sector 100\n");
  clear(read, 100);
  readSector(read, 10);
  printStringToNumLength(read, 100);
  printString("\n");

  printString("Sector 101\n");
  clear(read, 512);
  readSector(read, 11);
  printStringLength(read, 50);
  printString("\n");
  printFile(read, 0);
  printString("\n");

  printString("Sector 102\n");
  clear(read, 512);
  readSector(read, 12);
  printStringLength(read, 30);
  printString("\n");
  printFile(read, 0);
  printString("\n");

  printString("Sector 103\n");
  clear(read, 512);
  readSector(read, 13);
  printStringLength(read, 50);
  printString("\n");
  printIsiFile(read, 0*16);
  printString("\n");

  // while (1) {
  //   // read sector
  //   readString(read);
  //   printString("\n", 0, 100, 1);
  //   readSector(read, ((int) (read[0] - '0'))+100);
  //   printString(read, 0, 100, 1);
  //   printString("\n", 0, 100, 1);
  // }
  
  clear(read, 512);
  writeSector(read, 10);
  clear(read, 512);
  writeSector(read, 11);
  clear(read, 512);
  writeSector(read, 12);
  clear(read, 512);
  writeSector(read, 13);


  while (1);
}

void printString(char *string) {
  int ah = 0x0e*16*16; // 0Eh: write string pada interrupt 0x10, code Ox0e dikali 16*16 
                       // untuk setup AH
  int i = 0; //untuk iterate, berguna untuk offset memori
  while (*(string+i) != '\0'){ // while hingga akhir string
  // while (1) {
    int al = *(string+i); 
    if (*(string+i) == '\n'){
      interrupt(0x10,ah+13,0,0,0); // 13 = ASCII untuk CR / enter
    }
    interrupt(0x10,ah+al,0,0,0); // Call interrupt
    i++; // iterate
  }
}

void printStringLength(char *string, int length){
  int ah = 0x0e*16*16; // 0Eh: write string pada interrupt 0x10, code Ox0e dikali 16*16 
                       // untuk setup AH
  int i = 0; //untuk iterate, berguna untuk offset memori
  do {
    int al = *(string+i); 
    if (*(string+i) == '\n'){
      interrupt(0x10,ah+13,0,0,0); // 13 = ASCII untuk CR / enter
    }
    interrupt(0x10,ah+al,0,0,0); // Call interrupt
    i++; // iterate
  } while (i < length);  // while hingga length
  
}

void printStringToNumLength(char *string, int length) { 
  int i = 0; //untuk iterate, berguna untuk offset memori
  do {
    printNum(string[i]);
    printString("|");
    i++; // iterate
  } while (i < length); // while hingga length
}

void printFile(char* buffer, int file_idx) {
  int al = buffer[file_idx];
  printNum(al);
  al = buffer[file_idx+1];
  printNum(al);
  printStringLength(&buffer[file_idx+2], 16);
}

void printIsiFile(char* buffer, int file_idx) {
  printStringLength(&buffer[file_idx], 16);
}

void readString(char *string) {
  char *cc = string;
  cc--;
  do {
    cc++;
    *cc = interrupt(0x16, 0, 0, 0, 0);  // input dimasukkan ke dalam variabel string lewat pointer
    // if (*cc == 8) {
    //   *cc = ' ';
    //   interrupt(0x10, 0x0e*16*16+*cc,0,0,0);
    //   interrupt(0x10, 0x0e*16*16+8,0,0,0);
    //   interrupt(0x10, 0x0e*16*16+8,0,0,0);
    //   cc-=2;
    //   continue;
    // }
    interrupt(0x10, 0x0e00+*cc,0,0,0);
  } while(*(cc) != 13 && *(cc) != '\n');  // input bukan enter
  *cc = '\0'; // masukkan NULL character ke akhir string
}

void writeString() {
  char cc;
  while (cc != 13 || cc != '\n') {
    cc = interrupt(0x16, 0, 0, 0, 0);
    interrupt(0x10,0x0e00+cc,0,0,0);
  }
}

void clear(char *buffer, int length){
  int i = 0;
  while(i < length){
    buffer[i]='\0';
    i++;
  }
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

int initialize_array(char *arr, char *string) {
  int i=0;
  while (string[i] != '\0') {
    arr[i] = string[i];
    i++;
  }
  return i;
}

void initialize_array_length(char *arr, char *string, int length) {
  int i=0;
  do {
    arr[i] = string[i];
    i++;
  } while (i < length);
}

void readSector(char *buffer, int sector) {
  /*
  ah: 02h
  al: sectors to read count
  ch: cylinder
  cl: sector
  dh: head
  dl: drive
  */
  int cylinder;
  int head;
  int drive;
  cylinder = div(sector, 36);
  sector = mod(sector, 18)+1;
  head = mod(div(sector, 18), 2);
  drive = 0;

  // cylinder = 0;
  // // sector = mod(sector, 18)+1;
  // head = 0;
  // drive = 0;

  interrupt(0x13, 0x201, buffer, (cylinder << 2) + sector, (head << 2) + drive);
  // interrupt(0x13, 0x02*16*16+0x01, 0x1000, 0x0*16*16+0x02, 0x0*16*16+0x0);
}

void writeSector(char *buffer, int sector) {
  /*
  ah: 03h
  al: sectors to write count
  ch: track
  cl: sector
  dh: head
  dl: drive
  */
  int cylinder;
  int head;
  int drive;
  cylinder = div(sector, 36);
  sector = mod(sector, 18)+1;
  head = mod(div(sector, 18), 2);
  drive = 0;

  // cylinder = 0;
  // // sector = mod(sector, 18)+1;
  // head = 0;
  // drive = 0;
  interrupt(0x13, 0x0301, buffer, (cylinder << 2) + sector, (head << 2) + drive);
}

// void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
void writeFile(char *buffer, char parentIndex) {

  char sector_map[512];
  char sector_file_1[512];
  char sector_file_2[512];
  char sector_sectors[512];
  char file_name[15];
  int idx = 0;
  int isAdaYgKosong = 0;
  int isAdaYgKosongDiFiles = 0;
  int isAdaYgKosongDiSectors = 0;
  int isFile = 1;
  int isParentRoot = 0;
  int idx_file = 0;

  int loop = 0;
  parentIndex = 0x1A;
  
  clear(sector_map, 512);
  readSector(sector_map, 10);

  clear(sector_file_1, 512);
  readSector(sector_file_1, 11);

  clear(sector_file_2, 512);
  readSector(sector_file_2, 12);

  clear(sector_sectors, 512);
  readSector(sector_sectors, 13);

  // clear(buffer, 16);
  // initialize_array(buffer, "TestIsiFile");
  clear(file_name, 15);
  file_name[initialize_array(file_name, "FileName")] = '\0';

  // tanda ada idx files kosong atau terisi di map: idx = 0 -> buff[10+idx=0]
  isAdaYgKosongDiFiles = 0;
  while (isAdaYgKosongDiFiles == 0 && idx < 64) {
    if (sector_map[11+idx] != 0xFF) {
      isAdaYgKosongDiFiles = 1;
    } else {
      idx++;
    }
  }

  // tanda ada idx sector kosong atau terisi di map: idx = 0 -> buff[10+idx=0]
  isAdaYgKosongDiSectors = 0;
  while(isAdaYgKosongDiSectors == 0 && idx_file < 32) {
    if (sector_map[75+idx_file] != 0xFF) {
      isAdaYgKosongDiSectors = 1;
    } else {
      idx_file++;
    }
  }
  if (isAdaYgKosongDiFiles == 1 && isAdaYgKosongDiSectors == 1) {
    isAdaYgKosong = 1;
  }
  printString("idx: ");
  printNum(idx);
  printString(" ");
  printString("idx_file: ");
  printNum(idx_file);
  printString("\n");
  printStringToNumLength(&sector_map[10], 10);
  printString("\n");
  printStringToNumLength(&sector_map[75], 10);
  printString("\n");
  
  if (isAdaYgKosong == 1) {
    if (idx < 32) {
      clear(&sector_file_1[idx*16], 16);
      if (isParentRoot) {
        sector_file_1[idx*16] = 0xFF;
      } else {
        sector_file_1[idx*16] = parentIndex;
      }
      if (isFile == 0) {
        initialize_array(&sector_file_1[idx*16+2], "Nama Folder");  // max 14 character
        sector_map[11+idx] = 0xFF;
      } else {
        initialize_array(&sector_file_1[idx*16+2], file_name);  // max 14 character
        sector_map[11+idx] = 0xFF;

        initialize_array_length(&sector_sectors[idx_file*16], buffer, 16);
        sector_map[75+idx_file] = 0xFF;
      }
    } else {
      clear(&sector_file_2[idx*16-32*16], 16);
      if (isParentRoot) {
        sector_file_2[idx*16] = 0xFF;
      } else {
        sector_file_2[idx*16] = parentIndex;
      }
      if (isFile == 0) {
        initialize_array(&sector_file_2[idx*16-32*16+2], "Nama Folder");  // max 14 character
        sector_map[11+idx] = 0xFF;
      } else {
        initialize_array(&sector_file_2[idx*16-32*16+2], file_name);  // max 14 character
        sector_map[11+idx] = 0xFF;
        initialize_array_length(&sector_sectors[idx_file*16], buffer, 16);
        sector_map[75+idx_file] = 0xFF;
      }
    }
  }

  printString("Sector Map\n");
  printStringToNumLength(sector_map, 80);
  printString("\n");

  printString("Sector File\n");
  printFile(sector_file_1, idx*16);
  printString("\n");

  printString("Sector Sectors\n");
  printIsiFile(sector_sectors, idx_file*16);

  printString("\n");

  writeSector(sector_map, 100);
  writeSector(sector_file_1, 101);
  writeSector(sector_file_2, 102);
  writeSector(sector_sectors, 103);

}

// void readFile(char *buffer, char *path, int *result, char parentIndex) {
void readFile(char *buffer, char *path, int *result, char parentIndex) {

  char sector_map[512];
  char sector_file_1[512];
  char sector_file_2[512];
  char sector_sectors[512];
  char file_name[15];
  int idx = 0;

  clear(sector_map, 512);
  readSector(sector_map, 10);

  clear(sector_file_1, 512);
  readSector(sector_file_1, 11);

  clear(sector_file_2, 512);
  readSector(sector_file_2, 12);

  clear(sector_sectors, 512);
  readSector(sector_sectors, 13);

  // while (idx < 64) {
  //   if (sector_map[11+idx] != 0xFF) {
  //     isAdaYgKosongDiFiles = 1;
  //   } else {
  //     idx++;
  //   }
  // }
  // sector_file_1[idx*16+2]
}

int div(int x, int y) {
  return x/y;
}

int mod(int x, int m) {
  while (x >= m) { x -= m; }
  return x;
}

void printNum(int c) {
  char cc = div(c, 16);
  if (div(c, 16) >= 0 && div(c, 16) <= 9) {
    cc = cc+'0';
  } else if (div(c, 16) >= 0xA && div(c, 16) <= 0xF) {
    cc = cc-0xA+'A';
  }
  printStringLength(&cc, 1);

  cc = mod(c, 16);
  if (mod(c, 16) >= 0 && mod(c, 16) <= 9) {
    cc = cc+'0';
  } else if (mod(c, 16) >= 0xA && mod(c, 16) <= 0xF) {
    cc = cc-0xA+'A';
  }
  printStringLength(&cc, 1);
}

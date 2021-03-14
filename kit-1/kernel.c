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
void makeFile(char *buffer, char *path);
// void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void writeFile(char *buffer, char *path, char parentIndex);
// void writeFile(char *buffer, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void printFile(char* buffer, int file_idx);
void printIsiFile(char* buffer, int file_idx);
char searchParentIndex(char *path, char parentIndex);
void listAll(char *path);

int strcmp (char *str1, char *str2);

char sector_map[512];
char sector_file_1[512];
char sector_file_2[512];
char sector_sectors[512];

int main() {
  char read[512];
  char current_path[200];
  int idx_curr_path = 1;
  char parentIndex = 0xFF;

  initialize_array(current_path, "/\0");

  // clear(read, 512);
  writeSector(read, 10);
  writeSector(read, 11);
  writeSector(read, 12);
  writeSector(read, 13);

  // clear(sector_map, 512);
  // clear(sector_file_1, 512);
  // clear(sector_file_2, 512);
  // clear(sector_sectors, 512);
  readSector(sector_map, 10);
  readSector(sector_file_1, 11);
  readSector(sector_file_2, 12);
  readSector(sector_sectors, 13);

  // interrupt(0x10, 0x00*16*16+0x01, 0, 0, 0);
  // interrupt(0x10, 0x00*16*16+0x01, 0*16*16+0x01, 0, 0);

  // makeInterrupt21();

  // asciiPrint();
  // printString("Your name:");
  // readString(read);
  // printString("\n");
  // printString("Selamat datang, ");
  // printString(read);
  // printString("\n");

  printString("---------------------\n");
  // interrupt(0x13, 0x02*16*16+0x01, 0x1000, 0x0*16*16+0x02, 0x0*16*16+0x0);
  // clear(read, 100);
  // readSector(read, 11);
  // printStringLength(read, 20);
  // printString("\n");
  printString("Finished Test\n");

  clear(read, 16);
  initialize_array(read, "Isi File");
  makeFile(read, "parent1/parent2/parent3/anak.txt");
  makeFile(read, "parent1/parent2/parent3/anak2.txt");
  makeFile(read, "parent_2/parent2/anak4.txt");

  // printString("Sector 100\n");
  // clear(read, 100);
  // readSector(read, 10);
  // printStringToNumLength(read, 100);
  // printString("\n");

  // printString("Sector 101\n");
  clear(read, 512);
  readSector(read, 11);
  // printStringLength(read, 50);
  // printString("\n");
  printFile(read, 0);
  printString("\n");
  printFile(read, 0x10);
  printString("\n");
  printFile(read, 0x20);
  printString("\n");
  printFile(read, 0x30);
  printString("\n");
  // printString("Sector 102\n");
  // clear(read, 512);
  // readSector(read, 12);
  // printStringLength(read, 30);
  // printString("\n");
  // printFile(read, 0);
  // printString("\n");

  // printString("Sector 103\n");
  clear(read, 512);
  readSector(read, 13);
  // printStringLength(read, 50);
  // printString("\n");
  printIsiFile(read, 0*16);
  printString("\n");
  printIsiFile(read, 1*16);
  printString("\n");
  printIsiFile(read, 2*16);
  printString("\n");
  printIsiFile(read, 3*16);
  printString("\n");

  // while (1) {
  //   // read sector
  //   readString(read);
  //   printString("\n", 0, 100, 1);
  //   readSector(read, ((int) (read[0] - '0'))+100);
  //   printString(read, 0, 100, 1);
  //   printString("\n", 0, 100, 1);
  // }

  // terminal
  printString("\n\n");
  while (strcmp(read, "exit") == 0) {
    printString("root:");
    printString(current_path);
    printString("$ ");

    readString(read);
    printString("\n");
    if (strcmp(read, "ls")) {
      printNum(parentIndex);
      printString("\n");
      listAll(parentIndex);

    } else if (strcmp(read, "cd ")) {
      if (read[2] != '\0' || read[3] != '\0') {
        if (read[3] == '/') {
          // path dari root
          parentIndex = searchParentIndex(&read[4], 0xFF);
          idx_curr_path = initialize_array(current_path, &read[3]);
        } else {
          // path relatif
          if (parentIndex != 0xFF) {
            current_path[idx_curr_path] = '/';
            idx_curr_path++;
          }
          parentIndex = searchParentIndex(&read[3], parentIndex);
          idx_curr_path = initialize_array(&current_path[idx_curr_path], &read[3]);

        }
      }

    } else if (strcmp(read, "cat ")) {

    } else {
      printString("Command not found\n");
    }
    printString("\n");
  }

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
  // int cylinder;
  // int head;
  // int drive;
  // cylinder = div(sector, 36);
  // sector = mod(sector, 18)+1;
  // head = mod(div(sector, 18), 2);
  // drive = 0;

  // cylinder = 0;
  // // sector = mod(sector, 18)+1;
  // head = 0;
  // drive = 0;
  interrupt(0x13, 0x201, buffer, (div(sector, 36) << 2) + (mod(sector, 18)+1), (mod(div(sector, 18), 2) << 2) + 0);
  // interrupt(0x13, 0x201, buffer, (cylinder << 2) + sector, (head << 2) + drive);
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
  // int cylinder;
  // int head;
  // int drive;
  // cylinder = div(sector, 36);
  // sector = mod(sector, 18)+1;
  // head = mod(div(sector, 18), 2);
  // drive = 0;

  // cylinder = 0;
  // // sector = mod(sector, 18)+1;
  // head = 0;
  // drive = 0;
  interrupt(0x13, 0x0301, buffer, (div(sector, 36) << 2) + (mod(sector, 18)+1), (mod(div(sector, 18), 2) << 2) + 0);
  // interrupt(0x13, 0x0301, buffer, (cylinder << 2) + sector, (head << 2) + drive);
}

char searchParentIndex(char *path, char parentIndex) {
  char folder_name[15];
  int idx_to_file;
  int idx_path = 0;
  // int idx;

  clear(folder_name, 15);
  
  while (1) {
    // menyalin nama folder sampai folder selanjutnya atau menyalin nama file sampai titik
    idx_to_file = 0;
    while (path[idx_path] != '/' && path[idx_path] != '\0') {
      folder_name[idx_to_file] = path[idx_path];
      idx_path++;
      idx_to_file++;
    }
    folder_name[idx_to_file] = '\0';


    // mencari nama folder pada sector files yang akan menjadi parent bagi folder selanjutnya atau file selanjutnya
    idx_to_file = 0;
    while (idx_to_file < 32) {
      if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file_2[idx_to_file*16] && 
          strcmp(folder_name, &sector_file_1[idx_to_file*16+2])) {
        break;
      }
      idx_to_file++;
    }

    // jika nama folder belum ketemu
    if (idx_to_file == 32) {
      while (idx_to_file < 64) {
        if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file_2[idx_to_file*16 - 32*16] && 
            strcmp(folder_name, &sector_file_2[idx_to_file*16 - 32*16 +2])) {
          break;
        }
        idx_to_file++;
      }
    }


    printNum(parentIndex);
    printString("\n");
    // jika nama folder sudah ketemu
    parentIndex = idx_to_file;

    if (path[idx_path] == '\0') break;
    idx_path++;
  }
}

void listAll(char parentIndex) {
  // list semua file 
  int idx_to_file = 0;
  while (idx_to_file < 32) {
    if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file_2[idx_to_file*16 - 32*16]) {
      printStringLength(&sector_file_1[idx_to_file*16+2], 14);
      printString("\n");
    }
    idx_to_file++;
  }

  while (idx_to_file < 64) {
    if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file_2[idx_to_file*16 - 32*16]) {
      printStringLength(&sector_file_2[idx_to_file*16 - 32*16 +2], 14);
      printString("\n");
    }
    idx_to_file++;
  }
}

void makeFile(char *buffer, char *path) {
  char parentIndex = 0xFF;
  char folder_name[15];
  int isAdaYgKosongDiFiles;
  int idx_to_file;
  int idx_path = 0;
  int idx;

  clear(folder_name, 15);
  
  while (1) {
    // tanda ada idx files kosong atau terisi di map: idx = 0 -> buff[10+idx=0]
    isAdaYgKosongDiFiles = 0;
    idx = 0;
    while (isAdaYgKosongDiFiles == 0 && idx < 64) {
      if (sector_map[11+idx] != 0xFF) {
        isAdaYgKosongDiFiles = 1;
      } else {
        idx++;
      }
    }

    if (isAdaYgKosongDiFiles == 0) {
      return;
    }

    // menyalin nama folder sampai folder selanjutnya atau menyalin nama file sampai titik
    idx_to_file = 0;
    while (path[idx_path] != '/' && path[idx_path] != '.') {
      folder_name[idx_to_file] = path[idx_path];
      idx_path++;
      idx_to_file++;
    }
    folder_name[idx_to_file] = '\0';
    if (path[idx_path] == '.') break;

    // mencari nama folder pada sector files yang akan menjadi parent bagi folder selanjutnya atau file selanjutnya
    idx_to_file = 0;
    while (idx_to_file < 32) {
      if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file_2[idx_to_file*16 - 32*16] && 
          strcmp(folder_name, &sector_file_1[idx_to_file*16+2])) {
        break;
      }
      idx_to_file++;
    }

    // jika nama folder belum ketemu
    if (idx_to_file == 32) {
      while (idx_to_file < 64) {
        if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file_2[idx_to_file*16 - 32*16] && 
            strcmp(folder_name, &sector_file_2[idx_to_file*16 - 32*16 +2])) {
          break;
        }
        idx_to_file++;
      }
    }

    // jika nama folder sudah ketemu
    if (idx_to_file < 64) {
      parentIndex = idx;
    } else {
      // jika belum ada, maka buat folder terlebih dahulu
      if (idx < 32) {
        clear(&sector_file_1[idx*16], 16);
        sector_file_1[idx*16] = parentIndex;

        initialize_array(&sector_file_1[idx*16+2], folder_name);  // max 14 character
        sector_map[11+idx] = 0xFF;
      } else {
        clear(&sector_file_2[idx*16 - 32*16], 16);
        sector_file_2[idx*16] = parentIndex;

        initialize_array(&sector_file_2[idx*16-32*16+2], folder_name);  // max 14 character
        sector_map[11+idx] = 0xFF;
      }
      parentIndex = idx;
      printString("parent: ");
      printNum(parentIndex);
      printString("\n");

    }
    idx_path++;
  }

  path[initialize_array(path, folder_name)] = '\0'; 
  writeFile(buffer, path, parentIndex);
}

// void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
void writeFile(char *buffer, char *path, char parentIndex) {

  // char *file_name = path;
  int idx = 0;
  int isAdaYgKosong = 0;
  int isAdaYgKosongDiFiles = 0;
  int isAdaYgKosongDiSectors = 0;
  int idx_file = 0;

  // clear(file_name, 15);
  // file_name[initialize_array(file_name, path)] = '\0';

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
  if (isAdaYgKosongDiFiles && isAdaYgKosongDiSectors) {
    isAdaYgKosong = 1;
  }
  // printString("idx: ");
  // printNum(idx);
  // printString(" ");
  // printString("idx_file: ");
  // printNum(idx_file);
  // printString("\n");
  // printStringToNumLength(&sector_map[10], 10);
  // printString("\n");
  // printStringToNumLength(&sector_map[75], 10);
  // printString("\n");
  
  if (isAdaYgKosong) { 
    if (idx < 32) {
      clear(&sector_file_1[idx*16], 16);
      sector_file_1[idx*16] = parentIndex;

      sector_file_1[idx*16+1] = idx_file;

      initialize_array(&sector_file_1[idx*16+2], path);  // max 14 character
      sector_map[11+idx] = 0xFF;

      initialize_array_length(&sector_sectors[idx_file*16], buffer, 16);
      sector_map[75+idx_file] = 0xFF;
    } else {
      clear(&sector_file_2[idx*16-32*16], 16);
      sector_file_2[idx*16] = parentIndex;

      sector_file_2[idx*16+1] = idx_file;

      initialize_array(&sector_file_2[idx*16-32*16+2], path);  // max 14 character
      sector_map[11+idx] = 0xFF;

      initialize_array_length(&sector_sectors[idx_file*16], buffer, 16);
      sector_map[75+idx_file] = 0xFF;
    }
  }

  // printString("Sector Map\n");
  // printStringToNumLength(sector_map, 80);
  // printString("\n");

  // printString("Sector File\n");
  // printFile(sector_file_1, idx*16);
  // printString("\n");

  // printString("Sector Sectors\n");
  // printIsiFile(sector_sectors, idx_file*16);
  // printString("\n");

  writeSector(sector_map, 10);
  writeSector(sector_file_1, 11);
  writeSector(sector_file_2, 12);
  writeSector(sector_sectors, 13);

}

// void readFile(char *buffer, char *path, int *result, char parentIndex) {
void readFile(char *buffer, char *path, int *result, char parentIndex) {

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

int strcmp (char *str1, char *str2) {
  int i = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i] != str2[i]) return 0;
    i++;
  }
  return 1;
}

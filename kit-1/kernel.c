// /* Ini deklarasi fungsi */
void handleInterrupt21 (int AX, int BX, int CX, int DX);
int copy_arr(char *arr, char *str);
void copy_arr_length(char *arr, char *str, int length);
void printNum(int c);
void printString(char *str);
void printStringLength(char *str, int length);
void printStringToNumLength(char *str, int length);
void readString(char *str);
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
void asciiPrint();

int div(int x, int y);
int mod(int x, int m);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void makeFile(char *buff, char *path);
// void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void writeFile(char *buffer, char *path, char parentIndex);
// void writeFile(char *buffer, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void printFile(char* buff, int file_idx);
void printIsiFile(char* buffer, int file_idx);
char search_curr_idx(char *path, char parentIndex);
char search_parent_idx(char curr_idx);

void listAll(char parentIndex);
void process_dotdot_inPath(char *path);
int strcmp(char *str1, char *str2);
void strcat(char *str1, char *str2);
int strlen(char *str);

char sector_map[512], sector_file[1024], sector_file_2[512], sector_sectors[512];

int main() {
  char read[512], curr_path[70], curr_idx = 0xFF, result_idx;
  // char history[100][10];
  // int idx_history;
  // int repeat = 0;
  // for (idx_history = 0; idx_history < 10; idx_history++) {
  //   history[idx_history][0] = '\0';
  // } idx_history = 0;

  copy_arr(curr_path, "/\0");
  clear(read, 512);
  writeSector(read, 10);
  writeSector(read, 11);
  writeSector(read, 12);
  writeSector(read, 13);
  readSector(sector_map, 10);
  readSector(sector_file, 11);
  readSector(sector_file+512, 12);
  readSector(sector_sectors, 13);

  // interrupt(0x10, 0x00*16*16+0x01, 0, 0, 0);
  // interrupt(0x10, 0x00*16*16+0x01, 0*16*16+0x01, 0, 0);

  // makeInterrupt21();
  // asciiPrint();
  printString("Your name:");
  readString(read);
  printString("\n");
  printString("Selamat datang, ");
  printString(read);
  printString("\n");
  // printString("---------------------\n");
  // interrupt(0x13, 0x02*16*16+0x01, 0x1000, 0x0*16*16+0x02, 0x0*16*16+0x0);

  // clear(read, 16);
  copy_arr(read, "Isi File");
  makeFile(read, "parent111/parent2/parent3/anak.txt");
  makeFile(read, "parent111/parent2/parent3/anak2.txt");
  makeFile(read, "parent_2/parent2/anak4.txt");

  // printString("Sector 100\n");
  // clear(read, 100);
  // readSector(read, 10);
  // printStringToNumLength(read, 100);
  // printString("\n");

  // printString("Sector 101\n");
  // clear(read, 512);
  // readSector(read, 11);
  // // printStringLength(read, 50);
  // // printString("\n");
  // printFile(read, 0);
  // printString("\n");
  // printFile(read, 0x10);
  // printString("\n");
  // printFile(read, 0x20);
  // printString("\n");
  // printFile(read, 0x30);
  // printString("\n");
  // printString("Sector 102\n");
  // clear(read, 512);
  // readSector(read, 12);
  // printStringLength(read, 30);
  // printString("\n");
  // printFile(read, 0);
  // printString("\n");

  // printString("Sector 103\n");
  // clear(read, 512);
  // readSector(read, 13);
  // printStringLength(read, 50);
  // printString("\n");
  // printIsiFile(read, 0*16);
  // printString("\n");
  // printIsiFile(read, 1*16);
  // printString("\n");
  // printIsiFile(read, 2*16);
  // printString("\n");
  // printIsiFile(read, 3*16);
  // printString("\n");
  // terminal
  printString("\n\n");
  while (!strcmp(read, "exit") || strlen(read) != 4) {
    // repeat = 0;
    read[0] = '\0';
    printString("root:");
    printString(curr_path);
    printString("$ ");

    readString(read);
    printString("\n");
    // if (repeat) { continue; }
    // strcat(history[idx_history++], read);
    // printNum(read[0]);
    if (read[0] == '\0') {
      continue;
    } else if (strcmp(read, "ls")) {
      listAll((char) curr_idx);

    } else if (strcmp(read, "cd ")) {
      if (read[2] != '\0' || read[3] != '\0') {
        if (read[3] == '/') {
          // path dari root
          if (read[4] != '\0') {
            result_idx = search_curr_idx(&read[4], 0xFF);
            // jika ada file yg tidak ketemu maka dilewati
            if (result_idx == 0xFA) {
              printString("Directory not found\n");
              continue;
            } else {
              curr_idx = result_idx;
            }
          } else {
            curr_idx = 0xFF;
          }
          // path mulai dari root
          *curr_path = '\0';
          // copy path terbaru
          strcat(curr_path, &read[3]);
          process_dotdot_inPath(&curr_path);
        } else {
          // path relatif
          result_idx = search_curr_idx(&read[3], curr_idx);
          // jika ada file yg tidak ketemu maka dilewati
          if (result_idx == 0xFA) {
            printString("Directory not found\n");
            continue;
          } else {
            curr_idx = result_idx;
          }
          // jika index lama bukan root
          if (result_idx != 0xFF) {
            strcat(curr_path, "/");
          }
          // copy path terbaru
          strcat(curr_path, &read[3]);
          process_dotdot_inPath(&curr_path);
        }
      }

    } else if (strcmp(read, "cat ")) {
      
    } else {
      printString("Command not found\n");
    }
  }

  while (1);
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

void clear(char *buffer, int length){
  int i = 0;
  while(i < length){
    buffer[i++]='\0';
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

// membaca sektor
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

// menulis ke sektor
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

// mencari index dari folder/file tersebut pada sektor files
char search_curr_idx(char *path, char curr_idx) {
  char old_curr_idx = curr_idx, parentIndex, folder_name[15];
  int idx_to_file, idx_path = 0;

  while (1) {
    // menyalin nama folder sampai folder selanjutnya atau menyalin nama file sampai titik
    idx_to_file = 0;
    while (path[idx_path] != '/' && path[idx_path] != '\0') {
      folder_name[idx_to_file] = path[idx_path];
      idx_path++;
      idx_to_file++;
    }
    folder_name[idx_to_file] = '\0';

    if (strcmp(folder_name, "..") ) {
      if (folder_name[0] != '\0' && folder_name[1] != '\0')
        curr_idx = search_parent_idx(curr_idx);
    } else {
      // mencari nama folder pada sector files yang akan menjadi parent bagi folder selanjutnya atau file selanjutnya
      parentIndex = curr_idx;
      for (idx_to_file = 0; idx_to_file < 64; idx_to_file++) {
        if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file[idx_to_file*16] && 
            strcmp(folder_name, &sector_file[idx_to_file*16+2]) && strlen(folder_name) == strlen(&sector_file[idx_to_file*16+2])) {
          break;
        }
      }

      // jika nama folder sudah ketemu
      // index dari folder tersebut ketemu
      if (idx_to_file < 64)
        curr_idx = idx_to_file;
      else
        // jika tidak ketemu maka kembalikan nilai -xFA
        return 0xFA;
    }

    if (path[idx_path] == '\0') break;
    idx_path++;
  }
  // printNum(curr_idx);
  // printString("\n");
  return curr_idx;
}

// mencari index parent pada file/folder di sektor files
char search_parent_idx(char curr_idx) {
  return sector_file[curr_idx*16];
}

void process_dotdot_inPath(char *path) {
  char nama_file[15], new_path[100];
  int idx_path = 1, idx_new_path = 1, i = 0;
  new_path[0] = '\0';
  strcat(new_path, "/");
  while (1) {
    i = 0;
    while (path[idx_path] != '/' && path[idx_path] != '\0') {
      nama_file[i] = path[idx_path];
      idx_path++;
      i++;
    }
    nama_file[i] = '\0';

    if (strcmp(nama_file, "..")) {
      if (nama_file[1] == '\0') {

      } else {
        idx_new_path = strlen(new_path);
        while (new_path[idx_new_path] != '/') {
          idx_new_path--;
        }
        if (idx_new_path > 1) {
          new_path[idx_new_path] = '\0';
        } else {
          new_path[1] = '\0';
        }
      }
    } else {
      if (idx_new_path > 1) {
        strcat(new_path, "/");
      }
      strcat(new_path, nama_file);
    }
    if (path[idx_path] == '\0') break;
    idx_path++;
    idx_new_path++;
  }
  path[copy_arr(path, new_path)] = '\0';
}

// list semua file 
void listAll(char parentIndex) {
  int idx_to_file = 0;
  for (; idx_to_file < 64; idx_to_file++) {
    if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file[idx_to_file*16]) {
      printString(&sector_file[idx_to_file*16+2]);
      printString("\n");
    }
  }
  printString("\n");
}

// membuat folder/file
void makeFile(char *buff, char *path) {
  char parentIndex = 0xFF;
  {
    char folder_name[15];
    int isAdaYgKosongDiFiles, idx_to_file, idx_path = 0, idx;

    clear(folder_name, 15);
    
    while (1) {
      // tanda ada yang kosong pada sektor files
      isAdaYgKosongDiFiles = 0;
      idx = 0;
      while (isAdaYgKosongDiFiles == 0 && idx < 64) {
        if (sector_map[11+idx] != 0xFF)
          isAdaYgKosongDiFiles = 1;
        else
          idx++;
      }

      // jika tidak ada yang kosong maka file tidak dibuat
      if (isAdaYgKosongDiFiles == 0) return;

      // menyalin nama folder sampai folder selanjutnya atau menyalin nama file sampai titik
      idx_to_file = 0;
      while (path[idx_path] != '/' && path[idx_path] != '.') {
        folder_name[idx_to_file] = path[idx_path];
        idx_path++;
        idx_to_file++;
      }
      folder_name[idx_to_file] = '\0';

      // jika path yang sekarang adalah nama file tersebut maka selesai pencarian atau pembuatan folder
      if (path[idx_path] == '.') break;

      // mencari nama folder pada sector files yang akan menjadi parent bagi folder selanjutnya atau file selanjutnya
      idx_to_file = 0;
      while (idx_to_file < 64) {
        if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file[idx_to_file*16] && 
            strcmp(folder_name, &sector_file[idx_to_file*16+2])) {
          break;
        }
        idx_to_file++;
      }

      // jika nama folder sudah ketemu
      if (idx_to_file < 64) {
        parentIndex = idx_to_file;
      } else {
        // jika belum ada, maka buat folder terlebih dahulu
        clear(&sector_file[idx*16], 16);
        // menulis parent dari folder/file
        sector_file[idx*16] = parentIndex;

        // menulis nama folder/file pada sektor files
        sector_file[copy_arr(&sector_file[idx*16+2], folder_name)+idx*16+2] = '\0';  // max 14 character
        printString(&sector_file[idx*16+2]);
        printString("\n");
        // tandai  di map
        sector_map[11+idx] = 0xFF;

        // index sekarang akan menjadi parent untuk folder anaknya
        parentIndex = idx;

        // printString("parent: ");
        // printNum(parentIndex);
        // printString("\n");

      }
      idx_path++;
    }

    path[copy_arr(path, folder_name)] = '\0';
    // printString("File: ");
    // printString(path);
    // printString("Parent: ");
    // printNum(parentIndex);
    // printString("\n");
  }
  writeFile(buff, path, parentIndex);
}

// void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
// menulis file
void writeFile(char *buffer, char *path, char parentIndex) {
  int idx = 0, isAdaYgKosongDiFiles = 0, isAdaYgKosongDiSectors = 0, idx_file = 0;
  // tanda ada yang kosong pada sektor files
  isAdaYgKosongDiFiles = 0;
  while (isAdaYgKosongDiFiles == 0 && idx < 64) {
    if (sector_map[11+idx] != 0xFF)
      isAdaYgKosongDiFiles = 1;
    else
      idx++;
  }

  // tanda ada yang kosong pada sektor sectors
  isAdaYgKosongDiSectors = 0;
  while(isAdaYgKosongDiSectors == 0 && idx_file < 32) {
    if (sector_map[75+idx_file] != 0xFF) {
      isAdaYgKosongDiSectors = 1;
    } else {
      idx_file++;
    }
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
  
  // jika ada yang kosong pada sektor sectors dan sektor files
  if (isAdaYgKosongDiFiles && isAdaYgKosongDiSectors) { 
    sector_map[11+idx] = 0xFF;
    idx = idx*16; // index pada sector files

    // nulis parentIndex pada sector files
    clear(&sector_file[idx], 16);
    sector_file[idx] = parentIndex;

    // nulis index di sectors pada sector files
    sector_file[idx+1] = idx_file;

    // menulis nama file
    sector_file[copy_arr(&sector_file[idx+2], path)+idx+2] = '\0';  // max 14 character

    // menulis isi file
    copy_arr_length(&sector_sectors[idx_file*16], buffer, 16);
    sector_map[75+idx_file] = 0xFF;
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

  // tulis pada sector map, files, dan sectors
  writeSector(sector_map, 10);
  writeSector(sector_file, 11);
  writeSector(sector_file+512, 12);
  writeSector(sector_sectors, 13);
}

// void readFile(char *buffer, char *path, int *result, char parentIndex) {
// membaca file
void readFile(char *buffer, char *path, int *result, char parentIndex) {
  int idx = 0;
  int buffer_pointer = 0;

  while(sector_file[idx*16] != parentIndex && idx < 32){
    idx++;
  }

  if(strcmp(path, &sector_file[idx*16+2]) && strlen(path) == strlen(&sector_file[idx*16+2]) && sector_map[11+idx] == 0xFF){
    int no_sector = idx*16+1;
    while(sector_sectors[no_sector] != 0){
      readSector(buffer + buffer_pointer,13);
      buffer_pointer += 512;
      no_sector++;
    }
  }
  else{
    printString("error message");
    buffer = "\0";
    result = 0;
  }
}

int div(int x, int y) {
  return x/y;
}

int mod(int x, int m) {
  while (x >= m) { x -= m; }
  return x;
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

// membandingkan dua array sampai jika ada null character pada array 1 atau array 2
int strcmp (char *str1, char *str2) {
  int i = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i++] != str2[i]) return 0;
  }
  return 1;
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

#include "fileio.h"
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


// menulis file
void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
  int idx = 0, isAdaYgKosongDiFiles = 0, isAdaYgKosongDiSectors = 0, idx_file = 0;
  // tanda ada yang kosong pada sektor files
  isAdaYgKosongDiFiles = 0;
  while (isAdaYgKosongDiFiles == 0 && idx < 64) {
    if (sector_map[11+idx] != 0xFF){
      isAdaYgKosongDiFiles = 1;}
    else{
      idx++;}
  }
  if (!isAdaYgKosongDiFiles) {
    sectors = -2;
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
  if (!isAdaYgKosongDiFiles) {
    sectors = -3;
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

void readFile1(char *buffer, char *path, int *result, char parentIndex) {
  int idx = 0;
  int buffer_pointer = 0;

  while(sector_file[idx*16] != parentIndex && idx < 32){
    idx++;
  }

  if(strcmp(path, &sector_file[idx*16+2]) && strlen(path) == strlen(&sector_file[idx*16+2]) && sector_map[11+idx] == 0xFF){
    int no_sector = idx*16+1;


  }
  else{
    printString("error message");
    buffer = "\0";
    result = 0;
  }
}
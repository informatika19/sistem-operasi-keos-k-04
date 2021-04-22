#include "fileio.h"
#include "ls.h"
#include "mkdir.h"
#include "text.h"
#include "fileoperation.h"

/* Ini deklarasi fungsi */
void handleInterrupt21 (int AX, int BX, int CX, int DX);
// char search_curr_idx(char *path, char parentIndex);
// char search_parent_idx(char curr_idx);
void process_dotdot_inPath(char *path);


// char sector_map[512], sector_file[1024], sector_sectors[512];
int main() {
  char read[512], curr_path[70], curr_idx = 0xFF;
  char result_idx[1];

  copy_arr(curr_path, "/\0");
  clear(read, 512);
  writeSector(read, 5);
  writeSector(read, 6);
  writeSector(read, 7);
  writeSector(read, 8);
  printString("\n");
  printString(read);
  readSector(sector_map, 5);
  readSector(sector_file, 6);
  readSector(sector_file+512, 7);
  readSector(sector_sectors, 8);
  // printString(sector_map);

  // interrupt(0x10, 0x00*16*16+0x01, 0, 0, 0);
  // interrupt(0x10, 0x00*16*16+0x01, 0*16*16+0x01, 0, 0);

  // makeInterrupt21();
  // asciiPrint();
  // printString("Test");
  // printString("\n");
  // printString("Your name:");
  // readString(read);
  // printString("\n");
  // printString("Selamat datang, ");
  // printString(read);
  // printString("\n");
  // printString("---------------------\n");
  // interrupt(0x13, 0x02*16*16+0x01, 0x1000, 0x0*16*16+0x02, 0x0*16*16+0x0);

  // clear(read, 16);
  //copy_arr(read, "Isi File");
  makeFile("KUcing", "parent111/parent2/parent3/anak.txt");
  makeFile("Aku sayang dia", "parent111/parent2/parent3/anak2.txt");
  makeFile("Budi pergi", "parent_2/parent2/anak4.txt");
  // printFile(sector_file, 0);
  // printFile(sector_file, 1);
  // printFile(sector_file, 2);
  // printFile(sector_file, 3);

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
    read[0] = '\0';
    printString("root:");
    printString(curr_path);
    printString("$ ");

    readString(read);
    printString("\n");
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
            search_curr_idx(&read[4], 0xFF, result_idx);
            // jika ada file yg tidak ketemu maka dilewati
            if (result_idx[0] == 0xFA) {
              printString("Directory not found\n");
              continue;
            } else {
              curr_idx = result_idx[0];
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
          // jika index lama bukan root
          if (curr_idx != 0xFF) {
            strcat(curr_path, "/");
          }
          // path relatif
          search_curr_idx(&read[3], curr_idx, result_idx);
          // jika ada file yg tidak ketemu maka dilewati
          if (result_idx[0] == 0xFA) {
            printString("Directory not found\n");
            continue;
          } else {
            curr_idx = result_idx[0];
          }
          // copy path terbaru
          strcat(curr_path, &read[3]);
          // printString(curr_path);
          process_dotdot_inPath(&curr_path);
        }
      }

    } else if (strcmp(read, "cat ")) {
        int i = 0;
        if (read[4] != '\0'){
          for (; i < 10 ; i++){
            // printNum(i);
            // printString("\n");
            // if (sector_sectors[i] != "\0");
            //   printString(&sector_sectors[i*16]);
            // printString("\n");

            if (sector_map[11+i] == 0xFF && sector_file[i*16] == curr_idx  && sector_file[i*16+1] != 0xFF && strcmpfitlen(&read[4],&sector_file[i*16+2])){
              // printNum(sector_file[i*16+1]);
              // printString("\n");
              // printString(&sector_sectors[2]);
              printString(&sector_sectors[sector_file[i*16+1]*16]);
              printString("\n");
              break;
            }
          }
        }
    } else if (strcmp(read, "mkdir ")) {
      mkdir(&read[6], curr_idx);
    } else {
      printString("Command not found\n");
    }
  }

  while (1);
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


void process_dotdot_inPath(char *path) {
  char folder[15], new_path[100];
  int i_path = 1, i_new_path = 1, i = 0;
  new_path[0] = '\0';
  strcat(new_path, "/");
  clear(folder, 15);
  while (1) {
    // i = 0;
    // while (path[i_path] != '/' && path[i_path] != '\0') {
    //   folder[i] = path[i_path];
    //   i_path++;
    //   i++;
    // }
    // folder[i] = '\0';

    copy_dir(path, folder, &i_path);

    if (strcmp(folder, "..")) {
      if (folder[1] == '\0') {

      } else {
        i_new_path = strlen(new_path);
        while (new_path[i_new_path] != '/') {
          i_new_path--;
        }
        if (i_new_path > 1) {
          new_path[i_new_path] = '\0';
        } else {
          new_path[1] = '\0';
        }
      }
    } else {
      if (i_new_path > 1) {
        strcat(new_path, "/");
      }
      strcat(new_path, folder);
    }
    if (path[i_path] == '\0') break;
    i_path++;
    i_new_path++;
  }
  copy_arr_length(path, new_path, i_new_path);
  // path[i_new_path] = '\0';
  // printString(new_path);
  // path[copy_arr(path, new_path)] = '\0';
}




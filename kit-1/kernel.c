#include "fileio.h"
#include "ls.h"
#include "mkdir.h"
#include "text.h"

/* Ini deklarasi fungsi */
void handleInterrupt21 (int AX, int BX, int CX, int DX);
char search_curr_idx(char *path, char parentIndex);
char search_parent_idx(char curr_idx);
void process_dotdot_inPath(char *path);

char sector_map[512], sector_file[1024], sector_file_2[512], sector_sectors[512];
int main() {
  char read[512], curr_path[70], curr_idx = 0xFF, result_idx;

  copy_arr(curr_path, "/\0");
  clear(read, 512);
  writeSector(read, 10);
  writeSector(read, 11);
  writeSector(read, 12);
  writeSector(read, 13);
  printString("\n");
  printString(read);
  readSector(sector_map, 10);
  readSector(sector_file, 11);
  readSector(sector_file+512, 12);
  readSector(sector_sectors, 13);
  printString(sector_map);

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
  printFile(read, 0);
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
  // agar tidak circular
  if (curr_idx != 0xFF)
    return sector_file[curr_idx*16];
  return 0xFF;
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




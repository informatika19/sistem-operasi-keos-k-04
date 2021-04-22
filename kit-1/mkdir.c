#include "mkdir.h"
#include "fileio.h"
#include "fileoperation.h"
#include "text.h"
// membuat direktori
char mkdir(char *path, char curr_idx) {
  char parentIndex = curr_idx;

  char folder[14];
  int i, i_path = 0, i_file;
  char result_idx[1];

  clear(folder, 14);
  
  curr_idx = search_curr_idx(path, curr_idx, result_idx);
  if (result_idx[0] == 0xFA) {
    // printString("Not Exist");
    while (1) {
      // jika file penuh maka file tidak dibuat
      if (isFileFull(&i_file) == 1) return;
      // idx_to_file = 0;
      // while (path[i_path] != '/' && path[i_path] != '\0') {
      //   folder[idx_to_file] = path[i_path];
      //   i_path++;
      //   idx_to_file++;
      // }
      // proses directory 
      i = copy_dir(path, folder, &i_path);
      while (i < 14) {
        folder[i++] = '\0';
      }
      // printString(folder);
      // buat folder
      // menulis parent dari folder/file
      sector_file[i_file*16] = parentIndex;

      // menulis nama folder/file pada sektor files
      copy_arr_length(&sector_file[i_file*16+2], folder, 14);
      // printString(&sector_file[i_file*16+2]);
      // tandai  di map
      sector_map[11+i_file] = 0xFF;

      // index sekarang akan menjadi parent untuk folder anaknya
      parentIndex = i_file;

      if (path[i_path] == '\0') break;

      i_path++;
    }
  } else {
    parentIndex = curr_idx;
  }

  return parentIndex;
}

// membuat folder/file
void makeFile(char *buff, char *path) {
  char parentIndex = 0xFF;
  int *sectors = 0;
  int i_path = 0;
  int len = 0;
  char file[14];
  char *dir;
  while (path[i_path] != '.') {
    i_path++;
  }
  while (i_path > 0 && path[i_path] != '/') {
    len++;
    i_path--;
  }
  // to make directory if exist
  copy_arr_length(dir, path, i_path);
  dir[i_path] = '\0';
  if (path[i_path] == '/')
    parentIndex = mkdir(dir, parentIndex);

  // copy file name and write file
  len--;
  while (len < 14) {
    file[len++] = '\0';
  }
  copy_arr_length(file, &path[i_path+1], 14);
  // write to file
  writeFile(buff, file, sectors, parentIndex);
}
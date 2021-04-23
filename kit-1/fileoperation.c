#include "fileoperation.h"
#include "fileio.h"
int isFileFull(int *idx) {
    // tanda ada yang kosong pada sektor files
    int isFileFullBool = 1;
    *idx = 0;
    while (isFileFullBool == 1 && (*idx) < 64) {
      if (sector_map[11+(*idx)] != 0xFF)
        isFileFullBool = 0;
      else
        (*idx)++;
    }
    return isFileFullBool;
}

// mencari index dari folder/file tersebut pada sektor files
char search_curr_idx(char *path, char curr_idx, char *result_idx) {
  char old_curr_idx = curr_idx, parent_idx, folder[15];
  int i_file, i_path = 0;

  while (1) {
    // menyalin nama folder sampai folder selanjutnya atau menyalin nama file sampai titik
    // i_file = 0;
    // while (path[i_path] != '/' && path[i_path] != '\0') {
    //   folder[i_file] = path[i_path];
    //   i_path++;
    //   i_file++;
    // }
    // folder[i_file] = '\0';
    i_file = copy_dir(folder, path, &i_path);

    if (strcmp(folder, "..") ) {
      if (folder[0] != '\0' && folder[1] != '\0')
        curr_idx = search_parent_idx(curr_idx);
    } else {
      // mencari nama folder pada sector files yang akan menjadi parent bagi folder selanjutnya atau file selanjutnya
      parent_idx = curr_idx;
      // for (i_file = 0; i_file < 64; i_file++) {
      //   if (sector_map[11+i_file] == 0xFF && parent_idx == sector_file[i_file*16] && 
      //       strcmpfitlen(folder, &sector_file[i_file*16+2])) {
      //       // strcmp(folder, &sector_file[i_file*16+2]) && strlen(folder) == strlen(&sector_file[i_file*16+2])) {
      //     break;
      //   }
      // }
      i_file = search_folder_idx(folder, parent_idx);

      // jika nama folder sudah ketemu
      // index dari folder tersebut ketemu
      if (i_file < 64)
        curr_idx = i_file;
      else {
        // jika tidak ketemu maka nilai result idx = 0xFA
        result_idx[0] = 0xFA;
        // kembalikan nilai index terakhir dicari
        // printString("curr_idx:");
        // printNum(curr_idx);
        return curr_idx;
      }
    }

    if (path[i_path] == '\0') break;
    i_path++;
  }
  // printNum(curr_idx);
  // printString("\n");
  result_idx[0] = curr_idx;
  return curr_idx;
}

// mencari index parent pada file/folder di sektor files
char search_parent_idx(char curr_idx) {
  // agar tidak circular
  if (curr_idx != 0xFF)
    return sector_file[curr_idx*16];
  return 0xFF;
}

char search_folder_idx(char *file_folder, char parent_idx) {
  int i_file_folder = 0;
  // search file/folder index
  for (; i_file_folder < 64; i_file_folder++) {
    if (sector_map[11+i_file_folder] == 0xFF && parent_idx == sector_file[i_file_folder*16] && 
      strcmpfitlen(file_folder, &sector_file[i_file_folder*16+2])) {
      return i_file_folder;
    }
  }
}

char copy_dir(char *folder, char *dir, char *i_dir) {
  int i_folder = 0;
  while (dir[*i_dir] != '/' && dir[*i_dir] != '\0') {
    folder[i_folder++] = dir[(*i_dir)++];
  }
  folder[i_folder] = '\0';
  return i_folder;
}
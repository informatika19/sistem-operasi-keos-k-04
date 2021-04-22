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
  char old_curr_idx = curr_idx, parentIndex, folder[15];
  int idx_to_file, idx_path = 0;

  while (1) {
    // menyalin nama folder sampai folder selanjutnya atau menyalin nama file sampai titik
    // idx_to_file = 0;
    // while (path[idx_path] != '/' && path[idx_path] != '\0') {
    //   folder[idx_to_file] = path[idx_path];
    //   idx_path++;
    //   idx_to_file++;
    // }
    // folder[idx_to_file] = '\0';
    idx_to_file = copy_dir(path, folder, &idx_path);

    if (strcmp(folder, "..") ) {
      if (folder[0] != '\0' && folder[1] != '\0')
        curr_idx = search_parent_idx(curr_idx);
    } else {
      // mencari nama folder pada sector files yang akan menjadi parent bagi folder selanjutnya atau file selanjutnya
      parentIndex = curr_idx;
      for (idx_to_file = 0; idx_to_file < 64; idx_to_file++) {
        if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file[idx_to_file*16] && 
            strcmp(folder, &sector_file[idx_to_file*16+2]) && strlen(folder) == strlen(&sector_file[idx_to_file*16+2])) {
          break;
        }
      }

      // jika nama folder sudah ketemu
      // index dari folder tersebut ketemu
      if (idx_to_file < 64)
        curr_idx = idx_to_file;
      else {
        // jika tidak ketemu maka nilai result idx = 0xFA
        result_idx[0] = 0xFA;
        // kembalikan nilai index terakhir dicari
        return parentIndex;
      }
    }

    if (path[idx_path] == '\0') break;
    idx_path++;
  }
  // printNum(curr_idx);
  // printString("\n");
  result_idx[0] = curr_idx;
  return curr_idx;
}

// mencari index parent pada file/folder di sektor files
char search_parent_idx(char curr_idx) {
  // agar tidak circular
  // if (curr_idx != 0xFF)
  return sector_file[curr_idx*16];
  return 0xFF;
}

char copy_dir(char *dir, char *folder, char *i_dir) {
  int i_folder = 0;
  while (dir[*i_dir] != '/' && dir[*i_dir] != '\0') {
    folder[i_folder] = dir[*i_dir];
    (*i_dir)++;
    i_folder++;
  }
  folder[i_folder] = '\0';
  return i_folder;
}
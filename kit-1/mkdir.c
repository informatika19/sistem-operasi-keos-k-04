
// membuat folder/file
void makeFile(char *buff, char *path) {
  char parentIndex = 0xFF;
  int *sectors = 0;
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
  writeFile(buff, path, sectors, parentIndex);
}
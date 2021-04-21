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
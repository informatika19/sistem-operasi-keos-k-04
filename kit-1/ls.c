#include "fileio.h"
#include "text.h"
#include "ls.h"
// list semua file 
void listAll(char parentIndex) {
  int idx_to_file = 0;
  // printString("ls--");
  // printString("parentIndex:");
  // printNum(parentIndex);
  for (; idx_to_file < 64; idx_to_file++) {
    if (sector_map[11+idx_to_file] == 0xFF && parentIndex == sector_file[idx_to_file*16]) {
      printString(&sector_file[idx_to_file*16+2]);
      printString("\n");
    }
  }
  // printFile(sector_file, 0);
  // printFile(sector_file, 1);
  // printFile(sector_file, 2);
  // printFile(sector_file, 3);
  // printFile(sector_file, 4);
  // printString("\n");
}
#include "fileio.h"
#include "text.h"
#include "ls.h"
// list semua file 
void listAll(char parent_idx) {
  int i_file = 0;
  // printString("ls--");
  // printString("parent_idx:");
  // printNum(parent_idx);
  for (; i_file < 64; i_file++) {
    if (sector_map[11+i_file] == 0xFF && parent_idx == sector_file[i_file*16]) {
      printString(&sector_file[i_file*16+2]);
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
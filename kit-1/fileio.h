
#ifndef __FILEIO_H__
#define __FILEIO_H__
char sector_map[512], sector_file[1024], sector_sectors[512];
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);

#endif
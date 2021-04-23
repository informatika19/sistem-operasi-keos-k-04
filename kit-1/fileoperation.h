#ifndef __FILEOPERATION_H__
#define __FILEOPERATION_H__

int isFileFull();
char search_curr_idx(char *path, char *i_path, char curr_idx, char *result_idx);
char search_parent_idx(char curr_idx);
char copy_dir(char **dir, char **folder, char *idx_path);
char search_folder_idx(char folder, char parent_idx);
#endif
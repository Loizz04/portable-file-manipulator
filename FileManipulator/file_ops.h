#ifndef FILE_OPS_H
#define FILE_OPS_H

int create_file(const char *filename);
int delete_file(const char *filename);
int rename_file_p(const char *oldname, const char *newname);
int copy_file(const char *source, const char *dest);
int move_file(const char *source, const char *dest);

#endif

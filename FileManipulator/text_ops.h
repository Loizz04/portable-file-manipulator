#ifndef TEXT_OPS_H
#define TEXT_OPS_H

int append_text(const char *filename, const char *text);
int insert_text(const char *filename, const char *text, long position);
int clear_text(const char *filename);
int show_file_paged(const char *filename, int lines_per_page);

#endif

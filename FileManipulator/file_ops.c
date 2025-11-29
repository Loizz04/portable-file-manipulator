#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_ops.h"

int create_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not create file %s\n", filename);
        return 0;
    }
    fclose(fp);
    return 1;
}

int delete_file(const char *filename) {
    if (remove(filename) != 0) {
        printf("Error: Could not delete file %s\n", filename);
        return 0;
    }
    return 1;
}

int rename_file_p(const char *oldname, const char *newname) {
    if (rename(oldname, newname) != 0) {
        printf("Error: Could not rename %s to %s\n", oldname, newname);
        return 0;
    }
    return 1;
}

int copy_file(const char *source, const char *dest) {
    FILE *src = fopen(source, "rb");
    if (!src) {
        printf("Error: Could not open source %s\n", source);
        return 0;
    }

    FILE *dst = fopen(dest, "wb");
    if (!dst) {
        printf("Error: Could not create destination %s\n", dest);
        fclose(src);
        return 0;
    }

    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }

    fclose(src);
    fclose(dst);
    return 1;
}

int move_file(const char *source, const char *dest) {
    if (!copy_file(source, dest))
        return 0;

    if (!delete_file(source))
        return 0;

    return 1;
}

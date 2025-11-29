#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_ops.h"

int append_text(const char *filename, const char *text) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    fputs(text, fp);
    fclose(fp);
    return 1;
}

int insert_text(const char *filename, const char *text, long position) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);

    if (position < 0 || position > size) {
        printf("Error: Invalid position\n");
        free(buffer);
        return 0;
    }

    fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Cannot rewrite file.\n");
        free(buffer);
        return 0;
    }

    fwrite(buffer, 1, position, fp);
    fputs(text, fp);
    fputs(buffer + position, fp);

    fclose(fp);
    free(buffer);
    return 1;
}

int clear_text(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Cannot clear file %s\n", filename);
        return 0;
    }
    fclose(fp);
    return 1;
}

int show_file_paged(const char *filename, int lines_per_page) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
        count++;

        if (count == lines_per_page) {
            printf("\n--- Press ENTER to continue ---");
            getchar();
            count = 0;
        }
    }

    fclose(fp);
    return 1;
}

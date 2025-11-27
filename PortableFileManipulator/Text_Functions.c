#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "text_ops.h"
#include <string.h>

// ========================
// FUNCTION PROTOTYPES

void append_text();
void insert_text();
void clear_file();
void show_file();

void text_Functions() {
    int choice;

    while (1) {
        printf("=== FileManipulator Menu ===\n");
        printf("1. Append text to file\n");
        printf("2. Insert text in file\n");
        printf("3. Clear file\n");
        printf("4. Show file content\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case 1: append_text(); break;
        case 2: insert_text(); break;
        case 3: clear_file(); break;
        case 4: show_file(); break;
        case 5: exit(0);
        default: printf("Invalid choice.\n");
        }
        printf("\n");
    }

    //return 0;
}

// ========================
// FUNCTION DEFINITIONS


void append_text() {
    char filename[256];
    char text[1024];

    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    printf("Enter text to append: ");
    fgets(text, sizeof(text), stdin);

    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fprintf(file, "%s", text);
    fclose(file);
    printf("Text appended.\n");
}

void insert_text() {
    char filename[256];
    char text[1024];
    int position;

    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    printf("Enter text to insert: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter position (character index): ");
    scanf("%d", &position);
    while (getchar() != '\n');

    FILE* file = fopen(filename, "r+");
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);

    if (position > filesize) position = filesize;

    char* buffer = malloc(filesize - position + 1);
    fseek(file, position, SEEK_SET);
    fread(buffer, 1, filesize - position, file);

    fseek(file, position, SEEK_SET);
    fwrite(text, 1, strlen(text), file);
    fwrite(buffer, 1, filesize - position, file);

    free(buffer);
    fclose(file);

    printf("Text inserted.\n");
}

void clear_file() {
    char filename[256];

    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fclose(file);
    printf("File cleared.\n");
}

void show_file() {
    char filename[256];
    int lines_per_page;

    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    printf("Enter number of lines per page: ");
    scanf("%d", &lines_per_page);
    while (getchar() != '\n');

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        count++;

        if (count == lines_per_page) {
            printf("Press Enter to continue...");
            getchar();
            count = 0;
        }
    }

    fclose(file);
}

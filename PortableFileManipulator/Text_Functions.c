#define _CRT_SECURE_NO_WARNINGS

/* Text_Functions.c
   Author: Rawan Genina
   Description: Implements text file operations:
     - Append text to a file
     - Insert text at a specific position
     - Clear file contents
     - Display file contents with pagination
     - Menu for selecting these operations
     - Help Funtion 
*/

#include <stdio.h>
#include <stdlib.h>
#include "Text_Functions.h"
#include <string.h>

//
// ================= HELP FUNCTION =================
// Provides help messages for all operations
static void Text_Functions_Help(int mode) {
    switch (mode) {
    case 0: // General help for menu
        printf("=== Text Functions Help ===\n");
        printf("1. Append text to a file\n");
        printf("2. Insert text at a specific position in a file\n");
        printf("3. Clear all contents of a file\n");
        printf("4. Show file contents with pagination\n");
        printf("5. Help\n");
        printf("6. Exit text operations menu\n\n");
        break;
    case 5: // Append text help
        printf("Append Text Help:\n");
        printf("Enter filename, then text to append. Text will be added to the end of the file.\n\n");
        break;
    case 6: // Insert text help
        printf("Insert Text Help:\n");
        printf("Enter filename, text to insert, and character position.\n");
        printf("Text will be inserted at the given position.\n\n");
        break;
    case 7: // Clear file help
        printf("Clear File Help:\n");
        printf("Enter filename. All content of the file will be deleted permanently.\n\n");
        break;
    case 8: // Show file help
        printf("Show File Help:\n");
        printf("Enter filename and number of lines per page.\n");
        printf("The content will be displayed page by page.\n\n");
        break;
    default:
        printf("Unknown help topic.\n");
        break;
    }
}

// ================= MENU WRAPPER =================
void append_text();
void insert_text();
void clear_file();
void show_file();

void Text_Functions() {
    int choice;

    while (1) { // Loop until user chooses exit
        printf("=== FileManipulator Menu ===\n");
        printf("1. Append text to file\n");
        printf("2. Insert text in file\n");
        printf("3. Clear file\n");
        printf("4. Show file content\n");
        printf("5. Help\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        // Validate numeric input
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        while (getchar() != '\n'); // Clear leftover newline

        // Call the appropriate function
        switch (choice) {
        case 1: append_text(); break;
        case 2: insert_text(); break;
        case 3: clear_file(); break;
        case 4: show_file(); break;
        case 5: Text_Functions_Help(0); break;
        case 6: return; // Exit menu
        default: printf("Invalid choice.\n");
        }
        printf("\n");
    }
}

// ================= APPEND FUNCTION =================
void append_text() {
    char filename[256]; // File name
    char text[1024];    // Text to append

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline

    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(5); // Help for append
        return;
    }

    printf("Enter text to append: ");
    fgets(text, sizeof(text), stdin); // Get user input text

    FILE* file = fopen(filename, "a"); // Open file in append mode
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fprintf(file, "%s", text); // Write text at end
    fclose(file);
    printf("Text appended.\n");
}

// ================= INSERT FUNCTION =================
void insert_text() {
    char filename[256];
    char text[1024];
    int position;

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(6); // Help for insert
        return;
    }

    printf("Enter text to insert: ");
    fgets(text, sizeof(text), stdin);

    // UPDATE: remove newline instead of forcing one
    text[strcspn(text, "\n")] = '\0';
    size_t len = strlen(text);

    printf("Enter position (character index): ");
    scanf("%d", &position);
    while (getchar() != '\n'); // clear buffer

    FILE* file = fopen(filename, "rb"); // Open file for reading binary
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fseek(file, 0, SEEK_END); // Determine file size
    long filesize = ftell(file);
    rewind(file);

    if (position > filesize) position = filesize; // Clamp position to file end

    char* buffer = malloc(filesize + len + 1); // Allocate memory
    if (!buffer) {
        printf("Memory error.\n");
        fclose(file);
        return;
    }

    fread(buffer, 1, filesize, file); // Read whole file
    fclose(file);

    memmove(buffer + position + len, buffer + position, filesize - position); // Shift content
    memcpy(buffer + position, text, len); // Insert new text

    file = fopen(filename, "wb"); // Rewrite file
    fwrite(buffer, 1, filesize + len, file);
    fclose(file);

    free(buffer);

    printf("Text inserted successfully.\n");
}

// ================= CLEAR FUNCTION =================
void clear_file() {
    char filename[256];

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(7);
        return;
    }

    FILE* file = fopen(filename, "w"); // Open file to truncate
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fclose(file);
    printf("File cleared.\n");
}

// ================= SHOW FUNCTION (PAGINATION) =================
void show_file() {
    char filename[256];
    int lines_per_page, page_number;

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(8);
        return;
    }

    printf("Enter number of lines per page: ");
    scanf("%d", &lines_per_page);
    while (getchar() != '\n');

    printf("Enter page number to display: ");
    scanf("%d", &page_number);
    while (getchar() != '\n');

    FILE* file = fopen(filename, "r"); // Open file for reading
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    char line[1024];
    int current_line = 0;
    int lines_to_skip = (page_number - 1) * lines_per_page; // Calculate lines to skip to reach page

    // Skip lines before requested page
    while (current_line < lines_to_skip && fgets(line, sizeof(line), file)) {
        current_line++;
    }

    if (current_line < lines_to_skip) {
        printf("Page %d does not exist (file too short).\n", page_number);
        fclose(file);
        return;
    }

    printf("\n=== Page %d ===\n", page_number);

    int printed = 0;
    while (printed < lines_per_page && fgets(line, sizeof(line), file)) {
        printf("%s", line);
        printed++;
    }

    if (printed == 0) {
        printf("Page %d is empty.\n", page_number);
    }
    else if (printed < lines_per_page) {
        printf("\n--- End of File ---\n");
    }

    fclose(file);
}


/* Text_Functions.c

   Author: Rawan Genina

   Declarations for text file operations:
   - Append text to an existing file
   - Insert text at a specified position
   - Clear all contents of a file
   - Display file contents with pagination
   - Menu function for text operations
   - Calls helpfunction() for context-sensitive help
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "Text_Functions.h"
#include <string.h>

// Provides help for all text file operations
static void Text_Functions_Help(int mode) {
    switch (mode) {
    case 0: // Main Text Functions menu
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

// Text Operations (FUNCTION PROTOTYPES)
void append_text();
void insert_text();
void clear_file();
void show_file();

void Text_Functions() {
    int choice;

    //infintie loop //keep showing the menu until user chooses Exit 
    while (1) {
        // Menu List 
        printf("=== FileManipulator Menu ===\n");
        printf("1. Append text to file\n");
        printf("2. Insert text in file\n");
        printf("3. Clear file\n");
        printf("4. Show file content\n");
        printf("5. Help\n");  // help option
        printf("6. Exit\n");  // Adjusted exit to 6 to include help as 5
        printf("Enter your choice: ");

        //validate the input 
        if (scanf("%d", &choice) != 1) {//insure user inters a number 
            printf("Invalid input Please fix it \n");
            while (getchar() != '\n'); // clear input buffer 
            continue;
        }
        while (getchar() != '\n'); // clear newline from buffer// clear leftover newline 

        // User Choices 
        switch (choice) {
        case 1:
            append_text();//appened text to file 
            break;
        case 2:
            insert_text();// insert text to file 
            break;
        case 3:
            clear_file();// remove the file contents 
            break;
        case 4:
            show_file();// display file with pagination 
            break;
        case 5: // HELP OPTION 
            Text_Functions_Help(0);  // global help for Text Functions
            break;
        case 6:
            return;  // exit text functions menu
        default:
            printf("Invalid choice.\n");
        }
        printf("\n");
    }
}
//Append, insert, clear, show 
// FUNCTION DEFINITIONS

//Append text to the end of a file 
void append_text() {
    char filename[256]; // stores file name 
    char text[1024]; //stores text to append 

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;//remove newline 

    // Call help if user enters /h
    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(5);  // append help
        return;
    }

    //get the text from the user to append 
    printf("Enter text to append: ");
    fgets(text, sizeof(text), stdin);

    //open file in append mode 
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fprintf(file, "%s", text);//write text to file 
    fclose(file);
    printf("Text appended.\n");
}

//Insert text at a specific character position in a file 
void insert_text() {
    char filename[256];
    char text[1024];
    int position;

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(6);  // insert help
        return;
    }

    printf("Enter text to insert: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter position (character index): ");
    scanf_s("%d", &position);//get insertion index 
    while (getchar() != '\n');// clear buffer 

    FILE* file = fopen(filename, "r+");// open file for reading and writing 
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }
    //to determine the file size 
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);

    //to insure position is within file bounds 
    if (position > filesize) position = filesize;

    //to allocate buffer for content after insertion point 
    char* buffer = malloc(filesize - position + 1);
    fseek(file, position, SEEK_SET);
    fread(buffer, 1, filesize - position, file);//read remaining content 

    //write new text at position, then remaining content 
    fseek(file, position, SEEK_SET);
    fwrite(text, 1, strlen(text), file);
    fwrite(buffer, 1, filesize - position, file);

    free(buffer);
    fclose(file);

    printf("Text inserted.\n");
}
//clear all content from a file 
void clear_file() {
    char filename[256];

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(7);  // clear help
        return;
    }

    FILE* file = fopen(filename, "w");//open file in write mode, turncates file 
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    fclose(file);
    printf("File cleared.\n");
}

//to display file contents with pagination 
void show_file() {
    char filename[256];
    int lines_per_page;

    printf("Enter filename (or /h for help): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strcmp(filename, "/h") == 0) {
        Text_Functions_Help(8);  // show file help
        return;
    }

    printf("Enter number of lines per page: ");
    scanf_s("%d", &lines_per_page);
    while (getchar() != '\n');//clear buffer 

    FILE* file = fopen(filename, "r");//open file for reading 
    if (!file) {
        printf("Cannot open file.\n");
        return;
    }

    char line[1024];
    int count = 0;

    //Read file line by line 
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        count++;

        //pause after the specified number of lines 
        if (count == lines_per_page) {
            printf("Press Enter to continue...");
            getchar();
            count = 0;
        }
    }

    fclose(file);
}
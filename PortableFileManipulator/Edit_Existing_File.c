/*Edit_existing_file.c

Author: Lois Mathew

Performs following operations:
- Delete existing file
- Rename existing file
- Copy file
- Move file from one directory to other
- Has a comprehensive help function

*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "Edit_Existing_File.h"


#if defined(__unix__) || defined(__APPLE__)
char restrictedChar[] = { '\0' };      //effectively: no restricted chars
char pathSeparator = '/';
char incorrectPathSeparator = '\\';
#elif defined(_WIN32) || defined(_WIN64)
char restrictedChar[] = { '<','>','|','?','*' };
char pathSeparator = '\\';
char incorrectPathSeparator = '/';
#else
// fallback if OS is unknown: no restricted characters
char restrictedChar[] = { '\0' };
char pathSeparator = '/';
char incorrectPathSeparator = '\\';
#endif

//function that trips quotes when file path is inputed (for windows) this function is for easy accessibilty
void stripQuotesIfWindows(char* path) {
#if defined(_WIN32) || defined(_WIN64)
    size_t len = strlen(path);
    if (len >= 2 && path[0] == '"' && path[len - 1] == '"') {
        memmove(path, path + 1, len - 1);
        path[len - 2] = '\0';
    }
#endif
}

//help function 
static void helpfunction(int mode) {
    printf("=== Edit Existing File Help Function ===\n");
    switch (mode) {

    case 0: // main menu help
        printf("MAIN HELP:\n");
        printf("This module lets you manage existing files., you can: \n");
        printf("  1) Delete a file\n");
        printf("  2) Rename a file\n");
        printf("  3) Copy a file\n");
        printf("  4) Move a file\n\n");
        printf("You can type /h at ANY prompt (menu or path input) to see help related to what you are doing\n");
        break;

    case 1:
        printf("\nDELETE HELP:\n");
        printf("  - Use this option to permanently remove a file.\n");
        printf("  - Make sure the path is correct and you really want to delete the file, because this CANNOT be undone\n");
        break;

    case 2:
        printf("\nRENAME HELP:\n");
        printf("  - Use this option to change a file's name and/or location.\n");
        printf("  - You will be asked for:\n");
        printf("      1) CURRENT path of the file.\n");
        printf("      2) NEW name for the file.\n");
        printf("  - Ensure the path exists");
        break;

    case 3:
        printf("\nCOPY HELP:\n");
        printf("  - Use this option to duplicate a file.\n");
        printf("  - You will be asked for:\n");
        printf("      1) SOURCE file path to copy from.\n");
        printf("      2) DESTINATION path/name to copy to.\n");
        printf("  - The program opens the source file in binary mode, reads data into a buffer, and writes it to the destination file entered\n");
        printf("  - Rememeber if the destination file already exists, it will be overwritten\n");
        printf("  - If a specific file path is not defined, the copy of the file will be created in the local diretory\n");
        break;

    case 4:
        printf("\nMOVE HELP:\n");
        printf("  - Use this option to move a file to a new location.\n");
        printf("  - You will be asked for:\n");
        printf("      1) SOURCE file path to move from.\n");
        printf("      2) DESTINATION path/name to move to.\n");
        printf("  - The program will copy the file to the new directory and then delete the file in the current directory.\n");
        break;

    default:
        printf("Unknown help topic.\n");
        break;
    }
}

//function that checks illegal characters for group of restricted characters
int checkIllegalCharc(const char* path) {
    size_t lengthOfPath;
    int sizeRestritedChars;
    size_t j;

    if (path == NULL) {
        return 1;
    }

    lengthOfPath = strlen(path);
    sizeRestritedChars = (int)(sizeof(restrictedChar) / sizeof(restrictedChar[0]));

    for (int i = 0; i < sizeRestritedChars; i++) {
        for (j = 0; j < lengthOfPath; j++) {
            if (path[j] == restrictedChar[i]) {
                return 1; // illegal character was found
            }
        }
    }
    return 0;
}

//function to print illegal char messages
void printIllegalCharMessage(const char* path) {

    int sizeRestrictedCharArray;

    printf("Filename or path '%s' contains an illegal character.\n", path);
    printf("Restricted characters on this system: ");

    sizeRestrictedCharArray = (int)(sizeof(restrictedChar) / sizeof(restrictedChar[0]));
    for (int i = 0; i < sizeRestrictedCharArray; i++) {
        if (restrictedChar[i] != '\0') { // avoid printing NUL on Unix/mac
            printf("%c ", restrictedChar[i]);
        }
    }
    printf("\n\n");
}

//delete file function
int deleteFile(const char* filePath) {
    if (filePath == NULL || filePath[0] == '\0') {
        printf("Empty Path\n");
        return -1;
    }

    if (checkIllegalCharc(filePath)) {
        printIllegalCharMessage(filePath);
        return -1;
    }

    if (remove(filePath) == 0) {
        printf("\nSuccessfully deleted file %s \n\n", filePath);
        return 0;
    }
    else {
        perror("remove");
        printf("\nFailed to delete file '%s'\n\n", filePath);
        return -1;
    }
}

//rename file function
int renameFile(const char* oldName, const char* newName) {
    //check for invalid
    if (!oldName || !newName || oldName[0] == '\0' || newName[0] == '\0') {
        printf("\nrenameFile: invalid old or new name.\n");
        return -1;
    }

    if (checkIllegalCharc(oldName)) {
        printIllegalCharMessage(oldName);
        return -1;
    }
    if (checkIllegalCharc(newName)) {
        printIllegalCharMessage(newName);
        return -1;
    }
    //rename the file
    if (rename(oldName, newName) == 0) {
        printf("\nRenamed '%s' -> '%s'\n\n", oldName, newName);
        return 0;
    }
    //error message if there was function error
    else {
        perror("rename");
        printf("\nFailed to rename '%s' to '%s'\n\n", oldName, newName);
        return -1;
    }
}
//copy fule function
int copyFile(const char* sourcePath, const char* destPath) {
    FILE* sourceFile;
    FILE* destFile;
    char buffer[4096];
    size_t bytesRead;
    size_t bytesWritten;

    //check for invalid or null paths
    if (!sourcePath || !destPath || sourcePath[0] == '\0' || destPath[0] == '\0') {
        printf("\ncopyFile: invalid source or destination path.\n");
        return -1;
    }
    //check for illegal charecters in source path
    if (checkIllegalCharc(sourcePath)) {
        printIllegalCharMessage(sourcePath);
        return -1;
    }
    //check for illegal charecters in dest path
    if (checkIllegalCharc(destPath)) {
        printIllegalCharMessage(destPath);
        return -1;
    }

    //open source file
    sourceFile = fopen(sourcePath, "rb");
    //check error during open
    if (sourceFile == NULL) {
        perror("fopen source");
        printf("\nFailed to open source file '%s'\n\n", sourcePath);
        return -1;
    }
    //open destination file
    destFile = fopen(destPath, "wb");

    //copying bytes from source file to detination file 
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        bytesWritten = fwrite(buffer, 1, bytesRead, destFile);
        //catches write errors
        if (bytesWritten != bytesRead) {
            perror("fwrite");
            printf("\nWrite error while copying '%s' to '%s'\n\n", sourcePath, destPath);
            fclose(sourceFile);
            fclose(destFile);
            return -1;
        }
    }
    //catches read errors when copying
    if (ferror(sourceFile)) {
        perror("fread");
        printf("\nRead error while copying '%s'\n\n", sourcePath);
        fclose(sourceFile);
        fclose(destFile);
        return -1;
    }
    //cleanup
    fclose(sourceFile);
    fclose(destFile);

    printf("Copied '%s' -> '%s'\n\n", sourcePath, destPath);
    return 0;
}

//function to move a file
int moveFile(const char* sourcePath, const char* destPath) {

    if (!sourcePath || !destPath || sourcePath[0] == '\0' || destPath[0] == '\0') {
        printf("moveFile: invalid path.\n");
        return -1;
    }

    if (checkIllegalCharc(sourcePath)) {
        printIllegalCharMessage(sourcePath);
        return -1;
    }
    if (checkIllegalCharc(destPath)) {
        printIllegalCharMessage(destPath);
        return -1;
    }

    // do not overwrite an existing destination file
    {
        FILE* testFile = fopen(destPath, "rb");
        if (testFile != NULL) {
            fclose(testFile);
            printf("\nmoveFile: destination file '%s' already exists. Move aborted.\n\n", destPath);
            return -1;
        }
    }

    if (copyFile(sourcePath, destPath) != 0) {
        printf("\nmoveFile: copy failed.\n");
        return -1;
    }

    if (deleteFile(sourcePath) != 0) {
        printf("\nmoveFile: delete failed.\n");
        return -1;
    }

    printf("Moved '%s' -> '%s'\n", sourcePath, destPath);
    return 0;
}


void Edit_Existing_File() {
    int choice;
    char sourcePath[256];
    char destPath[256];
    char choiceinp[32];
    int c;

    //clear leftover newline from previous input (e.g., main menu scanf)
    while ((c = getchar()) != '\n' && c != EOF) {
        //discard
    }

    while (1) {
        printf("\n====Edit Existing File Menu====\n");
        printf("\n");
        printf("1. Delete a file\n");
        printf("2. Rename a file\n");
        printf("3. Copy a file\n");
        printf("4. Move a file\n");
        printf("5. Help (You can also access the help function any time using /h)\n");
        printf("6. Return to main menu\n");
        printf("\nEnter your choice (1-6): ");

        //takes bad input, clears stdin and retakes user input
        if (!fgets(choiceinp, sizeof(choiceinp), stdin)) {
            printf("\nInvalid Input, enter number from 1-6\n");
            continue;
        }
        choiceinp[strcspn(choiceinp, "\n")] = '\0';
        int valid = 1;
        if (choiceinp[0] == '\0') {
            valid = 0;
        }
        for (int i = 0; choiceinp[i] != '\0'; i++) {
            if (choiceinp[i] < '0' || choiceinp[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (valid) {
            choice = atoi(choiceinp);
        }
        else {
            choice = -1;
        }
        if (choice < 1 || choice > 6) {
            printf("\nInvalid Input, enter number from 1-6\n");
            continue;
        }


        if (choice == 6) {
            printf("\nReturning to main menu\n");
            break;
        }

        switch (choice) {
        case 1:

            printf("Enter path of file to delete (or /h for delete help): ");
            if (!fgets(sourcePath, sizeof(sourcePath), stdin)) {
                printf("Input error.\n");
                break;
            }
            sourcePath[strcspn(sourcePath, "\n")] = '\0';
            //strip quotes
            stripQuotesIfWindows(sourcePath);

            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(1);   // delete help
                continue;          // back to menu
            }
            deleteFile(sourcePath);
            break;


        case 2:

            printf("Enter the CURRENT path of the file (or /h for rename help): ");
            //fgets
            if (!fgets(sourcePath, sizeof(sourcePath), stdin)) {
                printf("Input error.\n");
                break;
            }
            sourcePath[strcspn(sourcePath, "\n")] = '\0';
            //strip quotes
            stripQuotesIfWindows(sourcePath);

            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(2);   // rename help
                continue;
            }
            printf("\n");

            printf("Enter the NEW name for the file (or /h for rename help): ");
            if (!fgets(destPath, sizeof(destPath), stdin)) {
                printf("Input error.\n");
                break;
            }
            destPath[strcspn(destPath, "\n")] = '\0';

            stripQuotesIfWindows(destPath);

            if (strcmp(destPath, "/h") == 0) {
                helpfunction(2);
                continue;
            }

            renameFile(sourcePath, destPath);
            break;

        case 3:
            printf("\nEnter the SOURCE file path to COPY (or /h for help): ");
            if (!fgets(sourcePath, sizeof(sourcePath), stdin)) {
                printf("Input error.\n");
                break;
            }
            sourcePath[strcspn(sourcePath, "\n")] = '\0';

            stripQuotesIfWindows(sourcePath);

            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(3);   // copy help
                continue;
            }

            printf("\nEnter the DESTINATION path / name of file (or /h for help): ");
            if (!fgets(destPath, sizeof(destPath), stdin)) {
                printf("Input error.\n");
                break;
            }
            destPath[strcspn(destPath, "\n")] = '\0';

            stripQuotesIfWindows(destPath);

            if (strcmp(destPath, "/h") == 0) {
                helpfunction(3);
                continue;
            }

            copyFile(sourcePath, destPath);
            break;

        case 4:
            printf("\nEnter the SOURCE file path to MOVE (or /h for help): ");
            if (!fgets(sourcePath, sizeof(sourcePath), stdin)) {
                printf("Input error.\n");
                break;
            }
            sourcePath[strcspn(sourcePath, "\n")] = '\0';
            stripQuotesIfWindows(sourcePath);
            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(4);   // move help
                continue;
            }

            printf("\nEnter the DESTINATION path/name (or /h for help): ");
            if (!fgets(destPath, sizeof(destPath), stdin)) {
                printf("Input error.\n");
                break;
            }
            destPath[strcspn(destPath, "\n")] = '\0';
            stripQuotesIfWindows(destPath);
            if (strcmp(destPath, "/h") == 0) {
                helpfunction(4);
                continue;
            }

            moveFile(sourcePath, destPath);
            break;

        case 5:
            helpfunction(0);
            break;

        default:

            printf("Invalid choice. Enter a number from 1-6.\n\n");
            break;
        }
    }
}

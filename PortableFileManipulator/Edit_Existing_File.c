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
char restrictedChar[] = { '\0' };      // effectively: no restricted chars
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

//help function 
void helpfunction(int mode) {
    printf("=== Edit Existing File Help Function ===\n");
    switch (mode) {

    case 0: // main menu help
        printf("MAIN HELP:\n");
        printf("This module lets you manage existing files.\n");
        printf("Available operations:\n");
        printf("  1) Delete a file\n");
        printf("  2) Rename a file\n");
        printf("  3) Copy a file\n");
        printf("  4) Move a file\n\n");
        printf("You can type /h at ANY prompt (menu or path input)\n");
        printf("to see help related to what you are doing.\n");
        break;

    case 1:
        printf("\nDELETE HELP:\n");
        printf("  - Use this option to permanently remove a file.\n");
        printf("  - The program calls remove(filePath).\n");
        printf("  - Make sure the path is correct and you really want\n");
        printf("    to delete the file, because this CANNOT be undone.\n");
        break;

    case 2:
        printf("\nRENAME HELP:\n");
        printf("  - Use this option to change a file's name and/or location.\n");
        printf("  - You will be asked for:\n");
        printf("      1) CURRENT name/path of the file.\n");
        printf("      2) NEW name/path for the file.\n");
        printf("  - Internally uses rename(oldName, newName).\n");
        printf("  - If illegal characters are found in either path,\n");
        printf("    the program will abort the rename operation.\n");
        break;

    case 3:
        printf("\nCOPY HELP:\n");
        printf("  - Use this option to duplicate a file.\n");
        printf("  - You will be asked for:\n");
        printf("      1) SOURCE file path to copy from.\n");
        printf("      2) DESTINATION path/name to copy to.\n");
        printf("  - The program opens the source file in binary mode,\n");
        printf("    reads data into a buffer, and writes it to the\n"
            "    destination file in chunks.\n");
        printf("  - If the destination file already exists, it will\n");
        printf("    be overwritten.\n");
        break;

    case 4:
        printf("\nMOVE HELP:\n");
        printf("  - Use this option to move a file to a new location.\n");
        printf("  - You will be asked for:\n");
        printf("      1) SOURCE file path to move from.\n");
        printf("      2) DESTINATION path/name to move to.\n");
        printf("  - The program first tries rename(source, dest).\n");
        printf("  - If rename fails (e.g., different drives), it tries:\n");
        printf("      copyFile(source, dest)   then\n");
        printf("      deleteFile(source)\n");
        printf("    so it behaves like a move.\n");
        break;

    default:
        printf("Unknown help topic.\n");
        break;
    }
}

//function that checks illegal characters
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
        printf("Successfully deleted file %s \n\n", filePath);
        return 0;
    }
    else {
        perror("remove");
        printf("Failed to delete file '%s'\n\n", filePath);
        return -1;
    }
}

int renameFile(const char* oldName, const char* newName) {
    if (oldName == NULL || newName == NULL ||
        oldName[0] == '\0' || newName[0] == '\0') {
        printf("renameFile: invalid old or new name.\n");
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

    if (rename(oldName, newName) == 0) {
        printf("\nRenamed '%s' -> '%s'\n\n", oldName, newName);
        return 0;
    }
    else {
        perror("rename");
        printf("\nFailed to rename '%s' to '%s'\n\n", oldName, newName);
        return -1;
    }
}

int copyFile(const char* sourcePath, const char* destPath) {
    FILE* sourceFile;
    FILE* destFile;
    char buffer[4096];
    size_t bytesRead;
    size_t bytesWritten;

    if (sourcePath == NULL || destPath == NULL ||
        sourcePath[0] == '\0' || destPath[0] == '\0') {
        printf("copyFile: invalid source or destination path.\n");
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

    sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL) {
        perror("fopen source");
        printf("Failed to open source file '%s'\n\n", sourcePath);
        return -1;
    }

    destFile = fopen(destPath, "wb");
    if (destFile == NULL) {
        perror("fopen dest");
        printf("Failed to open destination file '%s'\n\n", destPath);
        fclose(sourceFile);
        return -1;
    }

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        bytesWritten = fwrite(buffer, 1, bytesRead, destFile);
        if (bytesWritten != bytesRead) {
            perror("fwrite");
            printf("\nWrite error while copying '%s' to '%s'\n\n",
                sourcePath, destPath);
            fclose(sourceFile);
            fclose(destFile);
            return -1;
        }
    }

    if (ferror(sourceFile)) {
        perror("fread");
        printf("\nRead error while copying '%s'\n\n", sourcePath);
        fclose(sourceFile);
        fclose(destFile);
        return -1;
    }

    fclose(sourceFile);
    fclose(destFile);

    printf("Copied '%s' -> '%s'\n\n", sourcePath, destPath);
    return 0;
}

int moveFile(const char* sourcePath, const char* destPath) {
    if (sourcePath == NULL || destPath == NULL ||
        sourcePath[0] == '\0' || destPath[0] == '\0') {
        printf("\nmoveFile: invalid source or destination path.\n");
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

    // First method is rename method
    if (rename(sourcePath, destPath) == 0) {
        printf("\nMoved '%s' -> '%s' (via rename)\n\n",
            sourcePath, destPath);
        return 0;
    }

    // rename failed: print why, then attempt copy+delete 
    perror("rename in moveFile (will try copy+delete)");
    printf("Falling back to copy+delete for move operation.\n");

    if (copyFile(sourcePath, destPath) == 0) {
        if (deleteFile(sourcePath) == 0) {
            printf("Moved '%s' -> '%s' (via copy+delete)\n\n",
                sourcePath, destPath);
            return 0;
        }
        else {
            printf("moveFile: copy succeeded but delete failed.\n\n");
            return -1;
        }
    }
    else {
        printf("moveFile: copy failed, move aborted.\n\n");
        return -1;
    }
}

void Edit_Existing_File() {
    int choice;
    char sourcePath[256];
    char destPath[256];
    int c;

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
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid Input, enter number from 1-6\n");
            while ((c = getchar()) != '\n' && c != EOF) {
                //discard 
            }
            continue;
        }

        if (choice == 6) {
            printf("Returning to main menu\n");
            break;
        }

        switch (choice) {
        case 1:
            printf("Enter path of file to delete (or /h for delete help): ");
            scanf("%255s", sourcePath);
            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(1);   // delete help
                continue;          // back to menu
            }
            deleteFile(sourcePath);
            break;

        case 2:
            printf("Enter the CURRENT name/path of the file: ");
            scanf("%255s", sourcePath);
            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(2);   // rename help
                continue;
            }

            printf("Enter the NEW name/path for the file: ");
            scanf("%255s", destPath);
            if (strcmp(destPath, "/h") == 0) {
                helpfunction(2);   // rename help again
                continue;
            }

            renameFile(sourcePath, destPath);
            break;

        case 3:
            printf("Enter the SOURCE file path to COPY (or /h for help): ");
            scanf("%255s", sourcePath);
            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(3);   // copy help
                continue;
            }

            printf("Enter the DESTINATION path (or /h for help): ");
            scanf("%255s", destPath);
            if (strcmp(destPath, "/h") == 0) {
                helpfunction(3);   // copy help again
                continue;
            }

            copyFile(sourcePath, destPath);
            break;

        case 4:
            printf("Enter the SOURCE file path to MOVE (or /h for help): ");
            scanf("%255s", sourcePath);
            if (strcmp(sourcePath, "/h") == 0) {
                helpfunction(4);   // move help
                continue;
            }

            printf("Enter the DESTINATION path/name (or /h for help): ");
            scanf("%255s", destPath);
            if (strcmp(destPath, "/h") == 0) {
                helpfunction(4);   // move help again
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

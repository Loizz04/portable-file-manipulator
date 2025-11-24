// ...existing code...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int fileOpenOnWindows(char input[]) {
    // stub for Windows-specific handling; return 0 for now
    return 0;
}

int main()
{
    #if defined(__unix__)||defined(__APPLE__) 
        char restrictedChar[] = {'/'};
        char pathSeparator = '/';
        char incorrectPathSeparator = '\\';
    #elif defined(_WIN32)||defined(_WIN64)
        char restrictedChar[] = {'<','>',':','"','/','\\','|','?','*'};
        char pathSeparator = '\\';
        char incorrectPathSeparator = '/';
    #endif

    int sizeRestrictedCharArray = sizeof(restrictedChar) / sizeof(char);
    FILE *file;
    char input[255] = "";

    while (1) {
        printf("Enter a file you'd like to create, quit() to choose another action, or enter /h for help: ");
        if (scanf("%254s", input) != 1) {           // safe width specifier
            memset(input, 0, sizeof(input));
            continue;
        }

        if (strcmp(input, "/h") == 0) {
            printf("Create File: enter filename and extension to create in current directory.\n");
            printf("Restricted characters for this system: ");
            for (int i = 0; i < sizeRestrictedCharArray; ++i) printf("%c ", restrictedChar[i]);
            printf("\nPath separator for this system: %c\n\n", pathSeparator);
            memset(input, 0, sizeof(input));
            continue;
        }

        if (strcmp(input, "quit()") == 0) {
            return EXIT_SUCCESS;
        }

        // validate restricted characters (only up to actual input length)
        int invalid = 0;
        size_t inlen = strlen(input);
        for (int i = 0; i < sizeRestrictedCharArray && !invalid; ++i) {
            for (size_t j = 0; j < inlen; ++j) {
                if (input[j] == restrictedChar[i]) {
                    invalid = 1;
                    break;
                }
            }
        }
        if (invalid) {
            printf("You've used an illegal character in your filename. Pick a new name and try again.\n\n");
            memset(input, 0, sizeof(input));
            continue;
        }

    #if defined(_WIN32) || defined(_WIN64)
        /* Use only C standard library: check existence with "r" then create with "w".
           Note: this is not atomic (race possible) but avoids non-standard open()/O_CREAT. */
        errno = 0;
        file = fopen(input, "r");
        if (file != NULL) {
            /* file exists */
            fclose(file);
            printf("File '%s' already exists. Pick a new filename and try again!\n\n", input);
        } else if (errno != 0 && errno != ENOENT) {
            /* fopen failed for a reason other than "not found" */
            perror("fopen");
            printf("\n");
        } else {
            /* file not found — create with "w" (portable C) */
            file = fopen(input, "w");
            if (file == NULL) {
                perror("fopen");
                printf("\n");
            } else {
                printf("Created file '%s'\n\n", input);
                fclose(file);
            }
        }
    #elif defined(__linux__)
    // This is an fopen mode introduced in C11 that will wrtie if the file doesn't
    // exist, and it fails if the file does
    file = fopen(input,"wx"); 
    if(file == NULL)
    {
        // We do not have Read or Write access to this file
        printf("Cannot open file! The filename you entered already exists in this directory.\nPick a new filename and try again!\n\n");
    }else {
    fclose(file);
    }
    #endif

        memset(input, 0, sizeof(input));
    }
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_ops.h"
#include "text_ops.h"

int main() {
    int choice;
    char filename[100], filename2[100], text[500];
    long pos;
    int lines;

    while (1) {
        printf("\n==== Portable File Manipulator ====\n");
        printf("1. Create file\n");
        printf("2. Delete file\n");
        printf("3. Rename file\n");
        printf("4. Copy file\n");
        printf("5. Move file\n");
        printf("6. Append text\n");
        printf("7. Insert text\n");
        printf("8. Clear text\n");
        printf("9. Show file (paged)\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);
        getchar(); // clean buffer

        switch (choice) {
        case 1:
            printf("Enter file name: ");
            scanf("%s", filename);
            create_file(filename);
            break;

        case 2:
            printf("Enter file name: ");
            scanf("%s", filename);
            delete_file(filename);
            break;

        case 3:
            printf("Old name: ");
            scanf("%s", filename);
            printf("New name: ");
            scanf("%s", filename2);
            rename_file_p(filename, filename2);
            break;

        case 4:
            printf("Source file: ");
            scanf("%s", filename);
            printf("Destination file: ");
            scanf("%s", filename2);
            copy_file(filename, filename2);
            break;

        case 5:
            printf("Source file: ");
            scanf("%s", filename);
            printf("Destination file: ");
            scanf("%s", filename2);
            move_file(filename, filename2);
            break;

        case 6:
            printf("File name: ");
            scanf("%s", filename);
            getchar();
            printf("Text to append: ");
            fgets(text, sizeof(text), stdin);
            append_text(filename, text);
            break;

        case 7:
            printf("File name: ");
            scanf("%s", filename);
            printf("Position (char index): ");
            scanf("%ld", &pos);
            getchar();
            printf("Text to insert: ");
            fgets(text, sizeof(text), stdin);
            insert_text(filename, text, pos);
            break;

        case 8:
            printf("File name: ");
            scanf("%s", filename);
            clear_text(filename);
            break;

        case 9:
            printf("File name: ");
            scanf("%s", filename);
            printf("Lines per page: ");
            scanf("%d", &lines);
            getchar();
            show_file_paged(filename, lines);
            break;

        case 0:
            return 0;

        default:
            printf("Invalid option.\n");
        }
    }

    return 0;
}

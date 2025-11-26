/* main.c
   Calls the Edit Existing File module.
   Author: Lois Mathew
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Edit_Existing_File.h"

int main() {

    int choice;

    while (1) {

        printf("\n====MAIN MENU====\n");
        printf("\n");
        printf("1. Edit Existing File\n");
        printf("2. Help\n");
        printf("3. Exit Program\n");
        printf("\nEnter your choice : ");

        if (scanf_s("%d", &choice) != 1) {

            printf("Invalid input. Please enter a number.\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                //clear stdin 
            }

            continue;
        }

        switch (choice) {

        case 1:
            Edit_Existing_File();   // <-- call your full menu function
            break;

        case 2:
            helpfunction(0);        // global help
            break;

        case 3:
            printf("Exiting program...\n");
            return 0;

        default:
            printf("Invalid input. Enter 1-3.\n");
            break;
        }
    }

    return 0;
}

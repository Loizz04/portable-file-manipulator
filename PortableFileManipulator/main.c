/* main.c
   calls all functions of pofm
   Author: Lois Mathew
   */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Text_Functions.h"   // text functions 
#include "Edit_Existing_File.h"
#include "CreateFile.h"


int main() {

    int choice;

    while (1) {

        printf("==== Welcome to the pofm system ====");
        printf("\n==== MAIN MENU ====\n");
        printf("\n");
        printf("1. Create new file\n");
        printf("2. Edit Existing File\n");
        printf("3. Text Functions\n");
        printf("4. Help\n");
        printf("5. Exit\n");
        printf("\nEnter your choice : ");

        if (scanf("%d", &choice) != 1) {

            printf("Invalid input. Please enter a number.\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                //clear stdin 
            }

            continue;
        }

        switch (choice) {

        case 1:
            CreateFile();   
            break;

        case 2:
            Edit_Existing_File();
            break;

        case 3:
            Text_Functions();
            break;

        case 4:
            printf("This is the main menu, please select one out of the 4 options, more detailed help functions are defined in seperate files");  // global help
            break;

        case 5:
            printf("Exiting program...\n");
            return 0;

        default:
            printf("Invalid input. Enter 1-5.\n");
            break;
        }
    }

    return 0;
}

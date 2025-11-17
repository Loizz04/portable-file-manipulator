// Testing Git push

#include <stdio.h>

void Create_New_File();
void Edit_Existing_File();
void build_Path();


int main() {
	int choice;
	while (1) {
		printf("=====================================\n");
		printf("  Portable File Manipulation system \n");
		printf("=====================================\n");
		printf("1. Edit Existing File\n");
		printf("2. Create New File\n ");
		printf("3. Exit\n");
		printf("Enter your choice: ");

		if (scanf_s("%d", &choice) != 1) {
			printf("\nInvalid input. Please enter a number.\n");
			clear_input();
			continue;
		}


		clear_input();

		switch (choice) {
		case 1:
			Edit_Existing_File();
			break;
		case 2:
			Create_New_File();
			break;
		case 3:
			printf("\nExiting program...\n");
			exit(0);
		default:
			printf("invalid input, please enter 1-3");
			break;
		}

		printf("\nPress Enter to return to main menu...");
		getchar(); // wait for user
	}

	return 0;
}
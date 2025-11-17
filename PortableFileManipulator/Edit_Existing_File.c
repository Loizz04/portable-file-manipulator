#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build_Path.h"   // if you want to use build_Path() here

char directory;
char fileName;
char path;

void Edit_Existing_File() {
	printf("Enter your the directory path: ");
	fgets(directory, sizeof(directory),stdin);
	directory[strcspn(directory, "\n")] = "\0";

	printf("Enter file name: ");
	fgets(fileName, sizeof(fileName), stdin);
	fileName[strcspn(fileName, "\n")] = "\0";


}






//optional build path function 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//charecter pointer directory set to address 
void build_Path(const char *directory, const char  *fileName, char *storeDirectory, size_t dirsize) {
	size_t len_dir = strlen(directory);

	//we first copy the directory into a directory holder
	strncpy(storeDirectory, directory, dirsize - 1);
	storeDirectory[dirsize - 1] = '\0';   // safety null terminator

	//correct any user slash faults
	if (len_dir > 0) {
		char last = directory[len_dir - 1];
		if (last != '/' && last != '\\') {
			strncat(storeDirectory, "/", dirsize - strlen(storeDirectory) - 1);
		}
	}
	//finally we append the file name 
	strncat(storeDirectory, fileName, dirsize - strlen(storeDirectory) - 1);

}
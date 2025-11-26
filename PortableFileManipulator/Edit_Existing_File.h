/* Edit_Existing_File.h

   Author: Lois Mathew

   Declarations for editing existing files:
   - Delete existing file
   - Rename existing file
   - Copy file
   - Move file from one directory to another
   - Help function for these operations
*/
#define _CRT_SECURE_NO_WARNINGS

#ifndef EDIT_EXISTING_FILE_H
#define EDIT_EXISTING_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

	extern char restrictedChar[];
	extern char pathSeparator;
	extern char incorrectPathSeparator;

	void helpfunction(int mode);

	int checkIllegalCharc(const char* path);

	void printIllegalCharMessage(const char* path);

	int deleteFile(const char* filePath);

	int renameFile(const char* oldName, const char* newName);

	int copyFile(const char* sourcePath, const char* destPath);

	int moveFile(const char* sourcePath, const char* destPath);

	void Edit_Existing_File(void);

#ifdef __cplusplus
}
#endif

#endif /* EDIT_EXISTING_FILE_H */
#pragma once

/* Text_Functions.h
   Author: Rawan Genina

   Function declarations for text file operations:
   - text_Functions: main menu handler
   - append_text: append text to a file
   - insert_text: insert text at a given position
   - clear_file: erase all contents of a file
   - show_file: display file content with pagination
*/

#ifndef TEXT_FUNCTIONS_H
#define TEXT_FUNCTIONS_H

// Function declarations
void Text_Functions(void);  // main menu handler
void append_text(void);
void insert_text(void);
void clear_file(void);
void show_file(void);
void helpfunction(int help_type);


#endif // TEXT_FUNCTIONS_H

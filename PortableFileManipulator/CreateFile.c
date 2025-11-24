#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // handle the different operating system imports
    // We can use these to set the value of a variable to handle the differnt directory handling
    // of MS Windows or UNIX-based Machines.
    #if defined(__unix__)||defined(__APPLE__) 
        char restrictedChar[] = {'/'};
        char pathSeparator = '/';
        char incorrectPathSeparator = '\\';
        
    #elif defined(_WIN32)||defined(_WIN64)
        char retrictedChar[] = {'<','>',':','"','/','\\','|','?','*'};
        char pathSeparator = '\\';
        char incorrectPathSeparator = '/';
    #endif
    int sizeRestrictedCharArray = sizeof(restrictedChar)/sizeof(char);
    int fileCheck = 0;
    FILE *file;
    char input[255] = "";

    // this is all in local dir
    while(1){
    while(!strcmp(input,""))
    {
        printf("Enter a file you'd like to create, quit() to choose another action, or enter /h  for help: ");
                scanf("%s",input); 
        if(!strcmp(input,"/h"))
        {
           memset(input, 0, sizeof(input));
            printf("The Create File function is used to create a new empty file.\n%s%s%s%s",
                "\nTo use this function a filename  and extension must be entered and a new file will be made in the local directory.",
                "\nEXAMPLE: file01.log\nEXAMPLE: file02.txt",
                "\nThe filename must be different from all filenames within the directory, and must not include any restricted characters.",
                "\nRestricted characters for this system: ");
            for (int i = 0; i < sizeRestrictedCharArray; i++)
                printf("%c ", restrictedChar[i]);
            printf("%s%c","\nIt also must not include the path separator for this system: ",pathSeparator);
            printf("%s","\nLastly, you must have permission to read and write in the directory you're choosing to create the file in.\n\n");
            fileCheck = 1;
            
            
        }else if(!strcmp(input, "quit()"))
        {
            memset(input, 0, sizeof(input));
            return EXIT_SUCCESS;
        }else
        {

            for(int i = 0 ;i < sizeRestrictedCharArray; i++)
            {
                for(int j = 0; j < sizeof(input)/sizeof(char); j++)
                {
                    if (input[j] == restrictedChar[i])
                    {
                        printf("%s","You've used an illegal character in your filename.\nPick a new name and try again.\n\n");
                        memset(input, 0, sizeof(input));
                    }
                }
            }
        }
    }
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

    memset(input, 0, sizeof(input));
}
    return EXIT_SUCCESS;
}
    

#include "client.h"

// Function: Prints a customized error message along with errno if an error occurs
// Arguments: An integer representing an err value and a string representing the customized error message
// Return Values: None
void print_error(int err, char *msg) {
    // Checking If There Is An Error
    if (err == -1) {
        // Printing Program Error Message
        printf("Error: %s\n", msg);

        // Printing Errno
        printf("%s\n", strerror(errno));
    }

    // Exiting Function
    return;
}

// Function: Removes whitespace from a string
// Arguments: The string to be trimmed.
// Return Values: None
void trim(char *s) {
    // Variable Declarations
    char *p = s;
    int l = strlen(p);

    // Trimming Off End
    while(isspace(p[l - 1])) p[--l] = 0;

    // Trimming Off Beginning
    while(*p && isspace(* p)) ++p, --l;

    // Changing Memory
    memmove(s, p, l + 1);
}

// Function: A function that gets input from the terminal
// Arguments: The string where the input should be copied to
// Return Values: The string that has the new data
char *get_input(char *buff) {
    // Reading In User Input
    if (fgets(buff, HARMONY_BUFFER_SIZE, stdin) == NULL) return "";

    // End String Properly If Needed
    *strchr(buff, '\n') = '\0';

    // Trimming String
    trim(buff);

    // Exiting Function
    return buff;
}

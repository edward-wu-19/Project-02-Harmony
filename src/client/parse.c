#include "client.h"

// Removes Whitespace From A String
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

// Gets Input From The Terminal
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

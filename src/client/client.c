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

// Main Function
int main() {
    // Creating Message Queue
    struct harmony_queue *Q = create_queue();

    // Creating Buffer
    char *buff = calloc(2*HARMONY_BUFFER_SIZE, sizeof(char));

    // Variable Declaration
    int cmd;

    while (1) {
        // Printing Screen
        print_screen(Q);

        // Getting Input
        buff = get_input(buff);

        // If String Is Empty Don't Do Anything
        if (strlen(buff) == 0) continue;

        if ((cmd = check_command(buff))) { // If Command Run It
            run_command(cmd);
        } else { // Else Update Message Queue
            update_queue(Q, buff);
        }
    }
}

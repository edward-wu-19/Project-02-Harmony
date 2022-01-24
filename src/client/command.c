#include "client.h"

// Global Variables
struct harmony_queue *Q;
char *buff, *usr;
struct harmony_message *data;
int cmd, server_socket, maxfd;
fd_set init, cpy;
char* harmony_help_message = "Harmony Help: \n\
    --exit : Terminates the program. \n\
    --help : Returns this message. \n\
    --quit : Terminates the program.\n\
    --rename : Changes your username. \n\
Only you can see this message.";
char* harmony_rename_message;

// Function: Checks whether a given input is a command
// Arguments: The string input that is being checked
// Return Values: An integer that represents the id of the command or 0 if it isnt a command
int check_command(char *buff) {
    // Checking With Command List
    if (strcmp(buff, "--quit") == 0 || strcmp(buff, "--exit") == 0) return 1;
    if (strcmp(buff, "--help") == 0) return 2;
    if (strcmp(buff, "--rename") == 0) return 3;

    // Exiting Function
    return 0;
}

// Function: A helper function that runs commands based on the command id
// Arguments: An integer that represents the command id
// Return Values: None
void run_command(int cmd) {
    // Running Commands
    if (cmd == 1) harmony_exit();
    if (cmd == 2) harmony_help();
    if (cmd == 3) harmony_rename();
}

// Function: A function that represents the --quit and --exit command and shuts down the client
// Arguments: None
// Return Values: None
void harmony_exit() {
    // Exiting Program
    clear_screen();
    client_exit();
}

// Function: A function that represents the --help command: sends a server message describing possible commands
// Arguments: None
// Return Values: None
void harmony_help() {
    // Creating Message
    data = calloc(1, sizeof(struct harmony_message));
    data = new_node(harmony_help_message, "Server", 0, get_time(), 0);

    // Adding To Queue
    update_queue(Q, data);
}

// Function: A function that represents the --rename command: allows user to type a new username and replaces the older username
// Arguments: None
// Return Values: None
void harmony_rename() {
    // Saving Old Username
    char* oldusr = calloc(HARMONY_USERNAME_SIZE, sizeof(char));
    strcpy(oldusr, usr);

    // Allocating Variables
    data = calloc(1, sizeof(struct harmony_message));
    harmony_rename_message = calloc(HARMONY_BUFFER_SIZE, sizeof(char));

    // Asking For Username
    usr = pick_name();

    // Updating Screen
    print_screen(Q);

    // Making Response Message
    strcpy(harmony_rename_message, oldusr);
    strcat(harmony_rename_message, " has renamed themselves to ");
    strcat(harmony_rename_message, usr);
    strcat(harmony_rename_message, ".");

    // Making Data Node
    data = new_node(harmony_rename_message, "", 0, get_time(), 0);

    // Sending Data
    int err1 = write(server_socket, data, sizeof(struct harmony_message));
    if (err1 == -1) {
        print_error(-1, "Client: Unable To Send Data To Server");
    }
}

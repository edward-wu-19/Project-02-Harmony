#include "client.h"

// Global Variables
char* harmony_help_message = "Harmony Help: \n\
    --exit : Terminates the program. \n\
    --help : Returns this message. \n\
    --quit : Terminates the program.\n\
    --rename : Changes your username. \n\
    --mute : Blocks you from seeing a user's messages. \n\
    --unmute : Unblocks you from seeing a user's messages. \n\
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
    if (strcmp(buff, "--mute") == 0) return 4;
    if (strcmp(buff, "--unmute") == 0) return 5;

    // Exiting Function
    return 0;
}

// Function: A helper function that runs commands based on the command id
// Arguments: An integer that represents the command id and the values that will be used in those commands
// Return Values: None
void run_command(int cmd, struct harmony_queue *Q, char *usr, int server_socket, int *mute) {
    // Running Commands
    if (cmd == 1) harmony_exit();
    if (cmd == 2) harmony_help(Q, mute);
    if (cmd == 3) harmony_rename(Q, usr, server_socket);
    if (cmd == 4) harmony_mute(mute);
    if (cmd == 5) harmony_unmute(mute);
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
// Arguments: The harmony_queue to which the help message will be added to
// Return Values: None
void harmony_help(struct harmony_queue *Q, int *mute) {
    // Creating Message
    struct harmony_message *data = calloc(1, sizeof(struct harmony_message));
    data = new_node(harmony_help_message, "Server", 0, get_time(), 0);

    // Adding To Queue
    update_queue(Q, data, mute);

    // Exiting Function
    free(data);
    return;
}

// Function: A function that represents the --rename command: allows user to type a new username and replaces the older username
// Arguments: The harmony_queue where the server message is going to, the original username, and the server_socket currently connected to
// Return Values: None
void harmony_rename(struct harmony_queue *Q, char *usr, int server_socket) {
    // Saving Old Username
    char* oldusr = calloc(HARMONY_USERNAME_SIZE, sizeof(char));
    strcpy(oldusr, usr);

    // Allocating Variables
    struct harmony_message *data = calloc(1, sizeof(struct harmony_message));
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

    // Exiting Function
    free(data);
    return;
}

// Function: Represents the --mute command: allows user to block messages from a certain user id
// Arguments: The mute array which represents which users are muted (1) or not (0).
// Return Values: None
void harmony_mute(int *mute) {
    // Variable Declarations
    char *str1 = calloc(HARMONY_USERNAME_SIZE, sizeof(char)), str2[HARMONY_BUFFER_SIZE] = "", temp[20];
    int usr, err, done = 0;

    while (!done) {
        // Setting Up Screen
        clear_screen();
        printf("%s\n", str2);
        printf("Please Enter A User Id To Mute: ");

        // Getting User Input
        str1 = get_input(str1);

        // Attempting To Convert Input Into An Integer
        err = sscanf(str1, "%d", &usr);
        if (err == 0) {
            strncpy(str2, "Input needs to be a valid number.", 33);
            continue;
        }

        // Making Sure Number Is In Bounds
        if (usr < 0 || usr >= HARMONY_QUEUE_SIZE) {
            // Making Error Message
            strncpy(str2, "Number must be between 0 and ", 30);
            sprintf(temp, "%d", HARMONY_QUEUE_SIZE-1);
            strncat(str2, temp, 20);
            strncat(str2, strdup("."), 1);
            continue;
        } else done = 1; // Ending Loop
    }

    // Exiting Function
    mute[usr] = 1;
    free(str1);
    return;
}

// Function: Represents the --unmute command: allows user to unblock messages from a certain user id
// Arguments: The mute array which represents which users are muted (1) or not (0).
// Return Values: None
void harmony_unmute(int *mute) {
    // Variable Declarations
    char *str1 = calloc(HARMONY_USERNAME_SIZE, sizeof(char)), str2[HARMONY_BUFFER_SIZE] = "", temp[20];
    int usr, err, done = 0;

    while (!done) {
        // Setting Up Screen
        clear_screen();
        printf("%s\n", str2);
        printf("Please Enter A User Id To Unmute: ");

        // Getting User Input
        str1 = get_input(str1);

        // Attempting To Convert Input Into An Integer
        err = sscanf(str1, "%d", &usr);
        if (err == 0) {
            strncpy(str2, "Input needs to be a valid number.", 33);
            continue;
        }

        // Making Sure Number Is In Bounds
        if (usr < 0 || usr >= HARMONY_QUEUE_SIZE) {
            // Making Error Message
            strncpy(str2, "Number must be between 0 and ", 30);
            sprintf(temp, "%d", HARMONY_QUEUE_SIZE-1);
            strncat(str2, temp, 20);
            strncat(str2, strdup("."), 1);
            continue;
        } else done = 1; // Ending Loop
    }

    // Exiting Function
    mute[usr] = 0;
    free(str1);
    return;
}

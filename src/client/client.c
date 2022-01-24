#include "client.h"

// Global Variables
struct harmony_queue *Q;
char *buff, *usr, *ip;
struct harmony_message *data;
int cmd, server_socket, maxfd, chn = 0;
fd_set init, cpy;


// Function: A function that properly ends the client program with ending messages and freeing data
// Arguments: None
// Return Values: None
void client_exit() {
    // Generating Exit Message
    strcpy(buff, usr);
    strcat(buff, " has left the channel!");
    data = new_node(buff, "", 0, get_time(), 0);

    // Sending Data
    int err1 = write(server_socket, data, sizeof(struct harmony_message));
    if (err1 == -1) {
        print_error(-1, "Client: Unable To Send Data To Server");
    }

    // Closing Socket
    int err = close(server_socket);
    if (err == -1) {
        print_error(-1, "Client: Unable To Close Server Socket");
        exit(-1);
    }

    // Freeing Stuff
    free(buff);
    free(usr);
    free(data);
    free(ip);
    free_queue(Q);

    // Ending Program
    printf("Client: Successfully Ended Session\n");
    printf("\nThank You For Using Harmony (Made By Mohammad Khan And Edward Wu)\n");
    exit(0);
}

// Function: Catches signals in the client program
// Arguments: An integer that represents the signal that is caught
// Return Values: None
static void sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) {
        // Exiting Function
        clear_screen();
        client_exit();
    }

    // Exiting Function
    return;
}

// Function: A function that asks the user for a username and sanitizes the username a bit
// Arguments: None
// Return Values: A string that represents the username typed
char* pick_name() {
    // Declaring Variables
    usr = calloc(HARMONY_USERNAME_SIZE, sizeof(char));
    int appropriate_name = 0;
    char* name_message = "";

    // Sanitizing Input
    while (!appropriate_name) {
        clear_screen();
        printf("%s\n", name_message);
        printf("Please Enter A Username: ");
        usr = get_input(usr);
        if (strcmp(usr, "") == 0){
            name_message = "Your username cannot be empty.";
        } else if (strcmp(usr, "Server") == 0){
            name_message = "Your username cannot be Server.";
        } else if (strcmp(usr, "server") == 0){
            name_message = "Your username cannot be server.";
        } else {
            appropriate_name = 1;
        }
    }

    // Exiting Function
    return usr;
}

// Function: The main function that represents the running of the entire program
// Arguments: argument count and the a singular argument that represents the desired ip address
// Return Values: Regular main output
int main(int argc, char **argv) {
    // Signal Handling
    signal(SIGINT, sighandler);

    // Variable Declarations
    Q = create_queue();
    buff = calloc(HARMONY_BUFFER_SIZE, sizeof(char));
    usr = calloc(HARMONY_USERNAME_SIZE, sizeof(char));
    data = calloc(1, sizeof(struct harmony_message));

    // Asking For Username
    usr = pick_name();

    // Completing Handshake
    if (argc == 2) server_socket = client_handshake(argv[1]);
    else server_socket = client_handshake(HARMONY_IP);
    if (server_socket == -1) {
        print_error(-1, "Client: Unable To Connect To Server");
        return -1;
    }

    // Setting Up File Descriptor Set
    maxfd = server_socket;
    FD_ZERO(&init);
    FD_SET(server_socket, &init);
    FD_SET(0, &init);

    // Generating Join Message
    strcpy(buff, usr);
    strcat(buff, " has joined the channel!");
    data = new_node(buff, "", 0, get_time(), 0);

    // Sending Data
    int err1 = write(server_socket, data, sizeof(struct harmony_message));
    if (err1 == -1) {
        print_error(-1, "Client: Unable To Send Data To Server");
    }

    while (1) {
        // Printing Screen
        print_screen(Q);

        // Making Copy Of Clients
        cpy = init;

        // Running Select
        int err = select(maxfd+1, &cpy, NULL, NULL, NULL);
        if (err == -1) {
            print_error(-1, "Client: Unable To Select");
        }

        // Checking From STDIN
        if (FD_ISSET(0, &cpy)) {
            // Getting STDIN Input
            buff = get_input(buff);

            // If String Is Empty Don't Do Anything
            if (strlen(buff) == 0) continue;

            if ((cmd = check_command(buff))) { // If Command Run It
                run_command(cmd);
            } else { // Else Write To Server
                // Creating Message Struct
                data = new_node(buff, usr, chn, get_time(), 0);

                // Sending Data
                int err1 = write(server_socket, data, sizeof(struct harmony_message));
                if (err1 == -1) {
                    print_error(-1, "Client: Unable To Send Data To Server");
                }
            }
        }

        // Checking From Server
        if (FD_ISSET(server_socket, &cpy)) {
            // Reading From Server
            int err2 = read(server_socket, data, sizeof(struct harmony_message));
            if (err2 == -1) {
                print_error(-1, "Client: Unable To Read Data From Server");
            }

            // If Empty End Client
            if (err2 == 0) {
                clear_screen();
                client_exit();
            }

            // Updating Message Queue
            update_queue(Q, data);
        }
    }

    // Ending Program
    clear_screen();
    client_exit();
}

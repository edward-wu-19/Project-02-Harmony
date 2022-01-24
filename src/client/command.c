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
Only you can see this message.\n";

int check_command(char *buff) {
    // Checking With Command List
    if (strcmp(buff, "--quit") == 0 || strcmp(buff, "--exit") == 0) return 1;
    if (strcmp(buff, "--help") == 0) return 2;
    if (strcmp(buff, "--rename") == 0) return 3;

    // Exiting Function
    return 0;
}

void run_command(int cmd) {
    // Running Commands
    if (cmd == 1) harmony_exit();
    if (cmd == 2) harmony_help();
    if (cmd == 3) harmony_rename();
}

void harmony_exit() {
    // Exiting Program
    clear_screen();
    client_exit();
    exit(0);
}

void harmony_help() {
    // Creating Message
    data = calloc(1, sizeof(struct harmony_message));
    data = new_node(harmony_help_message, "Server", 0, get_time(), 0);

    // Adding To Queue
    update_queue(Q, data);
}

void harmony_rename() {
    // *char harmony_rename = "Please enter your new name: ";
    // usr = get_input(usr);

    // int err3 = write(fd, harmony_help, sizeof(struct harmony_message));
    //     if (err3 == -1) {
    //         print_error(-1, "Server: Unable To Send Help");
    //         continue;
    //     }
}

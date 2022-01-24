#include "client.h"

// Global Variables
struct harmony_queue *Q;
char *buff, *usr;
struct harmony_message *data;
int cmd, server_socket, maxfd;
fd_set init, cpy;

int check_command(char *buff) {
    // Checking With Command List
    if (strcmp(buff, "--quit") == 0 || strcmp(buff, "--exit") == 0) return 1;
    // if (strcmp(buff, "--help") == 0) return 2;

    // Exiting Function
    return 0;
}

void run_command(int cmd) {
    // Running Commands
    if (cmd == 1) harmony_exit();
    // if (cmd == 2) harmony_help();
}

void harmony_exit() {
    // Clearing screen
    clear_screen();

    // Printing Goodbye Message
    printf("Thank You For Using Harmony (Made By Mohammad Khan And Edward Wu)\n");

    // Exiting Program
    exit(0);
}

// void harmony_help(){
//     buff = calloc(HARMONY_BUFFER_SIZE, sizeof(char));
//     usr = calloc(HARMONY_USERNAME_SIZE, sizeof(char));
//     data = calloc(1, sizeof(struct harmony_message));

//     data = new_node(buff, usr, chn, get_time(), 0);

//     int err1 = write(server_socket, data, sizeof(struct harmony_message));
//     if (err1 == -1) {
//         print_error(-1, "Client; Unable To Send Data To Server");
//     }

//     printf("""Server: \n\
//     --exit : Terminates the program.\n\
//     --help : Returns this message.\n\
//     --quit : Terminates the program.\n""");
// }
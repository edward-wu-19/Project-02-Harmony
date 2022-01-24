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
<<<<<<< HEAD

char* harmony_rename_message;
=======
>>>>>>> 3c98a879984f352635e7313ce12b125dfde25a64

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

void harmony_rename(){
    char* oldusr;
    strcpy(oldusr, usr); 

    data = calloc(1, sizeof(struct harmony_message));
    harmony_rename_message = calloc(HARMONY_USERNAME_SIZE, sizeof(char));

    // Asking For Username And Color
    clear_screen();
    printf("Please Enter A New Username: ");
    usr = get_input(usr);

    print_screen(Q);

    strcpy(harmony_rename_message, oldusr);
    strcat(harmony_rename_message, " has renamed themselves to ");
    strcat(harmony_rename_message, usr);
    strcat(harmony_rename_message, ".");

    printf("%s\n", harmony_rename_message);

    data = new_node(harmony_rename_message, "Server", 0, get_time(), 0);

    // Sending Data
    int err1 = write(server_socket, data, sizeof(struct harmony_message));
    if (err1 == -1) {
        print_error(-1, "Client: Unable To Send Data To Server");
    }
}

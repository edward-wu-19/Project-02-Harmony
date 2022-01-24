#include "client.h"

// Global Variables
struct harmony_queue *Q;
char *buff, *usr;
struct harmony_message *data;
int cmd, server_socket, maxfd, chn = 0;
fd_set init, cpy;

void client_exit() {
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
    free_queue(Q);

    // Ending Program
    printf("Client: Successfully Shut Down\n");
    printf("Thank You For Using Harmony\n");
    exit(0);
}

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

// Main Function
int main() {
    // Signal Handling
    signal(SIGINT, sighandler);

    // Variable Declarations
    Q = create_queue();
    buff = calloc(HARMONY_BUFFER_SIZE, sizeof(char));
    usr = calloc(HARMONY_USERNAME_SIZE, sizeof(char));
    data = calloc(1, sizeof(struct harmony_message));

    // Asking For Username And Color
    clear_screen();
    printf("Please Enter A Username: ");
    usr = get_input(usr);

    // Completing Handshake
    server_socket = client_handshake();
    if (server_socket == -1) {
        print_error(-1, "Client: Unable To Connect To Server");
        return -1;
    }

    // Setting Up File Descriptor Set
    maxfd = server_socket;
    FD_ZERO(&init);
    FD_SET(server_socket, &init);
    FD_SET(0, &init);

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
                    print_error(-1, "Client; Unable To Send Data To Server");
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

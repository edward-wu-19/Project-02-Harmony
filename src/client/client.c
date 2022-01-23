#include "client.h"

// Main Function
int main() {
    // Variable Declarations
    struct harmony_queue *Q = create_queue();
    char *buff = calloc(HARMONY_BUFFER_SIZE, sizeof(char));
    int cmd, server_socket, maxfd;
    fd_set init, cpy;

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

        // Checking Which File Descriptor Input Is From
        if (FD_ISSET(0, &cpy)) {
            // Getting STDIN Input
            buff = get_input(buff);

            // If String Is Empty Don't Do Anything
            if (strlen(buff) == 0) continue;

            if ((cmd = check_command(buff))) { // If Command Run It
                run_command(cmd);
            } else { // Else Write To Server
                int err1 = write(server_socket, buff, sizeof(buff));
                if (err1 == -1) {
                    print_error(-1, "Client; Unable To Send Data To Server");
                }
            }
        } else {
            // Reading From Server
            int err2 = read(server_socket, buff, sizeof(buff));
            if (err2 == -1) {
                print_error(-1, "Client: Unable To Read Data From Server");
            }

            // If Empty End Client
            if (err2 == 0) {
                close(server_socket);
                return 0;
            }

            // Updating Message Queue
            update_queue(Q, buff);
        }
    }

    close(server_socket);
    return 0;
}

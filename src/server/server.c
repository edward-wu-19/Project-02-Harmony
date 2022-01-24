#include "server.h"

// Global Variables
struct harmony_message *data;
int fd, listen_socket, maxfd = 0, client, itr;
fd_set init, cpy;

void server_exit() {
    // Closing All Sockets
    for (itr = 3; itr <= maxfd; itr++) {
        // Checking If Socket Is Valid
        if (fcntl(itr, F_GETFL) == -1) continue;

        // Closing Socket
        int err = close(itr);
        if (err == -1) {
            print_error(-1, "Server: Unable To Close Client Socket");
            continue;
        }
    }

    // Freeing Data
    free(data);

    // Exiting Function
    printf("Server: Successfully Shut Down\n");
    printf("Thank You For Using Harmony (Made By Mohammad Khan And Edward Wu)\n");
    exit(0);
}

static void sighandler(int signo) {
    // SIGINT Case
    if (signo == SIGINT) {
        // Exiting Normally
        server_exit();
    }

    // Exiting Function
    return;
}

int main() {
    // Signal Handling
    signal(SIGINT, sighandler);

    // Variable Declarations
    data = calloc(1, sizeof(struct harmony_message));

    // Initializing Original FD Set
    FD_ZERO(&init);
    listen_socket = server_setup();
    FD_SET(listen_socket, &init);
    maxfd = listen_socket;

    while (1) {
        // Making Copy Of Clients
        cpy = init;

        // Running Select
        int err = select(maxfd+1, &cpy, NULL, NULL, NULL);
        if (err == -1) {
            print_error(-1, "Server: Unable To Select");
        }

        // Looping Through File Descriptors
        for (fd = 0; fd <= maxfd; fd++) {

            // Checking If File Descriptor Has Input
            if (FD_ISSET(fd, &cpy)) {

                if (fd == listen_socket) {
                    // Accepting New Client
                    client = server_handshake(listen_socket);
                    FD_SET(client, &init);
                    if (client > maxfd) maxfd = client;
                } else {
                    // Get Message From Existing Client
                    int err1 = read(fd, data, sizeof(struct harmony_message));
                    if (err1 == -1) {
                        print_error(-1, "Server: Unable To Read Data");
                        continue;
                    }

                    // If Data Is Empty Remove Client
                    if (err1 == 0) {
                        FD_CLR(fd, &init);
                        int err2 = close(fd);
                        if (err2 == -1) {
                            print_error(-1, "Server: Unable To Close File Descriptor");
                            continue;
                        }
                    } else {
                        // Updating Message Struct
                        if (strcmp(data->sender, "Server") != 0){
                            data->id = fd;
                        }

                        // Send Data To All Other Clients
                        for (itr = 4; itr <= maxfd; itr++) {
                            // Checking If File Descriptor Is Valid
                            if (fcntl(itr, F_GETFL) == -1) continue;

                            // Sending Data
                            int err3 = write(itr, data, sizeof(struct harmony_message));
                            if (err3 == -1) {
                                print_error(-1, "Server: Unable To Write Data");
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }

    server_exit();
}

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
    printf("Thank You For Using Harmony\n");
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

    // Initializing Help Message
    char* harmony_help = """Harmony Help: \n\
    --exit : Terminates the program. \n\
    --help : Returns this message. \n\
    --quit : Terminates the program.\n\
    Only you can see this message.\n""";

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
                        data->id = fd;

                        // --help case
                        if (strcmp(data->val, "--help") == 0){
                            int err3 = write(fd, harmony_help, sizeof(struct harmony_message));
                            if (err3 == -1) {
                                print_error(-1, "Server: Unable To Send Help");
                                continue;
                            }
                        }

                        else{
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
    }

    server_exit();
    return 0;
}

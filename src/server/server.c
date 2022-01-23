#include "server.h"

int main() {
    // Variable Declarations
    int fd, listen_socket, maxfd, client;
    fd_set init, cpy;
    char *buff = calloc(HARMONY_BUFFER_SIZE, sizeof(char));

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
                    int err1 = read(fd, buff, sizeof(buff));
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
                    } else { // Send Data To All Other Clients
                        int err3 = write(fd, buff, sizeof(buff));
                        if (err3 == -1) {
                            print_error(-1, "Server: Unable To Write Data");
                            continue;
                        }
                    }
                }
            }
        }
    }

    close(listen_socket);
    return 0;
}

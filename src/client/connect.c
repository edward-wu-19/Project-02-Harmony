#include "client.h"

int client_handshake(char *ip) {
    // Setting Up Hints Struct
    struct addrinfo *hints;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET; // IPv4
    hints->ai_socktype = SOCK_STREAM; // TCP Socket

    // Getting IP Information
    struct addrinfo *results;
    int err1 = getaddrinfo(ip, HARMONY_PORT, hints, &results);
    if (err1 == -1) {
        print_error(-1, "Client: Unable To Get IP Information");
        return -1;
    }

    // Create Socket
    int server_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (server_socket == -1) {
        print_error(-1, "Client: Unable To Create Socket");
        return -1;
    }

    // Connecting To Server
    int err2 = connect(server_socket, results->ai_addr, results->ai_addrlen);
    if (err2 == -1) {
        print_error(-1, "Client: Unable To Connect To Server");
        return -1;
    }

    // Exiting Function
    free(hints);
    freeaddrinfo(results);
    return server_socket;
}

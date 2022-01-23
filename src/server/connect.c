#include "server.h"

// Function: Prints a customized error message along with errno if an error occurs
// Arguments: An integer representing an err value and a string representing the customized error message
// Return Values: None
void print_error(int err, char *msg) {
    // Checking If There Is An Error
    if (err == -1) {
        // Printing Program Error Message
        printf("Error: %s\n", msg);

        // Printing Errno
        printf("%s\n", strerror(errno));
    }

    // Exiting Function
    return;
}

int server_setup() {
    // Setting Up Hints Struct
    struct addrinfo *hints;
    hints = calloc(1, sizeof(struct addrinfo));
    hints->ai_family = AF_INET; // IPv4
    hints->ai_socktype = SOCK_STREAM; // TCP Socket
    hints->ai_flags = AI_PASSIVE; // Only Needed On Server

    // Getting IP Information
    struct addrinfo *results;
    int err1 = getaddrinfo(NULL, HARMONY_PORT, hints, &results);  // Server Sets Node To NULL
    if (err1 == -1) {
        print_error(-1, "Server: Unable To Get IP Information");
        return -1;
    } else printf("Server: Successfully Got IP Information\n");

    // Create Socket
    int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    if (listen_socket == -1) {
        print_error(-1, "Server: Unable To Create Socket");
        return -1;
    } else printf("Server: Successfully Created Socket\n");

    // Binding Socket
    int err2 = bind(listen_socket, results->ai_addr, results->ai_addrlen);
    if (err2 == -1) {
        print_error(-1, "Server: Unable To Bind Socket");
        return -1;
    } else printf("Server: Successfully Binded Socket\n");

    // Setting Up Listen
    int err3 = listen(listen_socket, 10);
    if (err3 == -1) {
        print_error(-1, "Server: Unable To Set Up Listen");
        return -1;
    } else printf("Server: Successfully Set Up Listen\n");

    // Exiting Function
    free(hints);
    freeaddrinfo(results);
    return listen_socket;
}

int server_handshake(int listen_socket) {
    // Variable Declarations
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    // Accepting Connection
    client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &sock_size);
    if (client_socket == -1) {
        print_error(-1, "Server: Unable To Accept Connection");
        return -1;
    } else printf("Server: Successfully Accepted Connection\n");

    // Exiting Function
    return client_socket;
}

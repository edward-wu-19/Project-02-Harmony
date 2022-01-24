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

// Function: Sets up the listen socket of the server
// Arguments: None
// Return Values: The file descriptor of the listening socket
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

    // Printing IP Address For Usage
    char *s = calloc(HARMONY_USERNAME_SIZE, sizeof(char));
    s = ip_to_string(results->ai_addr, s, HARMONY_USERNAME_SIZE);
    printf("Server: IP Address To Share - %s\n", s);

    // Setting Up Listen
    int err3 = listen(listen_socket, 10);
    if (err3 == -1) {
        print_error(-1, "Server: Unable To Set Up Listen");
        return -1;
    } else printf("Server: Successfully Set Up Listen\n");

    // Exiting Function
    free(hints);
    freeaddrinfo(results);
    free(s);
    return listen_socket;
}

// Function: A function that completes the connection between the client and the server
// Arguments: The listening socket file descriptor
// Return Values: An integer representing the client socket
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

// Function: A function that gets an IP address in a readable format
// Arguments: The sockaddr struct, the string to store the address, and the max length of the string
// Return Values: The string that stores the address
char *ip_to_string(const struct sockaddr *sa, char *s, int len) {
    if (sa->sa_family == AF_INET) { // IPv4 Case
        inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, len);
    } else if (sa->sa_family == AF_INET6) { // IPv6 Case
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, len);
    } else { // Error Case
        strncpy(s, "Server: Unable To Get IP Address\n", len);
    }
    return s;
}

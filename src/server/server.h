#ifndef SERVER

// Preventing Duplicates
#define SERVER

// Library Imports
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

// Color Constants (Not Sure About Window Compatability)
#define HARMONY_BLACK "\x1B[30m"
#define HARMONY_RED "\x1B[31m"
#define HARMONY_GREEN "\x1B[32m"
#define HARMONY_YELLOW "\x1B[33m"
#define HARMONY_BLUE "\x1B[34m"
#define HARMONY_MAGENTA "\x1B[35m"
#define HARMONY_CYAN "\x1B[36m"
#define HARMONY_WHITE "\x1B[37m"
#define HARMONY_RESET "\x1B[0m"

// Number Constants
#define HARMONY_BUFFER_SIZE 2048
#define HARMONY_QUEUE_SIZE 128
#define HARMONY_USERNAME_SIZE 32
#define HARMONY_PORT "9002"
#define HARMONY_TEST_IP "127.0.0.1"
#define HARMONY_IP "159.89.234.51"

// connect.c
void print_error(int err, char *msg);
int server_setup();
int server_handshake(int listen_socket);
struct harmony_message {
    char val[HARMONY_BUFFER_SIZE], sender[HARMONY_USERNAME_SIZE], time[HARMONY_USERNAME_SIZE];
    struct harmony_message *next;
    int channel, id;
};

#endif

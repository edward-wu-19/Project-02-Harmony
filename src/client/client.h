#ifndef CLIENT

// Preventing Duplicates
#define CLIENT

// Library Imports
#include <time.h>
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
#define HARMONY_IP "0.0.0.0"

// client.c
void client_exit();

// command.c
int check_command(char *buff);
void run_command(int cmd);
void harmony_exit();
void harmony_help();

// connect.c
int client_handshake();

// parse.c
void print_error(int err, char *msg);
void trim(char *p);
char *get_input(char *buff);

// queue.c
struct harmony_message {
    char val[HARMONY_BUFFER_SIZE], sender[HARMONY_USERNAME_SIZE], time[HARMONY_USERNAME_SIZE];
    struct harmony_message *next;
    int channel, id;
};
struct harmony_queue {
    int size;
    struct harmony_message *front, *back;
};
struct harmony_message *new_node(char *msg, char* usr, int chn, char* time, int id);
struct harmony_queue *create_queue();
void queue_push(struct harmony_queue *Q, char *msg, char *usr, int chn, int id);
void queue_pop(struct harmony_queue *Q);
void update_queue(struct harmony_queue *Q, struct harmony_message *data);
void free_queue(struct harmony_queue *Q);

// screen.c
#define max(a, b) (a >= b ? a : b)
#define min(a, b) (a <= b ? a : b)
void clear_screen();
struct winsize *get_terminal_size();
void print_screen(struct harmony_queue *Q);
char* get_time();

#endif

#include "client.h"

// Clears The Terminal Screen
void clear_screen() {
    // ANSI Escape Codes (Not Sure About Window Compatability)
    // \x1B[H Moves Cursor To Home Position
    // \x1B[2J Clears The Screen
    // \x1B[3J Clears Scroll Back
    printf("\x1B[H\x1B[2J\x1B[3J");

    // Exiting Function
    return;
}

// Gets Terminal Size
struct winsize *get_terminal_size() {
    // Obtaining Window Size
    struct winsize *w = calloc(1, sizeof(struct winsize *));
    int err = ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
    if (err == -1) {
        print_error(-1, "Client: Unable To Get Terminal Size");
        return NULL;
    }
    return w;
}

// Prints The Terminal Screen
void print_screen(struct harmony_queue *Q) {
    // Clearing Screen
    clear_screen();

    // Obtaining Terminal Size And Starting Node
    struct winsize *w = get_terminal_size();
    int height = w->ws_row, i = 0;
    struct harmony_message *node = Q->front;

    // Printing Blank Lines If Needed
    for (; i < max(0, height-(Q->size)-1); i++) printf("\n");

    // Printing Out Actual Messages
    while (node != NULL) {
        printf("%s\n\n", node->val);
        node = node->next;
    }

    // Exiting Functions
    printf("Type Here: ");
    return;
}

char* get_time(){
    // Getting Current Time
    time_t t = time(NULL);

    // Converting Time To String
    char *tm = ctime(&t);

    // Error Checking
    if (tm == NULL) print_error(-1, "Unable To Get Time");
    else return tm;
}
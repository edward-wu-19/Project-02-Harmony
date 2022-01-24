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
    int height = w->ws_row, i = 0, width = w->ws_col, j = 0;
    struct harmony_message *node = Q->front, *prev = Q->front;
    //
    // Printing Blank Lines If Needed
    for (; i < max(0, height-(Q->size)-2); i++) printf("\n");

    // Printing Out Actual Messages
    while (node != NULL) {
        if (prev->id == node->id && j != 0) printf("\n%s", node->val);
        else printf("\n\n%s (id:%03d) - %s\n%s", node->sender, node->id, node->time, node->val);
        prev = node;
        node = node->next;
        if (j == 0) j++;
    }

    // Spacing Out Messages
    printf("\n");
    for (i = 0; i < width; i++) {
        printf("-");
    }
    printf("\n");

    // Exiting Functions
    return;
}

char* get_time() {
    // Getting Current Time
    time_t t = time(NULL);

    // Converting Time To String
    char *tm = ctime(&t);

    // Error Checking
    if (tm == NULL) print_error(-1, "Unable To Get Time");

    // Getting Rid Of Extraneous Information
    tm = tm+4;

    // Ending To Get Rid Of Extraneous Information
    *(strchr(tm, '\n')-8) = '\0';

    // Exiting Function
    return tm;
}

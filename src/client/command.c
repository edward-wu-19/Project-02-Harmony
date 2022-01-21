#include "client.h"

int check_command(char *buff) {
    // Checking With Command List
    if (strcmp(buff, "--quit") == 0 || strcmp(buff, "--exit") == 0) return 1;
    if (strcmp(buff, "--help") == 0) return 2;

    // Exiting Function
    return 0;
}

void run_command(int cmd) {
    // Running Commands
    if (cmd == 1) harmony_exit();
}

void harmony_exit() {
    // Clearing screen
    clear_screen();

    // Printing Goodbye Message
    printf("Thanks For Using Harmony (Made By Mohammad Khan And Edward Wu)\n");

    // Exiting Program
    exit(0);
}

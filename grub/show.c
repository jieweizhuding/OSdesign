#include"show.h"

void startup() {
    my_printf("Tiny OS -version 0.1\n");
    my_printf(" _____ _                ___  ____  \n");
    my_printf("|_   _(_)_ __  _   _   / _ \\/ ___| \n");
    my_printf("  | | | | '_ \\| | | | | | | \\___ \\ \n");
    my_printf("  | | | | | | | |_| | | |_| |___) |\n");
    my_printf("  |_| |_|_| |_|\\__, |  \\___/|____/ \n");
    my_printf("               |___/               \n");
}

void promptString(){
    my_printf("TinyOS #");
}

void man(){
    my_printf("clear  --clear the terminal");
    my_printf("poweroff  --close PC");
}
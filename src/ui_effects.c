#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui_effects.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static void milliSleep(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void displayStartupScreen() {
    printf("\n+==========================================================+\n");
    printf("|                                                          |\n");
    printf("|             SMART HOSPITAL SYSTEM                        |\n");
    printf("|                                                          |\n");
    printf("|             Initializing System...                       |\n");
    printf("|             [");
    fflush(stdout);

    int totalBlocks = 25;
    for (int i = 1; i <= totalBlocks; i++) {
        printf("=");
        fflush(stdout);
        milliSleep(40);
    }
    printf("] 100%%       |\n");
    printf("|                                                          |\n");
    printf("|                SYSTEM READY [OK]                         |\n");
    printf("|                                                          |\n");
    printf("+==========================================================+\n\n");
    
    milliSleep(400);
}

void drawBoxHeader(const char *title) {
    printf("\n+==========================================================+\n");
    printf("| %-56s |\n", title);
    printf("+==========================================================+\n");
}

void pauseConsole() {
    printf("\n[Press ENTER to return to menu...]");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

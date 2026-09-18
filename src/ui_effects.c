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

void showProgressBar(const char *label, int totalSteps, int stepDelayMs) {
    if (totalSteps <= 0) totalSteps = 20;
    if (stepDelayMs <= 0) stepDelayMs = 15;

    printf("\n%s [", label);
    fflush(stdout);
    for (int i = 1; i <= totalSteps; i++) {
        printf("=");
        fflush(stdout);
        milliSleep(stepDelayMs);
    }
    printf("] 100%%\n");
}

void showLoadingSpinner(const char *label, int durationMs) {
    static const char spinnerChars[] = "|/-\\";
    int frameDelay = 50;
    int totalFrames = durationMs / frameDelay;
    if (totalFrames <= 0) totalFrames = 8;

    printf("\n");
    for (int i = 0; i < totalFrames; i++) {
        printf("\r%-50s [%c]", label, spinnerChars[i % 4]);
        fflush(stdout);
        milliSleep(frameDelay);
    }
    printf("\r%-50s [OK]\n", label);
    fflush(stdout);
}

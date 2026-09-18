#ifndef UI_EFFECTS_H
#define UI_EFFECTS_H

void displayStartupScreen();
void drawBoxHeader(const char *title);
void pauseConsole();
void showProgressBar(const char *label, int totalSteps, int stepDelayMs);
void showLoadingSpinner(const char *label, int durationMs);

#endif

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

#include <stdbool.h>

#define VERSION 0.01

struct winsize w;

typedef struct Status
{
        int verPos;
        int horPos;
        bool mode;
        bool running;
        int undoBuf[1024]; // item at index 0, reserved for a number
        int rows;         // representing the last used index
        int cols;
        char drawBrush;
        char* fileName;
} Status;

const char* functionTitles[4] = {
        "TAB Switch-Mode",
        "^Z Undo-Action  ",
        "^W Write-to-File",
        "^X Quit        "
};

const char* drawingTitles[4] = {
        "TAB Switch-Mode",
        "^A Draw * symbol",
        "^S Draw # symbol",
        "^D Draw empty  "
};

Status executeInput(int ch, Status s, WINDOW* win);

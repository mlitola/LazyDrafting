#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "common.h"

void printFunctionsOrDrawing(int rows, int startPos, bool drawing)
{
        for (int i=0; i<4; ++i)
        {
                move(rows, startPos+(i*19));
                if (drawing)
                        printw("%s", drawingTitles[i]);
                else
                        printw("%s", functionTitles[i]);
        }
}

void printMode(bool mode, int rows)
{
        move(rows, 4);
        hline(ACS_HLINE, 10);

        printw("MODE:");
        if (mode)
        {
                printw("Draw ");
                printFunctionsOrDrawing(rows, 15, mode);
        }
        else
        {
                printw("Input");
                printFunctionsOrDrawing(rows, 15, mode);
        }
        move(rows, 9);
}

void clearAll(int cols, int rows)
{
        for (int i=2; i<rows-2; i++)
        {
                for (int j=2; j<cols-2; j++)
                {
                        move(i,j);
                        printw(" ");
                }
        }
        move(2,2);
}

void saveToFile(int cols, int rows, char* fileName)
{
        FILE *f;
        chtype* read_chr = (chtype*) malloc(2*sizeof(chtype));
        int i=2, j=2;

        if ((int)strlen(fileName) > 0)
        {
                f = fopen(fileName, "w");

                while (i < rows)
                {
                        while(j < cols)
                        {
                                mvinchnstr(i, j, read_chr, 1);
                                fputs(read_chr, f);
                                j++;
                        }
                        fputs("\n", f);
                        j=2;
                        i++;
                }

                fclose(f);
        }
        free(read_chr);
}

Status addUndoAction(int ch, Status s)
{
        int index = s.undoBuf[0];
        if (index < 1023)
        {
                s.undoBuf[0]++;
                index++;
        }
        s.undoBuf[index] = ch;

        return s;
}

Status doUndo(Status s, WINDOW *win, int cols, int rows)
{
        if (s.undoBuf[0] == 0)
                return s;

        int limit = s.undoBuf[0];
        s.undoBuf[0] = 0;

        for (int i = 1; i < limit; ++i)
                s = executeInput(s.undoBuf[i], s, win);

        return s;
}

void doDrawing(bool mode, char brush) {
        if (mode) // if mode == DRAW --> draw!!
                printw("%c", brush);
}

Status executeInput(int ch, Status s, WINDOW* win)
{
        if ((ch > 31 && ch < 262) || (ch > 7 && ch < 11)) {
                s = addUndoAction(ch, s);
        }

        switch (ch)
        {
                case KEY_UP:
                        doDrawing(s.mode, s.drawBrush);
                        if (s.verPos > 2)
                        {
                                move(--s.verPos, s.horPos);
                        }
                        break;

                case KEY_DOWN:
                        doDrawing(s.mode, s.drawBrush);
                        if (s.verPos < s.rows-3) {
                                move(++s.verPos, s.horPos);
                        }
                        break;

                case KEY_LEFT:
                        doDrawing(s.mode, s.drawBrush);
                        if (s.horPos > 2)
                        {
                                move(s.verPos, --s.horPos);
                        }
                        break;

                case KEY_RIGHT:
                        doDrawing(s.mode, s.drawBrush);
                        if (s.horPos < s.cols-2)
                        {
                                move(s.verPos, ++s.horPos);
                        }
                        break;

                case 9: // KEY_STAB
                        s.mode = !s.mode;
                        printMode(s.mode, s.rows-1);
                        move(s.verPos, s.horPos);
                        break;

                case CTRL('z'): // undo
                        if (s.undoBuf[0] > 0)
                        {
                                clearAll(s.cols, s.rows);
                                s.horPos = 2;
                                s.verPos = 2;
                                s = doUndo(s, win, s.cols, s.rows);
                        }
                        else
                        {
                                move(s.verPos, s.horPos);
                        }
                        break;

                case CTRL('a'): // brush to *
                        move(s.verPos, s.horPos);
                        s.drawBrush = '*';
                        break;
                case CTRL('s'):  // brush to #
                        move(s.verPos, s.horPos);
                        s.drawBrush = '#';
                        break;
                case CTRL('d'): // empty brush
                        move(s.verPos, s.horPos);
                        s.drawBrush = ' ';
                        break;
                case CTRL('w'): // save-to-file
                        if (!s.mode)
                                saveToFile(s.cols, s.rows-2, s.fileName);

                        break;
                case CTRL('x'): // exit
                        s.running = false;
                        break;

                case 10:
                        s.horPos = 2;
                        move(++s.verPos, s.horPos);
                        break;

                case 127:
                        printw(" ");
                        if (s.horPos > 2)
                        {
                                move(s.verPos, --s.horPos);
                        }
                        else
                        {
                                move(s.verPos, s.horPos);
                        }
                        printw(" ");
                        move(s.verPos, s.horPos);
                        break;
                default:
                        printw("%c", ch);
                        if (s.horPos < s.cols-2)
                        {
                                move(s.verPos, ++s.horPos);
                        }
                        else {
                                if (s.verPos < s.rows-3)
                                {
                                        s.horPos=2;
                                        move(++s.verPos, s.horPos);
                                }
                                else
                                {
                                        move(s.verPos, s.horPos);
                                }
                        }
        }

        move(s.verPos, s.horPos);
        wrefresh(win);

        return s;
}

int main(void)
{
        Status status = {
                .mode = false,
                .horPos = 2,
                .verPos = 2,
                .running = true,
                .undoBuf[0] = 0,
                .drawBrush = '*',
                .fileName = "Draft1.txt",
        };

        initscr();
        cbreak();
        raw();
        noecho();

        keypad(stdscr, true);

        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        WINDOW *win = newwin(w.ws_row-2, w.ws_col-1, 1, 1);
        refresh();
        wrefresh(win);

        status.rows = w.ws_row;
        status.cols = w.ws_col;

        werase(win);
        box(win, 0, 0);
        move(0, 2);
        printw("Draft1.txt");
        move(0, status.cols-19);
        printw("Lazy Drafting %.2f", VERSION);
        printMode(status.mode, status.rows-1);
        wrefresh(win);
        move(2,2);

        while (status.running)
        {
                int ch = getch();
                status = executeInput(ch, status, win);
        }

        endwin();

        return 0;
}

#include "ui_utils.h"


void redraw_border(WINDOW *win, int16_t color_scheme) {
    wclear(win);
    wcolor_set(win, color_scheme, NULL);
    box(win, 0, 0);
    wrefresh(win);
}

WINDOW *init_win(int width, int height, int woffset, int hoffset, bool is_border, int16_t color_scheme) {
    WINDOW *win = newwin(width, height, woffset, hoffset);
    wattron(win, A_NORMAL);
    wattrset(win, COLOR_PAIR(1));
    if (is_border) {
        redraw_border(win, color_scheme);
    }
    return win;
}


#ifndef SPOLAB3_UI_UTILS_H
#define SPOLAB3_UI_UTILS_H

#include <ncurses.h>

void redraw_border(WINDOW *win, int16_t color_scheme);

WINDOW *init_win(int width, int height, int woffset, int hoffset, bool is_border, int16_t color_scheme);

#endif //SPOLAB3_UI_UTILS_H

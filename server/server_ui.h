#ifndef SPOLAB3_SERVER_UI_H
#define SPOLAB3_SERVER_UI_H

#include <ncurses.h>
#include <malloc.h>
#include "../utils/ui_list.h"

typedef struct {
    WINDOW *main_border;
    ui_list *list;
} UI;

WINDOW *init_win(int width, int height, int woffset, int hoffset, bool is_border);

UI *init_ui();

#endif //SPOLAB3_SERVER_UI_H

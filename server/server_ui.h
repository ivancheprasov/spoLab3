#ifndef SPOLAB3_SERVER_UI_H
#define SPOLAB3_SERVER_UI_H

#include <ncurses.h>
#include <malloc.h>
#include "../utils/ui/ui_list.h"

typedef struct {
    WINDOW *main_border;
    ui_list *list;
} UI;

UI *init_ui();

#endif //SPOLAB3_SERVER_UI_H

#ifndef SPOLAB3_UI_LIST_H
#define SPOLAB3_UI_LIST_H

#include <curses.h>
#include "linked_list.h"

typedef struct {
    WINDOW *window;
    bool is_tracking;
    uint16_t first_index;
    uint16_t last_index;
} ui_list;

ui_list *create_list(WINDOW *window);

void redraw(ui_list *list, linked_list *messages);

void tracking_redraw(ui_list *list, linked_list *messages);

void handle_up(ui_list *list, linked_list *messages);

void handle_down(ui_list *list, linked_list *messages);

void handle_home(ui_list *list, linked_list *messages);

void handle_end(ui_list *list, linked_list *messages);
#endif //SPOLAB3_UI_LIST_H

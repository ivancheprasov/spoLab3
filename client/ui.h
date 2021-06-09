#ifndef SPOLAB3_UI_H
#define SPOLAB3_UI_H

#include <stdbool.h>
#include <curses.h>
#include "../utils/ui/textfield.h"
#include "../utils/ui/ui_list.h"

typedef struct {
    bool is_msg_active;
    WINDOW *main;
    WINDOW *main_border;
    WINDOW *msg;
    WINDOW *msg_border;
    WINDOW *to;
    WINDOW *to_border;
    textfield *to_field;
    textfield *msg_field;
    ui_list *history;
} client_ui;

void toggle_active(client_ui *ui);

client_ui *init_client_ui(char* username);

#endif //SPOLAB3_UI_H

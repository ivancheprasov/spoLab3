#ifndef SPOLAB3_UI_H
#define SPOLAB3_UI_H

#include <stdbool.h>
#include <curses.h>
#include "../utils/ui/textfield.h"
#include "../utils/ui/ui_list.h"
#include "../utils/message.h"

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

client_ui *init_client_ui(char* username);

void switch_active_textfield(client_ui *ui);

void send_msg_to_server(int32_t server_fd, message *msg);

#endif //SPOLAB3_UI_H

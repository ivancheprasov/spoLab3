#include <memory.h>
#include <stdlib.h>
#include "ui.h"
#include "../utils/ui/ui_utils.h"

client_ui *init_client_ui(char *username) {
    client_ui *ui = malloc(sizeof(client_ui));
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    start_color();
    refresh();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    ui->main_border = init_win(LINES - 3, COLS, 0, 0, true, 1);
    ui->main = init_win(LINES - 5, COLS - 2, 1, 1, false, 1);
    mvwprintw(ui->main_border, 0, (COLS - strlen(username) - 4) / 2, "  %s  ", username);
    ui->to_border = init_win(3, 2 + USERNAME_SIZE, LINES - 3, 0, true, 1);
    ui->to = init_win(1, USERNAME_SIZE, LINES - 2, 1, false, 1);

    ui->msg_border = init_win(3, COLS - (2 + USERNAME_SIZE), LINES - 3, 2 + USERNAME_SIZE, true, 2);
    ui->msg = init_win(1, COLS - USERNAME_SIZE - 4, LINES - 2, 3 + USERNAME_SIZE, false, 1);

    wrefresh(ui->main_border);
    wrefresh(ui->main);

    wrefresh(ui->to_border);
    wrefresh(ui->to);

    wrefresh(ui->msg_border);
    wrefresh(ui->msg);
    ui->is_msg_active = true;
    ui->history = create_list(ui->main);
    ui->to_field = create_textfield(USERNAME_SIZE, ui->to);
    ui->msg_field = create_textfield(getmaxx(ui->main_border) - USERNAME_SIZE - 4, ui->msg);
    return ui;
}

void switch_active_textfield(client_ui *ui) {
    ui->is_msg_active = !ui->is_msg_active;
    if (ui->is_msg_active) {
        redraw_border(ui->msg_border, 2);
        redraw_border(ui->to_border, 1);
    } else {
        redraw_border(ui->to_border, 2);
        redraw_border(ui->msg_border, 1);
    }
    redraw_tf(ui->to_field);
    redraw_tf(ui->msg_field);
}

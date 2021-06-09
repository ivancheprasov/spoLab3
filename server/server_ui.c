#include "server_ui.h"
#include "../utils/ui/ui_utils.h"

UI *init_ui() {
    UI *ui = malloc(sizeof(UI));
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    start_color();
    refresh();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    ui->main_border = init_win(LINES, COLS, 0, 0, true, 1);
    WINDOW *main = init_win(LINES - 2, COLS - 2, 1, 1, false, 1);

    wrefresh(ui->main_border);
    wrefresh(main);

    ui->list = create_list(main);
    return ui;
}
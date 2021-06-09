#include "server_ui.h"

WINDOW *init_win(int width, int height, int woffset, int hoffset, bool is_border) {
    WINDOW *win = newwin(width, height, woffset, hoffset);
    wattron(win, A_NORMAL);
    wattrset(win, COLOR_PAIR(1));
    if (is_border) {
        box(win, 0, 0);
    }
    return win;
}

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

    ui->main_border = init_win(LINES, COLS, 0, 0, true);
    WINDOW *main = init_win(LINES - 2, COLS - 2, 1, 1, false);

    wrefresh(ui->main_border);
    wrefresh(main);

    ui->list = create_list(main);
    return ui;
}
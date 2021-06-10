#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/const.h"
#include "server.h"
#include "server_ui.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        puts("No required arguments provided: <server_port>");
        return -1;
    }
    uint16_t port = strtoul(argv[1], NULL, BASE_10);


    UI *ui = init_ui();
    server_info *info_ptr = startup(port, tracking_redraw, ui);

    if (info_ptr == NULL) {
        endwin();
        puts("Unable to startup");
        return -1;
    }
    int cur = getch();
    while (cur != KEY_F(1)) {
        char command[500] = {0};
        memset(command, 0, 500);
        switch (cur) {
            case KEY_UP:
                handle_up(ui->list, info_ptr->history);
                break;
            case KEY_DOWN:
                handle_down(ui->list, info_ptr->history);
                break;
            case KEY_F(2):
                handle_home(ui->list, info_ptr->history);
                break;
            case KEY_F(3):
                handle_end(ui->list, info_ptr->history);
                break;
        }
        cur = getch();
    }
    close_server(info_ptr);
    endwin();
    free(info_ptr);
}
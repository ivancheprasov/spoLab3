#include <malloc.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "ui.h"
#include "../utils/ui/ui_utils.h"
#include "../utils/const.h"
#include "../utils/message.h"
#include "client.h"


void send_msg_to_server(int32_t server_fd, message *msg) {
    char buf[MAX_MESSAGE_STRUCT_SIZE] = {0};
    uint16_t size = serialize(msg, buf);
    send(server_fd, &size, sizeof(size), MSG_NOSIGNAL);
    send(server_fd, buf, size, MSG_NOSIGNAL);
}

struct arg {
    uint16_t server_fd;

    void (*redraw_fun)(ui_list *list, linked_list *messages);

    linked_list *history;
    ui_list *list;
};

typedef struct arg arg;

_Noreturn void receive_notifications(arg *a) {
    while (true) {
        message *msg = malloc(sizeof(message));
        receive_message(msg, a->server_fd);
        add_last(a->history, msg);
        a->redraw_fun(a->list, a->history);
    }
}

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

int main(int argc, char **argv) {
    if (argc < 3) {
        puts("No required arguments provided: <server_port> <username>");
        return -1;
    }
    if (strlen(argv[2]) > USERNAME_SIZE - 1) {
        puts("Too large Name");
        return -1;
    }
    uint16_t port = strtoul(argv[1], NULL, BASE_10);
    int32_t server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(HOSTNAME);

    int connection = connect(server_fd, (const struct sockaddr *) &server_address, sizeof(server_address));
    if (connection == -1) {
        puts("There was an error making a connection to the remote socket");
        return -1;
    }
    sign_in(argv[2], server_fd);
    linked_list *history = init_list();
    message *buffer[HISTORY_BUFFER_SIZE] = {0};
    uint16_t history_size = get_history(buffer, server_fd);
    for (int i = history_size - 1; i >= 0; --i) {
        add_last(history, buffer[i]);
    }
    pthread_t notification_thread;
    client_ui *ui = init_client_ui(argv[2]);
    redraw(ui->history, history);
    arg a;
    a.server_fd = server_fd;
    a.history = history;
    a.list = ui->history;
    a.redraw_fun = tracking_redraw;
    pthread_create(&notification_thread, NULL, (void *(*)(void *)) receive_notifications, &a);
    int cur = getch();
    while (cur != KEY_F(1)) {
        switch (cur) {
            case '\n':
                if (ui->is_msg_active && ui->msg_field->cur_size > 0) {
                    message msg;
                    msg.text = malloc(strlen(ui->msg_field->text));
                    strcpy(msg.from, argv[2]);
                    strcpy(msg.to, ui->to_field->text);
                    strcpy(msg.text, ui->msg_field->text);
                    msg.from_size = strlen(argv[2]);
                    msg.to_size = strlen(ui->to_field->text);
                    msg.text_size = strlen(ui->msg_field->text);
                    send_msg_to_server(server_fd, &msg);
                    memset(ui->msg_field->text, 0, ui->msg_field->max_size);
                    memset(ui->to_field->text, 0, ui->to_field->max_size);
                    ui->msg_field->cur_size = 0;
                    redraw_tf(ui->msg_field);
                    if (ui->to_field->cur_size > 0) {
                        ui->to_field->cur_size = 0;
                        redraw_tf(ui->to_field);
                    }
                    ui->is_msg_active = !ui->is_msg_active;//fixme вынести в отдельный метод
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
                break;
            case KEY_UP:
                handle_up(ui->history, history);
                break;
            case KEY_DOWN:
                handle_down(ui->history, history);
                break;
            case KEY_F(2):
                handle_home(ui->history, history);
                break;
            case KEY_F(3):
                handle_end(ui->history, history);
                break;
            case KEY_LEFT:
            case KEY_RIGHT:
                ui->is_msg_active = !ui->is_msg_active;//fixme вынести в отдельный метод
                if (ui->is_msg_active) {
                    redraw_border(ui->msg_border, 2);
                    redraw_border(ui->to_border, 1);
                } else {
                    redraw_border(ui->to_border, 2);
                    redraw_border(ui->msg_border, 1);
                }
                redraw_tf(ui->to_field);
                redraw_tf(ui->msg_field);
                break;
            case KEY_BACKSPACE:
                if (ui->is_msg_active) {
                    handle_backspace(ui->msg_field);
                } else {
                    handle_backspace(ui->to_field);
                }
                break;
            default:
                if (ui->is_msg_active) {
                    handle_input(ui->msg_field, cur);
                } else {
                    handle_input(ui->to_field, cur);
                }
                break;
        }
//        usleep(100000);
        cur = getch();
    }
    endwin();
}
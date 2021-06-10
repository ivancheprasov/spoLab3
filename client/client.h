#ifndef SPOLAB3_CLIENT_H
#define SPOLAB3_CLIENT_H

#include <stdint.h>
#include <pthread.h>
#include "../utils/linked_list.h"
#include "../utils/const.h"
#include "../utils/message.h"
#include "../utils/ui/ui_list.h"

typedef struct {
    int32_t fd;
    char client_name[USERNAME_SIZE];
    pthread_t thread;
} client;

struct arg {
    uint16_t server_fd;

    void (*redraw_fun)(ui_list *list, linked_list *messages);

    linked_list *history;
    ui_list *list;
};

typedef struct arg arg;

int8_t sign_in(char *client_name, int32_t server_fd);

void free_client(client *ptr);

void receive_message(message *msg, int32_t server_fd);

uint16_t get_history(message **buffer, int32_t server_fd);

_Noreturn void receive_notifications(arg *a);

#endif //SPOLAB3_CLIENT_H

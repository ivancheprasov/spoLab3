#ifndef SPOLAB3_SERVER_H
#define SPOLAB3_SERVER_H

#include <stdbool.h>
#include "../utils/message.h"
#include "../utils/linked_list.h"
#include "../client/client.h"

struct server_info {
    uint16_t port;
    int32_t server_fd;
    linked_list *clients;
    linked_list *history;
    pthread_t manager_t_id;
    pthread_mutex_t mutex;
};

typedef struct server_info server_info;

server_info *startup(uint16_t port);

void close_server(server_info *info);

void handle_sign_out(server_info *info, client*c);

void handle_request(message *ptr, server_info *server);

static client *handle_sign_in(server_info *info, char *client_name, int32_t fd);

static client *to_client(void *value);

static bool is_name_unique(linked_list *ptr, char *client_name);

static bool by_username(void *value, char *to_find);

#endif //SPOLAB3_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "server.h"

struct accept_client_arg {
    server_info *server;
    client *client_info;
};

server_info *create_server_info(uint16_t port, void (*redraw_fun)(ui_list *list, linked_list *messages), UI *ui) {
    server_info *server = malloc(sizeof(server_info));
    server->port = port;
    server->clients = init_list();
    server->history = init_list();
    server->redraw_fun = redraw_fun;
    server->ui = ui;
    return server;
}

void send_msg(client *found, message *msg) {
    if (found != NULL) {
        char buf[MAX_MESSAGE_STRUCT_SIZE] = {0};
        uint16_t size = serialize(msg, buf);
        send(found->fd, &size, sizeof(size), MSG_NOSIGNAL);
        send(found->fd, buf, size, MSG_NOSIGNAL);
    }
}

void handle_request(message *ptr, server_info *server) {
    if (ptr->text_size != 0) {
        ptr->time = time(NULL);
        add_last(server->history, ptr);
        server->redraw_fun(server->ui->list, server->history);
        if (ptr->to_size == 0) {
            node *current = server->clients->first;
            while (current != NULL) {
                send_msg(current->value, ptr);
                current = current->next;
            }
        } else {
            client *receiver = find_element(by_username, server->clients, ptr->to);
            if (receiver != NULL) {
                send_msg(receiver, ptr);
            }
            client *sender = find_element(by_username, server->clients, ptr->from);
            send_msg(sender, ptr);
        }
    }
}

void work_with_client(
        int32_t client_socket,
        server_info *ptr
) {
    uint16_t msg_size;
    uint16_t text_size;
    recv(client_socket, &msg_size, sizeof(msg_size), MSG_NOSIGNAL);
    do {
        char buf[MAX_MESSAGE_STRUCT_SIZE];
        memset(buf, 0, MAX_MESSAGE_STRUCT_SIZE);
        recv(client_socket, buf, msg_size, MSG_NOSIGNAL);
        message *msg = malloc(sizeof(message));
        deserialize(msg, buf);
        text_size = msg->text_size;
        handle_request(msg, ptr);
        recv(client_socket, &msg_size, sizeof(msg_size), MSG_NOSIGNAL);
    } while (text_size != 0);
}

bool filter_values(void*value, char* to_filter) {
    return to_message(value)->to_size == 0 || strcmp(to_message(value)->to, to_filter) == 0 ;
}

void send_history(server_info *server, client *client) {
    message *buffer[HISTORY_BUFFER_SIZE];
    uint16_t size = get_last_n(server->history, (void **) buffer, HISTORY_BUFFER_SIZE, filter_values, client->client_name);
    send(client->fd, &size, sizeof(size), MSG_NOSIGNAL);
    for (uint16_t i = 0; i < size; ++i) {
        send_msg(client, buffer[i]);
    }
}

void accept_client(struct accept_client_arg *arg) {
    send_history(arg->server, arg->client_info);
    work_with_client(arg->client_info->fd, arg->server);
    handle_sign_out(arg->server, arg->client_info);
}

_Noreturn void manage_connections(server_info *info) {
    while (true) {
        struct sockaddr_in client_address;
        socklen_t address_len = sizeof(client_address);
        int32_t accepted_socket = accept(info->server_fd, (struct sockaddr *) &client_address, &address_len);
        char username[USERNAME_SIZE];
        recv(accepted_socket, username, USERNAME_SIZE, MSG_NOSIGNAL);
        client *created = handle_sign_in(info, username, accepted_socket);
        if (created != NULL) {
            struct accept_client_arg *arg = malloc(sizeof(struct accept_client_arg));
            arg->server = info;
            arg->client_info = created;
            pthread_create(&arg->client_info->thread, NULL, (void *(*)(void *)) accept_client, arg);
        }
    }
}

server_info *startup(uint16_t port, void (*redraw_fun)(ui_list *list, linked_list *messages), UI *ui) {
    server_info *server_info_ptr = create_server_info(port, redraw_fun, ui);
    int created_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(server_info_ptr->port);
    address.sin_family = AF_INET;
    server_info_ptr->server_fd = created_socket;
    int bind_result = bind(created_socket, (const struct sockaddr *) &address, sizeof(address));
    if (bind_result == -1) {
        return NULL;
    }
    listen(created_socket, 1);
    pthread_create(&server_info_ptr->manager_t_id, NULL, (void *(*)(void *)) manage_connections, server_info_ptr);
    return server_info_ptr;
}

void close_server(server_info *info) {
    close(info->server_fd);
}

void handle_sign_out(server_info *info, client *c) {
    pthread_mutex_lock(&info->mutex);
    close(c->fd);
    remove_element(by_username, info->clients, c->client_name);
    pthread_mutex_unlock(&info->mutex);
}

static bool by_username(void *value, char *to_find) {
    client *c = to_client(value);
    return strcmp(c->client_name, to_find) == 0;
}

static client *to_client(void *value) {
    return (client *) value;
}

static bool is_name_unique(linked_list *ptr, char *client_name) {
    node *current = ptr->first;
    for (uint32_t i = 0; i < ptr->size; ++i) {
        if (strcmp(client_name, to_client(current)->client_name) == 0) {
            return false;
        }
        current = current->next;
    }
    return true;
}

static client *handle_sign_in(server_info *info, char *client_name, int32_t fd) {
    int8_t response;
    client *client_ptr = NULL;
    pthread_mutex_lock(&info->mutex);
    if (is_name_unique(info->clients, client_name)) {
        client_ptr = malloc(sizeof(client));
        strcpy(client_ptr->client_name, client_name);
        client_ptr->fd = fd;
        add_last(info->clients, client_ptr);
        response = 0;
    } else {
        response = -1;
    }
    pthread_mutex_unlock(&info->mutex);
    send(fd, &response, sizeof(response), MSG_NOSIGNAL);
    return client_ptr;
}


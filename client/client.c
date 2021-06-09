#include <string.h>
#include "client.h"
#include <malloc.h>
#include <sys/socket.h>

int8_t sign_in(char *client_name, int32_t server_fd) {
    char response;
    send(server_fd, client_name, strlen(client_name), MSG_NOSIGNAL);
    recv(server_fd, &response, sizeof(response), MSG_NOSIGNAL);
    return (int8_t) response;
}

uint16_t get_history(message **buffer, int32_t server_fd) {
    uint16_t size = 0;
    recv(server_fd, &size, sizeof(size), MSG_NOSIGNAL);
    for (uint16_t i = 0; i < size; ++i) {
        message *msg = malloc(sizeof(message));
        receive_message(msg, server_fd);
        buffer[i] = msg;
    }
    return size;
}

void receive_message(message *msg, int32_t server_fd) {
    uint16_t size;
    recv(server_fd, &size, sizeof(size), MSG_NOSIGNAL);
    char buffer[size];
    recv(server_fd, buffer, size, MSG_NOSIGNAL);
    deserialize(msg, buffer);
}

void free_client(client *ptr) {
    free(ptr->client_name);
    free(ptr);
}


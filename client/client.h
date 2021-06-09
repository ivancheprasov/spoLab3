#ifndef SPOLAB3_CLIENT_H
#define SPOLAB3_CLIENT_H

#include <stdint.h>
#include <pthread.h>
#include "../utils/linked_list.h"
#include "../utils/const.h"
#include "../utils/message.h"


typedef struct {
    int32_t fd;
    char client_name[USERNAME_SIZE];
    pthread_t thread;
} client;

int8_t sign_in(char *client_name, int32_t server_fd);

void free_client(client *ptr);

void receive_message(message *msg, int32_t server_fd);

uint16_t get_history(message **buffer, uint16_t buffer_size, int32_t server_fd);
#endif //SPOLAB3_CLIENT_H

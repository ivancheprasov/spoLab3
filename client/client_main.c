#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "../utils/const.h"
#include "../utils/message.h"
#include "client.h"

struct arg {
    uint16_t server_fd;
};
typedef struct arg arg;

_Noreturn void receive_notifications(arg *a) {
    while (true) {
        message msg;
        receive_message(&msg, a->server_fd);
        log_message(&msg);
    }
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
    message *buffer[HISTORY_BUFFER_SIZE] = {0};
    uint16_t history_size = get_history(buffer, HISTORY_BUFFER_SIZE, server_fd);
    for (int i = 0; i < history_size; ++i) {
        log_message(buffer[history_size]);
    }
    pthread_t notification_thread;
    arg a;
    a.server_fd = server_fd;
    pthread_create(&notification_thread, NULL, (void *(*)(void *)) receive_notifications, &a);
    char input[50] = {0};
    char to_send[MAX_MESSAGE_STRUCT_SIZE] = {0};
    do {
        fgets(input, 50, stdin);
        input[strlen(input) - 1] = '\0';
        message *msg = malloc(sizeof(message));
        msg->text = malloc(strlen(input));
        strcpy(msg->from, argv[2]);
        strcpy(msg->text, input);
        msg->from_size = strlen(argv[2]);
        msg->text_size = strlen(input);
        msg->time = time(NULL);
        uint16_t size = serialize(msg, to_send);
        send(server_fd, &size, sizeof(size), MSG_NOSIGNAL);
        send(server_fd, to_send, size, MSG_NOSIGNAL);
    } while (strcmp(input, "quit") != 0);

}
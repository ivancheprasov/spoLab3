#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/const.h"
#include "server.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        puts("No required arguments provided: <server_port>");
        return -1;
    }
    uint16_t port = strtoul(argv[1], NULL, BASE_10);

    server_info *info_ptr = startup(port, handle_request);

    if (info_ptr != NULL) {
        char input[50] = {0};
        char to_send[MAX_MESSAGE_STRUCT_SIZE] = {0};
        do {
            fgets(input, 50, stdin);
            input[strlen(input) - 1] = '\0';
            message *msg = malloc(sizeof(message));
            uint16_t size = serialize(msg, to_send);
        } while (strcmp(input, "quit") != 0);
        close_server(info_ptr);
        free(info_ptr);
    } else {
        puts("Something went wrong");
    }
}
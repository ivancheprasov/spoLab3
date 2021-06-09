#ifndef SPOLAB3_MESSAGE_H
#define SPOLAB3_MESSAGE_H

#include <stdint.h>
#include <time.h>
#include "const.h"

typedef struct {
    uint8_t from_size;
    uint8_t to_size;
    uint16_t text_size;
    time_t time;
    char from[USERNAME_SIZE];
    char to[USERNAME_SIZE];
    char *text;
} message;

uint16_t serialize(message *src, char to_fill[MAX_MESSAGE_STRUCT_SIZE]);

uint16_t deserialize(message *to_fill, char src[MAX_MESSAGE_STRUCT_SIZE]);

void log_message(message *message);

#endif //SPOLAB3_MESSAGE_H

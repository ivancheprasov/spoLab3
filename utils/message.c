//
// Created by subhuman on 6/7/21.
//

#include <memory.h>
#include <malloc.h>
#include <unistd.h>
#include "message.h"

uint16_t serialize(message *src, char to_fill[MAX_MESSAGE_STRUCT_SIZE]) {
    if (src != NULL && to_fill != NULL) {
        memcpy(to_fill, src, sizeof(uint8_t) * 2 + sizeof(uint16_t) + TIME_T_SIZE);
        uint16_t size = 12;
        memcpy(to_fill + size, src->from, src->from_size);
        size += src->from_size;
        memcpy(to_fill + size, src->to, src->to_size);
        size += src->to_size;
        memcpy(to_fill + size, src->text, src->text_size);
        size += src->text_size;
        return size;
    } else {
        return -1;
    }
}

uint16_t deserialize(message *to_fill, char src[MAX_MESSAGE_STRUCT_SIZE]) {
    if (src != NULL && to_fill != NULL) {
        memcpy(to_fill, src, 12);
        uint16_t size = 12;
        memcpy(to_fill->from, src + size, to_fill->from_size);
        size += to_fill->from_size;
        memcpy(to_fill->to, src + size, to_fill->to_size);
        size += to_fill->to_size;
        to_fill->text = malloc(to_fill->text_size);
        memcpy(to_fill->text, src + size, to_fill->text_size);
        to_fill->text[to_fill->text_size] = '\0';
        size += to_fill->text_size;
        return size;
    } else {
        return -1;
    }
}

void log_message(message *message) {
    if (message != NULL && message->text_size != 0) {
        uint16_t log_size = DATE_STRING_SIZE + USERNAME_SIZE + 3 + message->text_size;
        char buffer[log_size];
        char date_str[DATE_STRING_SIZE] = {0};
        strftime(date_str, DATE_STRING_SIZE, "[%d.%m.%Y %X]", localtime(&message->time));
        char name_formatted[USERNAME_SIZE] = {0};
        memcpy(name_formatted, message->from, message->from_size);
        for (uint8_t i = message->from_size; i < USERNAME_SIZE - 1; ++i) {
            name_formatted[i] = ' ';
        }
        sprintf(buffer, "%s %s- %s\n", date_str, name_formatted, message->text);
        write(1, buffer, strlen(buffer));
    }
}
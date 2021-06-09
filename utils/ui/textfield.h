#ifndef SPOLAB3_TEXTFIELD_H
#define SPOLAB3_TEXTFIELD_H

#include <stdbool.h>
#include <stdint.h>
#include <curses.h>

typedef struct {
    WINDOW *window;
    uint16_t max_size;
    uint16_t cur_size;
    char *text;
} textfield;

textfield *create_textfield(uint16_t maxsize, WINDOW *window);

void handle_input(textfield *ptr, char c);

void handle_backspace(textfield *ptr);

void redraw_tf(textfield *ptr);

#endif //SPOLAB3_TEXTFIELD_H

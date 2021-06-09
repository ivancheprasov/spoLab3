#include <malloc.h>
#include "textfield.h"

textfield *create_textfield(uint16_t maxsize, WINDOW *window) {
    textfield *field = malloc(sizeof(textfield));
    field->text = malloc(maxsize);
    field->max_size = maxsize;
    field->cur_size = 0;
    field->window = window;
    return field;
}

void redraw_tf(textfield *ptr) {
    wclear(ptr->window);
    if (ptr->cur_size != 0) {
        mvwprintw(ptr->window, 0, 0, "%s", ptr->text);
    }
    wrefresh(ptr->window);
}
void handle_input(textfield *ptr, char c) {
    if (ptr->cur_size < ptr->max_size) {
        ptr->text[ptr->cur_size++] = c;
    }
    redraw_tf(ptr);
}

void handle_backspace(textfield *ptr) {
    if (ptr->cur_size > 0) {
        ptr->text[--ptr->cur_size] = '\0';
    }
    redraw_tf(ptr);
}
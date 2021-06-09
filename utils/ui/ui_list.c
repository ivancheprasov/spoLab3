#include <memory.h>
#include <malloc.h>
#include "ui_list.h"
#include "../message.h"

void redraw(ui_list *list, linked_list *messages) {
    wclear(list->window);
    if (list->is_tracking) {
        if (messages->size > getmaxy(list->window)) {
            uint16_t buffer_size = getmaxy(list->window);
            message *buffer[buffer_size];
            memset(buffer, 0, sizeof(message *) * buffer_size);
            get_last_n(messages, (void **) buffer, buffer_size, NULL, NULL);
            for (int16_t i = buffer_size - 1; i >= 0; --i) {
                char line_buffer[getmaxx(list->window)];
                memset(line_buffer, 0, getmaxx(list->window));
                strftime(line_buffer, sizeof(line_buffer), "[%d.%m.%Y %X]", localtime(&buffer[i]->time));
                mvwprintw(list->window, buffer_size - 1 - i, 0, "%s %s- %s\n", line_buffer, buffer[i]->from,
                          buffer[i]->text);
            }
        } else {
            if (messages->size > 0) {
                node *current = messages->first;
                for (int32_t i = 0; i < messages->size; ++i) {
                    char line_buffer[getmaxx(list->window)];
                    memset(line_buffer, 0, getmaxx(list->window));
                    strftime(line_buffer, sizeof(line_buffer), "[%d.%m.%Y %X]",
                             localtime(&to_message(current->value)->time));
                    mvwprintw(list->window, i, 0, "%s %s- %s\n", line_buffer, to_message(current->value)->from,
                              to_message(current->value)->text);
                    current = current->next;
                }
            }
        }
    } else {
//        if (messages->size > getmaxy(list->window)) {
        for (uint16_t i = list->first_index; i < list->last_index; ++i) {
            message *current = to_message(get_element(messages, i));
            char line_buffer[getmaxx(list->window)];
            memset(line_buffer, 0, getmaxx(list->window));
            strftime(line_buffer, sizeof(line_buffer), "[%d.%m.%Y %X]", localtime(&current->time));
            mvwprintw(list->window, i - list->first_index, 0, "%s %s- %s\n", line_buffer, current->from,
                      current->text);
        }
//        } else {
//
//        }
    }
    wrefresh(list->window);
}

void tracking_redraw(ui_list *list, linked_list *messages) {
    if(list->is_tracking){
        if (messages->size > getmaxy(list->window)) {
            list->first_index++;
        }
        list->last_index++;
        redraw(list, messages);
    }
}

void handle_up(ui_list *list, linked_list *messages) {
    if (messages->size > getmaxy(list->window)) {
        if (list->first_index > 0) {
            list->is_tracking = false;
            list->first_index--;
            list->last_index--;
        }
    } else {
        list->first_index = 0;
        list->last_index = messages->size - 1;
        list->is_tracking = true;
    }
    redraw(list, messages);
}

void handle_down(ui_list *list, linked_list *messages) {
    if (messages->size > getmaxy(list->window)) {
        if (messages->size > list->last_index) {
            list->is_tracking = false;
            list->first_index++;
            list->last_index++;
        }
    } else {
        list->first_index = 0;
        list->last_index = messages->size - 1;
        list->is_tracking = true;
    }
    redraw(list, messages);

}

void handle_home(ui_list *list, linked_list *messages) {
    if (messages->size > getmaxy(list->window)) {
        list->is_tracking = false;
        list->first_index = 0;
        list->last_index = getmaxy(list->window);
        redraw(list, messages);
    }
}

void handle_end(ui_list *list, linked_list *messages) {
    if (messages->size > getmaxy(list->window)) {
        list->is_tracking = true;
        list->last_index = messages->size;
        list->first_index = messages->size - getmaxy(list->window);
        redraw(list, messages);
    }
}

ui_list *create_list(WINDOW *window) {
    ui_list *list = malloc(sizeof(ui_list));
    list->window = window;
    list->first_index = 0;
    list->last_index = 0;
    list->is_tracking = true;
    return list;
}
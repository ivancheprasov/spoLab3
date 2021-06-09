#include <stddef.h>
#include <malloc.h>
#include "linked_list.h"

void *get_element(linked_list *ptr, uint32_t index) {
    if (index >= ptr->size) {
        return NULL;
    }
    struct node *current_node = ptr->first;
    for (uint32_t i = 0; i < index; i++) {
        current_node = current_node->next;
    }
    return current_node->value;
}

uint32_t add_first(linked_list *ptr, void *element) {
//    if (ptr->validation_fun(element)) {
    node *created = malloc(sizeof(node));
    created->next = ptr->first;
    created->value = element;
    ptr->first->prev = created;
    ptr->first = created;
    return 0;
//    }
    return -1;
}

uint32_t add_last(linked_list *ptr, void *element) {
//    if (ptr->validation_fun(element)) {
    node *created = malloc(sizeof(node));
    created->prev = ptr->last;
    created->value = element;
    if (ptr->last == NULL) {
        ptr->last = created;
        ptr->first = created;
    } else {
        ptr->last->next = created;
        ptr->last = created;
    }
    return ++ptr->size;
//    }
    return -1;
}

linked_list *init_list() {
    linked_list *created = malloc(sizeof(linked_list));
//    created->validation_fun = validation_fun;
    return created;
}

void free_list(linked_list *ptr) {
    node *current = ptr->first;
    ptr->free_value_fun(current->value);
    for (uint32_t i = 1; i < ptr->size; ++i) {
        current = current->next;
        free(current->prev);
        ptr->free_value_fun(current->value);
    }
    free(current);
    free(ptr);
}

void remove_element(bool (*by)(void *, char *), linked_list *ptr, char *to_find) {
    node *current = ptr->first;
    while (current != NULL && !by(current->value, to_find)) {
        current = current->next;
    }
    if (current != NULL) {
        if (current->prev != NULL) {
            current->prev->next = current->next;
        }
        if (current->next != NULL) {
            current->next->prev = current->prev;
        }
        free(current);
    }
}

void *find_element(bool (*by)(void *, char *), linked_list *ptr, char *to_find) {
    if (ptr->size == 0) {
        return NULL;
    }
    node *current = ptr->first;
    while (current != NULL && !by(current->value, to_find)) {
        current = current->next;
    }
    return current;
}

uint16_t get_last_n(linked_list *ptr, void **buffer, uint16_t buffer_size) {
    node *current = ptr->last;
    for (uint16_t i = 0; i < buffer_size; ++i) {
        if(current == NULL) return i;
        buffer[i] = current->value;
        current = current->prev;
    }
    return buffer_size;
}
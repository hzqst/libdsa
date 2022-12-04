#include <stdint.h>
#include <string.h>

#include "../include/list.h"

node* node_init(size_t type_size, const void *value);
void node_free(node *n);

struct list {
    size_t length;
    size_t type_size;
    node *front;
    node *back;
};

node* node_init(size_t type_size, const void *value) {
    // checks for overflow of amount of requested memory
    if (type_size > PTRDIFF_MAX) return NULL;

    node *new = malloc(sizeof(node));
    void *data = malloc(type_size);
    if (new == NULL || data == NULL) {
        free(data);
        free(new);
        return NULL;
    }

    memcpy(data, value, type_size);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

void node_free(node *n) {
    if (n != NULL) free(n->data);
    free(n);
}

list* list_init(size_t type_size) {
    list *l = malloc(sizeof(list));
    if (l == NULL) return NULL;

    l->length = 0;
    l->type_size = type_size;
    l->front = NULL;
    l->back = NULL;
    return l;
}

void list_free(list *l) {
    if (l != NULL) {
        node *front = l->front;
        while (front != NULL) {
            node *tmp = front->next;
            node_free(front);
            front = tmp;
        }
    }
    free(l);
}

size_t list_length(const list *l) {
    return l->length;
}

bool list_is_empty(const list *l) {
    return l->length == 0;
}

const node* list_front(const list *l) {
    if (list_is_empty(l)) return NULL;
    return l->front;
}

const node* list_back(const list *l) {
    if (list_is_empty(l)) return NULL;
    return l->back;
}

const node* list_push_back(list *l, const void *value) {
    node *new = node_init(l->type_size, value);
    if (new == NULL) return NULL;

    if (list_is_empty(l)) {
        l->front = new;
        l->back = new;
    } else {
       l->back->next = new;
       new->prev = l->back;
       l->back = new;
    }

    l->length += 1;
    return new;
}

const node* list_push_front(list *l, const void *value) {
    node *new = node_init(l->type_size, value);
    if (new == NULL) return NULL;

    if (list_is_empty(l)) {
        l->front = new;
        l->back = new;
    } else {
        new->next = l->front;
        l->front->prev = new;
        l->front = new;
    }

    l->length += 1;
    return new;
}

void list_pop_back(list *l) {
    if (list_is_empty(l)) return;

    node *back = l->back;
    node *prev = back->prev;
    l->back = prev;
    if (prev != NULL) prev->next = NULL;
    if (l->front == back) l->front = prev;

    l->length -= 1;
    node_free(back);
}

void list_pop_front(list *l) {
    if (list_is_empty(l)) return;

    node *front = l->front;
    node *next = front->next;
    l->front = next;
    if (next != NULL) next->prev = NULL;
    if (l->back == front) l->back = next;

    l->length -= 1;
    node_free(front);
}

const node* list_insert(list *l, node *pos, const void *value) {
    node *new = node_init(l->type_size, value);
    if (new == NULL) return NULL;

    if (pos == l->back) {
        pos->next = new;
        new->prev = pos;
        l->back = new;
    } else {
        node *next = pos->next;
        pos->next = new;
        new->prev = pos;
        new->next = next;
        next->prev = new;
    }

    l->length += 1;
    return new;
}

void list_erase(list *l, node *pos) {
    bool is_back = l->back == pos;
    bool is_front = l->front == pos;
    if (is_back) list_pop_back(l);
    if (is_front) list_pop_front(l);
    if (is_back || is_front) return;

    node *next = pos->next;
    node *prev = pos->prev;
    next->prev = prev;
    prev->next = next;

    l->length -= 1;
    node_free(pos);
}

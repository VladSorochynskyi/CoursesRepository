#include <stdlib.h>
#include "list.h"
#include "stock.h"


struct list_node_s{
    struct list_node_s * next;
    struct list_node_s * prev;
    good_t data;
};

struct list_s{
    struct list_node_s * head;
    struct list_node_s * tail;
    int size;
};

list_t list_new ()
{
    list_t list = malloc(sizeof(struct list_s));
    list->head = malloc(sizeof(struct list_node_s));
    list->tail = malloc(sizeof(struct list_node_s));
    list->head->prev = NULL;
    list->tail->next = NULL;
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->size = 0;
    return list;
}

void list_add(list_t list, good_t data, int pos)
{
    struct list_node_s * new_node = malloc(sizeof(struct list_node_s));
    struct list_node_s * cur_node = list->head;
    if (list->size < pos)
        pos = list->size;
    for (int i = 0; i < pos; i++)
    {
        cur_node = cur_node->next;
    }
    new_node->data = data;
    new_node->prev = cur_node;
    new_node->next = cur_node->next;
    cur_node->next->prev = new_node;
    cur_node->next = new_node;
    list->size++;
}

good_t list_del(list_t list, int pos)
{
    good_t data;
    struct list_node_s * cur_node = list->head->next;
    if (list->tail == cur_node)
        return NULL;
    if (pos >= list->size )
    {
        pos = list->size - 1;
    }
    for (int i = 0; i < pos; i++)
        cur_node = cur_node->next;
    data = cur_node->data;
    cur_node->prev->next = cur_node->next;
    cur_node->next->prev = cur_node->prev;
    free(cur_node);
    list->size--;
    return data;
}

good_t list_change(list_t list, good_t data, int pos)
{
    good_t tmp_data;
    struct list_node_s * cur_node = list->head->next;
    if (list->tail == cur_node)
        return NULL;
    if (pos >= list->size )
    {
        pos = list->size - 1;
    }
    for (int i = 0; i < pos; i++)
        cur_node = cur_node->next;
    tmp_data = cur_node->data;
    cur_node->data = data;
    return tmp_data;
}

good_t list_getGood (list_t list, int pos) {
    struct list_node_s * cur_node = list->head->next;
    if (list->tail == cur_node)
        return NULL;
    if (pos >= list->size )
    {
        pos = list->size - 1;
    }
    for (int i = 0; i < pos; i++)
        cur_node = cur_node->next;
    return cur_node->data;
}

int list_size(list_t list)
{
    return list->size;
}

void list_free(list_t list)
{
    for (int i = 0; i < list->size; i++)
        list_del(list, 0);
    free(list->tail);
    free(list->head);
    free(list);
}



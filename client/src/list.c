#include "list.h"
#include <stdlib.h>
#include <string.h>

List *List_create(unsigned int elem_size, unsigned int capacity)
{
    List *list = (List *)malloc(sizeof(List));


    list->elem_size = elem_size;
    list->capacity = capacity;
    list->size = 0;

    list->data = (void *)malloc(list->elem_size * list->capacity);

    return list;
}

void *List_getAt(List *list, unsigned int index)
{
    if (index >= list->size)
        return NULL;

    unsigned char *base = (unsigned char *)list->data;

    return base + (list->elem_size * index);
}

void List_resize(List *list, unsigned int new_capacity)
{   
    if(new_capacity < list->size) return;

    void *new_data = realloc(list->data, new_capacity * list->elem_size);

    if (new_data)
        list->data = new_data;
    else
    {
        new_data = malloc(list->elem_size * new_capacity);
        if (!new_data)
            return;

        unsigned char *new_base = (unsigned char *)new_data;
        unsigned char *old_base = (unsigned char *)list->data;

        memcpy(new_base, old_base, list->size * list->elem_size);

        free(old_base);
        list->data = new_data;
    }


    list->capacity = new_capacity;

}

void List_insert(List *list, void *elem)
{
    if (list->size >= list->capacity)
    {
        List_resize(list, (list->capacity == 0)? 1 : list->capacity * 2);
    }

    unsigned char *base = (unsigned char *)list->data;

    unsigned char *dest = base + (list->size * list->elem_size);

    memcpy(dest, elem, list->elem_size);

    list->size++;
}

void List_delete(List *list, unsigned int index)
{
    if(index >= list->size) return;

    memmove(List_getAt(list, index), List_getAt(list, index + 1), list->elem_size * (list->size - index - 1));
    
    list->size--;
}

void List_free(List *list)
{
    if (!list) return;
    free(list->data);
    free(list);
}
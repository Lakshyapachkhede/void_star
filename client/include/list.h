#ifndef LIST_H
#define LIST_H



typedef struct List
{
    void *data;
    unsigned int elem_size;
    unsigned int size;
    unsigned int capacity;
}List;


List *List_create(unsigned int elem_size, unsigned int capacity);
void *List_getAt(List *list, unsigned int index);
void List_insert(List *list, void *elem);
void List_delete(List *list, unsigned int index);
void List_free(List *list);



#endif

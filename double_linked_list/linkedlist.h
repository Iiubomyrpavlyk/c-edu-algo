#pragma once

#include "stdbool.h"
#include "stdlib.h"

/* types */
typedef struct list_node_s list_node_t;
typedef struct list_s list_t;

extern list_t * list(); // linked list contructor

extern bool list_push_front(list_t *list, const void *data, size_t size);
extern bool list_push_back(list_t *list, const void *data, size_t size);
extern bool list_insert(list_t *list, int index, const void* data, size_t size);

extern bool list_pop_front(list_t *list);
extern bool list_pop_back(list_t *list);
extern bool list_erase(list_t* list, int index);

// linked list container
struct list_s
{
    bool (*push_front)(list_t *list, const void *data, size_t size);
    bool (*push_back)(list_t *list, const void *data, size_t size);
    bool (*insert)(list_t *list, int index, const void *data, size_t size);
        
    bool (*pop_front)(list_t *list);
    bool (*pop_back)(list_t *list);
    bool (*erase)(list_t *list, int index);

    size_t size;
    list_node_t * first;
    list_node_t * last;
};

// linked list node
struct list_node_s
{
    void * data;
    size_t size;
    
    list_node_t * next, * prev;
};

















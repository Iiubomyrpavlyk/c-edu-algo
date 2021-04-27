#pragma once

#include <stdbool.h>
#include <stdlib.h>

/* types */
typedef struct queue_node_s queue_node_t;
typedef struct queue_s queue_t;

extern queue_t * queue();

extern bool queue_insert(queue_t *queue, const void *data, size_t size);
extern bool queue_remove(queue_t * queue);

extern bool queue_isempty(queue_t *queue);

extern bool queue_clear(queue_t *queue);

struct queue_s
{
    bool (*insert)(queue_t *queue, const void *data, size_t size);
    bool (*remove)(queue_t *queue);
    
    bool (*isempty)(queue_t *queue);

    bool (*clear)(queue_t *queue);

    size_t size;
    queue_node_t *head, *tail;
};


struct queue_node_s
{
    void *data;
    
    queue_node_t * next;
};






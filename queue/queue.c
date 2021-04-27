#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

queue_t * queue()
{
    queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
    
    if (queue == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for queue\n");
        return NULL;
    }
            
    queue->insert = queue_insert;
    queue->remove = queue_remove;
    
    queue->isempty = queue_isempty;
    
    queue->clear = queue_clear;

    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

bool queue_isempty(queue_t *queue)
{
    return queue->size;
}

bool queue_insert(queue_t *queue, const void *data, size_t size)
{
    if (data == NULL)
        return false;

    void *dup_data = malloc(sizeof(size));
    if (dup_data == NULL)
        return false;

    memcpy(dup_data, data, sizeof(size));   

    // make new node
    queue_node_t *node = (queue_node_t*) malloc(sizeof(queue_node_t));
    if (node == NULL)
        return false;
    
    node->data = dup_data;
    node->next = NULL;
    
    if (queue->head == NULL)
        queue->head = node;
    else 
    {
        queue_node_t *ptr = queue->head;
        while (ptr->next != NULL)
            ptr = ptr->next;

        ptr->next = node;
    }

    queue->tail = node;
   
    queue->size++;

    return true;
}

bool queue_remove(queue_t *queue)
{
    if (queue->head == NULL)
        return false;
   
    queue_node_t *node = queue->head;
    
    if (queue->head->next == NULL)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
        queue->head = node->next;

    queue->size--;
    free(node->data);
    free(node);          
                        
    return true;
}

bool queue_clear(queue_t *queue)
{
    if (queue == NULL)
        return true;

    while (queue->size--) 
        queue->remove(queue);      
  
    return true;
}








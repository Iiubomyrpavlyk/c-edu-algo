#include "stdio.h"
#include "string.h"
#include "linkedlist.h"
#include <stdlib.h>

static list_node_t * get_node(list_t * list, int index);
static bool remove_node(list_t * list, list_node_t * node);

list_t * list()
{
    list_t *list = (list_t *) calloc(1, sizeof(list_t));
    
    if (list == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for list\n");
        return NULL;     
    }   
    
    list->push_front = list_push_front;
    list->push_back = list_push_back;    
    list->insert = list_insert;
    
    list->pop_front = list_pop_front;
    list->pop_back = list_pop_back;
    list->erase = list_erase;
    
    list->first = NULL;
    list->last = NULL;

    return list;
}

bool list_push_front(list_t * list, const void *data, size_t size)
{
    return list_insert(list, 0, data, size);
}

bool list_push_back(list_t *list, const void *data, size_t size)
{
    return list_insert(list, -1, data, size);
}

bool list_insert(list_t *list, int index, const void *data, size_t size)
{
    if (data == NULL)
        return false;

    // adjust index
    if (index < 0)
        index = (list->size + index) + 1;   // -1 is same as last
    if (index < 0 || index > list->size)
    {
        fprintf(stderr, "Out of bound %d != {0, %lu}", index, list->size);
        return false;
    }

    // duplicate obj
    void *dup_data = malloc(sizeof(size));
    if (dup_data == NULL)
        return false;
   
    memcpy(dup_data, data, size);

    // make new node 
    list_node_t * node = (list_node_t *) malloc(sizeof(list_node_t));
   
    if (node == NULL)
    {
        free(node);
        return false;
    }
    
    node->data = dup_data;
    node->next = NULL;  
    node->prev = NULL;
   
    if (index == 0)
    {
        node->next = list->first;
        list->first = node;
        if (node->next != NULL)
            node->next->prev = node;
        if (list->last == NULL)
            list->last = node;
    } else if (index == list->size)
    {
        // add after last
        node->prev = list->last;
        if (node->prev != NULL)
            node->prev->next = node;
        list->last = node;
        if (list->first == NULL)
            list->first = node;
    } else
    {
        list_node_t *ptr = get_node(list, index);
        if (ptr == NULL)
            return false;
       
         // insert obj
         ptr->prev->next = node;
         node->prev = ptr->prev;
         node->next = ptr;
         ptr->prev = node;
    }
   
    list->size++;

    return true;
}

static list_node_t * get_node(list_t * list, int index)
{
    if (index < 0)
        index = list->size + index;

    if (index >= list->size)
        return NULL;

    list_node_t * node = list->first;
    int idx = 0;

    while (node != NULL)
    {
        if (idx == index)
            return node;
        node = node->next;
        idx++;
    }    

    return NULL;
}

static bool remove_node(list_t * list, list_node_t * node)
{
    if (node == NULL)
        return false;

    // chain prev and next elem
    if (node->prev == NULL)
        list->first = node->next;
    else 
        node->prev->next = node->next;

    if (node->next == NULL)
        list->last = node->prev;
    else
        node->next->prev = node->prev;    
    
    // adjust size
    list->size--;

    free(node->data);
    free(node);
    
    return true;
}  

bool list_pop_front(list_t *list)
{
    return remove_node(list, list->first);
}

bool list_pop_back(list_t *list)
{
    return remove_node(list, list->last);
}

bool list_erase(list_t *list, int index)
{
    list_node_t *node = get_node(list, index);

    if (node == NULL)
        return false;

    return remove_node(list, node);
}



#include <stdlib.h>
#include <windows.h>

#include "structs_unions.h"

Queue* initQueue() {
    Queue* queue = calloc(1, sizeof(Queue));
    if(queue == NULL)return NULL;

    return queue;
}

status_t enqueue(Queue* queue, void* data) {
    QueueElement* new = malloc(sizeof(QueueElement));
    if(new == NULL)return MEMORY_FAILURE;
    
    new->data = data;
    new->next = NULL;
    
    if(queue->size == 0) {
        queue->head = new;
        queue->last = new;
    }
    else if(queue->size == 1) {
        queue->head->next = new;
        queue->last = new;
    }
    else {
        queue->last->next = new;
    }
    queue->size++;
    
    return SUCCESS;
}

void* dequeue(Queue* queue) {
    if(queue->head == NULL)return NULL; //empty queue
    void* data = queue->head->data;
    QueueElement* next = queue->head->next;

    free(queue->head);   
    queue->head = next;
    queue->size--;

    return data;
}

char dequeueAlt(Queue* queue, void* out) {
    if(queue->head == NULL)return 0;
    out = queue->head->data;
    QueueElement* next = queue->head->next;

    free(queue->head);
    queue->head = next;
    queue->size--;

    return 1;
}

void printQueue(Queue* queue) {
    printf("Size: %lld\n", queue->size);
    QueueElement* current = queue->head;
    size_t i = 1;
    while(current != NULL) {
        printf("%lld | Data: %p\n", i, current->data);
        current = current->next;
        i++;
    }
}

void freeQueue(Queue* queue) {
    QueueElement* current = queue->head;
    QueueElement* tmp;
    while(current != NULL) {
        free(current->data);
        tmp = current->next;
        free(current);
        current = tmp;
        queue->size--;
    }
    free(queue);
}
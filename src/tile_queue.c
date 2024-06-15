#include <stdio.h>
#include <stdlib.h>

#include "deus.h"
#include "logging.h"
#include "tiles.h"

status_t enqueueTile(TileQueue* queue, Tile* tile) {
    TileQueueElement* e = malloc(sizeof(TileQueueElement));
    if(e == NULL) return MEMORY_FAILURE;
    
    e->tile = tile;
    e->next = NULL;

    if(queue->size == 0) {
        queue->head = queue->last = e;
    }
    else if(queue->size == 1) {
        queue->head->next = queue->last;
        queue->last->next = e;
        queue->last = e;
    }
    else {
        queue->last->next = e;
        queue->last = e;
    }
    queue->size++;
    return SUCCESS;
}

status_t dequeueTile(TileQueue* queue, Tile** toStore) {
    if(queue->size == 0) {
        *toStore = NULL;
        return FAILURE;
    }
    TileQueueElement* next = queue->head->next;
    *toStore = queue->head->tile;
    free(queue->head);
    queue->head = next;
    queue->size--;

    return SUCCESS;
}

void flushTileQueue(TileQueue* queue) {
    if(queue->size > 0 && queue->head != NULL) {
        TileQueueElement* current = queue->head;
        TileQueueElement* next = current->next;
        freeTile(current->tile);
        free(current);
        while(next != NULL) {
            current = next;
            next = current->next;
            freeTile(current->tile);
            free(current);
        }
        queue->size = 0;
    }
}

void printTileQueue(TileQueue* queue, FILE* stream) {
    if(queue->size == 0) logToStream(stream, LOGLEVEL_INFO, "Queue is empty");
    else {
        snprintf(loggingBuffer, loggingBufferSize, "[printTileQueue] Queue size: %llu\n", queue->size);
        logToStream(stream, LOGLEVEL_INFO, NULL);
        TileQueueElement* current = queue->head;
        while(current) {
            printTile(current->tile, stream);
            current = current->next;
        }
    }
}
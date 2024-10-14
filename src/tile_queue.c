#include <stdio.h>
#include <stdlib.h>

#include "deus.h"
#include "logging.h"
#include "tiles.h"

status_t enqueueTile(TileQueue* queue, Tile* tile) {
#ifdef DEBUG
    logToStream(
        defaultStream, LOGLEVEL_DEBUG, 
        "[enqueueTile] Queueing %s %s...",
        colorNames[tile->color],
        shapeNames[tile->shape]
    );
#endif
    
    queue->writeIndex++;
    if(queue->writeIndex >= tileQueuedAmount + 1) queue->writeIndex = 0;
    if(queue->writeIndex == queue->readIndex) {
        queue->writeIndex--;
        if(queue->writeIndex < 0) queue->writeIndex = tileQueuedAmount;
        return FAILURE;
    }
    queue->tiles[queue->writeIndex] = tile;
    return SUCCESS;
}

status_t dequeueTile(TileQueue* queue, Tile** toStore) {
    if(queue->readIndex == queue->writeIndex) {
        *toStore = NULL;
        return FAILURE; //queue is empty
    }
    queue->readIndex++;
    if(queue->readIndex >= tileQueuedAmount + 1) queue->readIndex = 0;
    *toStore = queue->tiles[queue->readIndex];
#ifdef DEBUG
    logToStream(
        defaultStream, LOGLEVEL_DEBUG, "[dequeueTile] Dequeued %s %s...",
        colorNames[queue->tiles[queue->readIndex]->color],
        shapeNames[queue->tiles[queue->readIndex]->shape]
    );
#endif
    return SUCCESS;
}

void flushTileQueue(TileQueue* queue) {
    while(queue->readIndex != queue->writeIndex) {
        queue->readIndex++;
        if(queue->readIndex >= tileQueuedAmount + 1) queue->readIndex = 0;
        freeTile(queue->tiles[queue->readIndex]);
    }
}

void printTileQueue(TileQueue* queue, FILE* stream) {
    int index = queue->readIndex;
    size_t size = 0;
    if(index == queue->writeIndex) logToStream(stream, LOGLEVEL_DEBUG, "Queue is empty");
    else {
        while(index != queue->writeIndex) {
            index++;
            if(index >= tileQueuedAmount + 1) index = 0;
            size++;
        };
        logToStream(
            stream, LOGLEVEL_DEBUG,
            "[printTileQueue] Queue size: %llu", size
        );
        index = queue->readIndex;
        while(index != queue->writeIndex) {
            index++;
            if(index >= tileQueuedAmount + 1) index = 0;
            printTile(queue->tiles[index], stream);
        }
    }
}
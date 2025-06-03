// src/min_heap.h

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

typedef struct MinHeap {
    int* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* min_heap_create();
void min_heap_free(MinHeap* min_heap);

void min_heap_insert(MinHeap* min_heap, int value);
char min_heap_remove(MinHeap* min_heap, int* removed);
char min_heap_peek(MinHeap* min_heap, int* peek);

int min_heap_length(MinHeap* min_heap);

#endif

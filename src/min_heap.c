// src/min_heap.c

#include <stdlib.h>
#include "min_heap.h"

#define MIN_HEAP_INITIAL_CAPACITY 3

MinHeap* min_heap_create() {
    MinHeap* min_heap = malloc(sizeof(MinHeap));
    if(!min_heap) return NULL;

    min_heap->data = malloc(sizeof(int) * MIN_HEAP_INITIAL_CAPACITY);
    if(!min_heap->data) {
        free(min_heap);
        return NULL;
    }

    min_heap->capacity = MIN_HEAP_INITIAL_CAPACITY;
    min_heap->size = 0;

    return min_heap;
}

void min_heap_free(MinHeap* min_heap) {
    if (!min_heap) return;
    free(min_heap->data);
    free(min_heap);
}

void min_heap_insert(MinHeap* min_heap, int value) {
    if (!min_heap) return;

    if (min_heap->size == min_heap->capacity) {
        int new_capacity = min_heap->capacity * 2;
        int* new_data = malloc(sizeof(int) * new_capacity);

        for (int i = 0; i < min_heap->size; i++) {
            new_data[i] = min_heap->data[i];
        }

        free(min_heap->data);
        min_heap->data = new_data;
        min_heap->capacity = new_capacity;
    }

    int i = min_heap->size;
    min_heap->data[i] = value;
    min_heap->size++;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (min_heap->data[parent] <= value) break;

        min_heap->data[i] = min_heap->data[parent];
        i = parent;
    }

    min_heap->data[i] = value;
}

char min_heap_remove(MinHeap* min_heap, int* removed) {
    if (!min_heap) return 0;
    if (min_heap->size == 0) return 0;

    int min_value = min_heap->data[0];
    int last_value = min_heap->data[min_heap->size - 1];
    min_heap->size--;

    int i = 0;
    int left, right, smallest;

    min_heap->data[0] = last_value;

    while(1) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;

        if (left < min_heap->size && min_heap->data[left] < min_heap->data[smallest]) {
            smallest = left;
        }

        if (right < min_heap->size && min_heap->data[right] < min_heap->data[smallest]) {
            smallest = right;
        }

        if (smallest == i) break;

        int temp = min_heap->data[i];
        min_heap->data[i] = min_heap->data[smallest];
        min_heap->data[smallest] = temp;

        i = smallest;
    }

    *removed = min_value;
    return 1;
}

int min_heap_peek(MinHeap* min_heap) {
    return NULL;
}

int min_heap_length(MinHeap* min_heap) {
    if (!min_heap) return;
    return min_heap->size;
}
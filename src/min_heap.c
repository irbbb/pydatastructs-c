// src/min_heap.c

#include <stdlib.h>
#include "min_heap.h"

#define MIN_HEAP_INITIAL_CAPACITY 3

static void min_heap_sift_up(MinHeap* min_heap, int index) {
    int value = min_heap->data[index];
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (min_heap->data[parent] <= value) break;
        min_heap->data[index] = min_heap->data[parent];
        index = parent;
    }
    min_heap->data[index] = value;
}

static void min_heap_sift_down(MinHeap* min_heap, int index) {
    int value = min_heap->data[index];
    int size = min_heap->size;

    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && min_heap->data[left] < min_heap->data[smallest]) smallest = left;
        if (right < size && min_heap->data[right] < min_heap->data[smallest]) smallest = right;
        if (smallest == index) break;

        min_heap->data[index] = min_heap->data[smallest];
        min_heap->data[smallest] = value;
        index = smallest;
    }
}

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

MinHeap* min_heap_heapify(int* arr, int length) {
    MinHeap* min_heap = malloc(sizeof(MinHeap));
    if(!min_heap) return NULL;

    min_heap->data = malloc(sizeof(int) * length);
    if(!min_heap->data) {
        free(min_heap);
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        min_heap->data[i] = arr[i];
    }

    min_heap->size = length;
    min_heap->capacity = length;

    for (int i = (length - 2) / 2; i >= 0; i--) {
        min_heap_sift_down(min_heap, i);
    }

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
        int* new_data = realloc(min_heap->data, sizeof(int) * new_capacity);
        if (!new_data) return;

        min_heap->data = new_data;
        min_heap->capacity = new_capacity;
    }

    min_heap->data[min_heap->size] = value;
    min_heap_sift_up(min_heap, min_heap->size);
    min_heap->size++;
}

char min_heap_remove(MinHeap* min_heap, int* removed) {
    if (!min_heap) return 0;
    if (min_heap->size == 0) return 0;

    *removed = min_heap->data[0];
    min_heap->data[0] = min_heap->data[--min_heap->size];
    min_heap_sift_down(min_heap, 0);

    return 1;
}

char min_heap_peek(MinHeap* min_heap, int* peek) {
    if (!min_heap) return 0;
    if (min_heap->size == 0) return 0;

    *peek = min_heap->data[0];

    return 1;
}

char min_heap_pushpop(MinHeap* min_heap, int value, int* removed) {
    if (!min_heap || min_heap->size == 0) {
        *removed = value;
        return 1;
    }

    if (value <= min_heap->data[0]) {
        *removed = value;
        return 1;
    }

    *removed = min_heap->data[0];
    min_heap->data[0] = value;
    min_heap_sift_down(min_heap, 0);

    return 1;
}

int min_heap_length(MinHeap* min_heap) {
    if (!min_heap) return 0;
    return min_heap->size;
}
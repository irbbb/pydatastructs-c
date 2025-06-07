// src/dynamic_array.c

#include "dynamic_array.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 16

struct dynamic_array_s {
    int32_t* data;      /**< Pointer to array memory */
    int32_t size;       /**< Number of elements stored */
    int32_t capacity;   /**< Allocated capacity */
};

dynamic_array_t* dynamic_array_create(void) {
    dynamic_array_t* arr = (dynamic_array_t*)calloc(1, sizeof(dynamic_array_t));
    if (!arr) return NULL;
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}

void dynamic_array_destroy(dynamic_array_t* array) {
    if (!array) return;
    free(array->data);
    free(array);
}

static bool ensure_capacity(dynamic_array_t* array, int32_t min_capacity) {
    if (array->capacity >= min_capacity) return true;

    int32_t new_capacity = array->capacity ? array->capacity * 2 : INITIAL_CAPACITY;
    if (new_capacity < min_capacity) new_capacity = min_capacity;

    int32_t* new_data = (int32_t*)realloc(array->data, new_capacity * sizeof(int32_t));
    if (!new_data) return false;

    array->data = new_data;
    array->capacity = new_capacity;
    return true;
}

bool dynamic_array_push_back(dynamic_array_t* array, int32_t value) {
    if (!array) return false;
    if (!ensure_capacity(array, array->size + 1)) return false;

    array->data[array->size++] = value;
    return true;
}

bool dynamic_array_pop_back(dynamic_array_t* array, int32_t* out_value) {
    if (!array || array->size == 0) return false;
    if (out_value) {
        *out_value = array->data[array->size - 1];
    }
    array->size--;
    return true;
}

bool dynamic_array_back(const dynamic_array_t* array, int32_t* out_value) {
    if (!array || array->size == 0 || !out_value) return false;

    *out_value = array->data[array->size - 1];
    return true;
}

bool dynamic_array_is_empty(const dynamic_array_t* array) {
    if (!array) return true;
    return array->size == 0;
}

int32_t dynamic_array_size(const dynamic_array_t* array) {
    if (!array) return 0;
    return array->size;
}

bool dynamic_array_remove_last(dynamic_array_t* array) {
    if (!array || array->size == 0) return false;
    array->size--;
    return true;
}

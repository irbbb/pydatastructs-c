#include "dynamic_array.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>  // for debug prints if needed

static const size_t DYN_ARRAY_MIN_CAPACITY = 16;
static const size_t DYN_ARRAY_GROW_FACTOR = 2;
static const size_t DYN_ARRAY_SHRINK_THRESHOLD_DIV = 4;

/**
 * @brief Helper to check multiplication overflow of size_t.
 * @param a First operand.
 * @param b Second operand.
 * @return true if overflow would occur, false otherwise.
 */
static bool mul_overflow_size_t(size_t a, size_t b) {
    if (a == 0 || b == 0) return false;
    return a > SIZE_MAX / b;
}

/**
 * @brief Internal helper to grow the dynamic array capacity.
 * @param array Pointer to dynamic array.
 * @return Status code.
 */
static dyn_array_status_t dynamic_array_grow(dynamic_array_t *array) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(array->data, DYN_ARRAY_ERROR_NULL_POINTER);

    size_t new_capacity = (array->capacity == 0) ? DYN_ARRAY_MIN_CAPACITY : array->capacity * DYN_ARRAY_GROW_FACTOR;

    if (mul_overflow_size_t(new_capacity, sizeof(DYN_ARRAY_TYPE))) {
        return DYN_ARRAY_ERROR_NO_MEMORY;
    }

    DYN_ARRAY_TYPE *tmp = realloc(array->data, new_capacity * sizeof(DYN_ARRAY_TYPE));
    if (!tmp) {
#ifdef DEBUG
        fprintf(stderr, "ERROR: realloc failed during grow at %s:%d\n", __FILE__, __LINE__);
#endif
        return DYN_ARRAY_ERROR_NO_MEMORY;
    }

    array->data = tmp;
    array->capacity = new_capacity;
    return DYN_ARRAY_OK;
}

/**
 * @brief Internal helper to reallocate the dynamic array's buffer.
 *
 * Ensures the new capacity is not below the minimum allowed, checks for
 * multiplication overflow, and reallocates the array's internal buffer.
 *
 * @param array Pointer to the dynamic array.
 * @param new_capacity Desired new capacity.
 * @return DYN_ARRAY_OK on success, or an appropriate error code on failure.
 */
static dyn_array_status_t dynamic_array_realloc(dynamic_array_t *array, size_t new_capacity) {
    if (new_capacity < DYN_ARRAY_MIN_CAPACITY) {
        new_capacity = DYN_ARRAY_MIN_CAPACITY;
    }

    if (mul_overflow_size_t(new_capacity, sizeof(DYN_ARRAY_TYPE))) {
        return DYN_ARRAY_ERROR_NO_MEMORY;
    }

    DYN_ARRAY_TYPE *tmp = realloc(array->data, new_capacity * sizeof(DYN_ARRAY_TYPE));
    if (!tmp) {
#ifdef DEBUG
        fprintf(stderr, "Memory allocation failed during realloc at %s:%d\n", __FILE__, __LINE__);
#endif
        return DYN_ARRAY_ERROR_NO_MEMORY;
    }

    array->data = tmp;
    array->capacity = new_capacity;
    return DYN_ARRAY_OK;
}

dynamic_array_t* dynamic_array_create(size_t capacity) {
    dynamic_array_t *array = malloc(sizeof(dynamic_array_t));
    if(!array) {
        return NULL;
    }

    dyn_array_status_t status = dynamic_array_init(array, capacity);
    if (status != DYN_ARRAY_OK) {
        free(array);
        return NULL;
    }
    return array;
}

dyn_array_status_t dynamic_array_init(dynamic_array_t *array, size_t capacity) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    if (capacity < DYN_ARRAY_MIN_CAPACITY) {
        capacity = DYN_ARRAY_MIN_CAPACITY;
    }
    array->data = malloc(capacity * sizeof(DYN_ARRAY_TYPE));
    if (!array->data) return DYN_ARRAY_ERROR_NO_MEMORY;

    array->size = 0;
    array->capacity = capacity;

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    if (pthread_mutex_init(&array->lock, NULL) != 0) {
        free(array->data);
        return DYN_ARRAY_ERROR_NO_MEMORY;
    }
#endif

    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_init_with_value(dynamic_array_t *array, size_t capacity, DYN_ARRAY_TYPE default_value) {
    dyn_array_status_t status = dynamic_array_init(array, capacity);
    if (status != DYN_ARRAY_OK) return status;

    for (size_t i = 0; i < capacity; i++) {
        array->data[i] = default_value;
    }
    array->size = capacity;

    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_free(dynamic_array_t *array) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(array->data, DYN_ARRAY_ERROR_NULL_POINTER);

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    pthread_mutex_destroy(&array->lock);
#endif

    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_destroy(dynamic_array_t *array) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    dyn_array_status_t status = dynamic_array_free(array);
    if (status != DYN_ARRAY_OK) return status;
    free(array);

    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_clear(dynamic_array_t *array) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(array->data, DYN_ARRAY_ERROR_NULL_POINTER);

    array->size = 0;
    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_push_back(dynamic_array_t *array, DYN_ARRAY_TYPE value) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(array->data, DYN_ARRAY_ERROR_NULL_POINTER);


#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    DYN_ARRAY_LOCK(array->lock);
#endif

    if (array->size >= array->capacity) {
        dyn_array_status_t grow_status = dynamic_array_grow(array);
        if (grow_status != DYN_ARRAY_OK) {
#ifdef DYNAMIC_ARRAY_THREAD_SAFE
            DYN_ARRAY_UNLOCK(array->lock);
#endif
            return grow_status;
        }
    }
    array->data[array->size++] = value;

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    DYN_ARRAY_UNLOCK(array->lock);
#endif
    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_pop_back(dynamic_array_t *array, DYN_ARRAY_TYPE* removed) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(array->data, DYN_ARRAY_ERROR_NULL_POINTER);

    if (array->size == 0) return DYN_ARRAY_ERROR_OUT_OF_RANGE;

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    DYN_ARRAY_LOCK(array->lock);
#endif

    array->size--;
    if (removed) {
        *removed = array->data[array->size];
    }

    if (array->capacity > DYN_ARRAY_MIN_CAPACITY &&
        array->size < array->capacity / DYN_ARRAY_SHRINK_THRESHOLD_DIV) {
        dynamic_array_realloc(array, array->capacity / DYN_ARRAY_GROW_FACTOR);
    }

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    DYN_ARRAY_UNLOCK(array->lock);
#endif
    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_shrink_to_fit(dynamic_array_t *array) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(array->data, DYN_ARRAY_ERROR_NULL_POINTER);

    if (array->size == array->capacity) return DYN_ARRAY_OK;
    return dynamic_array_realloc(array, array->size);
}

dyn_array_status_t dynamic_array_reserve(dynamic_array_t *array, size_t new_capacity) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(array->data, DYN_ARRAY_ERROR_NULL_POINTER);

    if (new_capacity <= array->capacity) return DYN_ARRAY_OK;
    if (new_capacity < array->size) return DYN_ARRAY_ERROR_OUT_OF_RANGE;

    return dynamic_array_realloc(array, new_capacity);
}

dyn_array_status_t dynamic_array_copy(const dynamic_array_t *src, dynamic_array_t *dst) {
    CHECK_PTR_RET(src, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(src->data, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(dst, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(dst->data, DYN_ARRAY_ERROR_NULL_POINTER);

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    DYN_ARRAY_LOCK(src->lock);
    DYN_ARRAY_LOCK(dst->lock);
#endif

    if (dst->capacity < src->size) {
        DYN_ARRAY_TYPE *tmp = realloc(dst->data, src->capacity * sizeof(DYN_ARRAY_TYPE));
        if (!tmp) {
#ifdef DYNAMIC_ARRAY_THREAD_SAFE
            DYN_ARRAY_UNLOCK(src->lock);
            DYN_ARRAY_UNLOCK(dst->lock);
#endif
            return DYN_ARRAY_ERROR_NO_MEMORY;
        }
        dst->data = tmp;
        dst->capacity = src->capacity;
    }
    memcpy_s(dst->data, src->data, src->size * sizeof(DYN_ARRAY_TYPE));
    dst->size = src->size;

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    DYN_ARRAY_UNLOCK(src->lock);
    DYN_ARRAY_UNLOCK(dst->lock);
#endif
    return DYN_ARRAY_OK;
}


/* Iterator implementation */

dyn_array_status_t dynamic_array_iterator_init(const dynamic_array_t *array, dynamic_array_iterator_t *iterator) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(iterator, DYN_ARRAY_ERROR_NULL_POINTER);
    iterator->array = array;
    iterator->current = 0;
    return DYN_ARRAY_OK;
}

dyn_array_status_t dynamic_array_iterator_next(dynamic_array_iterator_t *iterator, DYN_ARRAY_TYPE *out_value, bool *out_has_next) {
    CHECK_PTR_RET(iterator, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(out_value, DYN_ARRAY_ERROR_NULL_POINTER);
    CHECK_PTR_RET(out_has_next, DYN_ARRAY_ERROR_NULL_POINTER);

    if (iterator->current < iterator->array->size) {
        *out_value = iterator->array->data[iterator->current++];
        *out_has_next = true;
    } else {
        *out_has_next = false;
    }
    return DYN_ARRAY_OK;
}

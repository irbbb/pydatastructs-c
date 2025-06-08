#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type of elements stored in the dynamic array.
 * Change this typedef to customize the stored type.
 */
#ifndef DYN_ARRAY_TYPE
#define DYN_ARRAY_TYPE int32_t
#endif

/**
 * @brief Status codes returned by dynamic array functions.
 */
typedef enum {
    DYN_ARRAY_OK = 0,                 /**< Operation successful */
    DYN_ARRAY_ERROR_NULL_POINTER,     /**< A pointer argument was NULL */
    DYN_ARRAY_ERROR_OUT_OF_RANGE,     /**< Index out of valid range */
    DYN_ARRAY_ERROR_NO_MEMORY,        /**< Memory allocation failed */
    DYN_ARRAY_ERROR_MUTEX,            /**< Mutex error */
} dyn_array_status_t;

/**
 * @brief Macro to check if a pointer is NULL and return error if so.
 * This macro also logs the failure when DEBUG is enabled.
 *
 * @param ptr Pointer to check.
 * @param ret Return value if ptr is NULL.
 */
#ifndef CHECK_PTR_RET
#ifdef DEBUG
#include <stdio.h>
#define CHECK_PTR_RET(ptr, ret) \
    do { \
        if (__builtin_expect(!(ptr), 0)) { \
            fprintf(stderr, "[%s:%d] ERROR: NULL pointer detected: %s\n", __FILE__, __LINE__, #ptr); \
            return (ret); \
        } \
    } while (0)
#else
#define CHECK_PTR_RET(ptr, ret) do { if (__builtin_expect(!(ptr), 0)) return (ret); } while (0)
#endif
#endif

#ifdef DYNAMIC_ARRAY_THREAD_SAFE
#define DYN_ARRAY_LOCK(mutex) do { \
    int _lock_err = pthread_mutex_lock(&(mutex)); \
    if (_lock_err != 0) { \
        errno = _lock_err; \
        return DYN_ARRAY_ERROR_MUTEX; \
    } \
} while (0)

#define DYN_ARRAY_UNLOCK(mutex) do { \
    int _unlock_err = pthread_mutex_unlock(&(mutex)); \
    if (_unlock_err != 0) { \
        errno = _unlock_err; \
        return DYN_ARRAY_ERROR_MUTEX; \
    } \
} while (0)
#else
#define DYN_ARRAY_LOCK(mutex)
#define DYN_ARRAY_UNLOCK(mutex)
#endif

/**
 * @brief Dynamic array structure.
 */
typedef struct {
    DYN_ARRAY_TYPE *restrict data;
    size_t size;
    size_t capacity;
#ifdef DYNAMIC_ARRAY_THREAD_SAFE
    pthread_mutex_t lock;
#endif
} dynamic_array_t;

/* Constructors and destructors */
/**
 * @brief Creates a new dynamic array with a specified initial capacity.
 *
 * This function allocates memory for a dynamic array and initializes it with
 * the given capacity. If allocation or initialization fails, it returns NULL.
 *
 * @param capacity Initial capacity of the dynamic array.
 * @return dynamic_array_t* Pointer to the created dynamic array, or NULL on failure.
 */
dynamic_array_t* dynamic_array_create(size_t capacity);

/**
 * @brief Initialize a dynamic array with given capacity.
 * @param array Pointer to dynamic array to initialize.
 * @param capacity Initial capacity. If 0, uses default minimum.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_init(dynamic_array_t* array, size_t capacity);

/**
 * @brief Initialize a dynamic array and fill it with a default value.
 * @param array Pointer to dynamic array to initialize.
 * @param capacity Initial capacity.
 * @param default_value Value to fill the array with.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_init_with_value(dynamic_array_t* array, size_t capacity, DYN_ARRAY_TYPE default_value);

/**
 * @brief Free all resources used by the dynamic array internals.
 * Does NOT free the dynamic_array struct itself.
 * After calling this, array must not be used without reinitialization.
 * @param array Pointer to dynamic array to free internals.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_free(dynamic_array_t* array);

/**
 * @brief Destroy a dynamic array allocated with dynamic_array_create.
 * Calls dynamic_array_free and then frees the struct itself.
 * @param array Pointer to dynamic array to destroy.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_destroy(dynamic_array_t* array);

/* Core operations */
/**
 * @brief Clear the array content but keep allocated memory.
 * @param array Pointer to dynamic array.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_clear(dynamic_array_t* array);

/**
 * @brief Append an element to the end of the array, growing if needed.
 * @param array Pointer to dynamic array.
 * @param value Element to add.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_push_back(dynamic_array_t* array, DYN_ARRAY_TYPE value);

/**
 * @brief Remove the last element from the array.
 * Shrinks capacity if necessary.
 * @param array Pointer to dynamic array.
 * @param removed Removed element value.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_pop_back(dynamic_array_t* array, DYN_ARRAY_TYPE* removed);

/**
 * @brief Shrink allocated memory to fit exactly the size.
 * @param array Pointer to dynamic array.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_shrink_to_fit(dynamic_array_t* array);

/**
 * @brief Increase the capacity to at least new_capacity.
 * @param array Pointer to dynamic array.
 * @param new_capacity New capacity, must be >= current size.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_reserve(dynamic_array_t* array, size_t new_capacity);

/**
 * @brief Copy contents from src to dst, reinitializing dst.
 * @param src Pointer to source dynamic array.
 * @param dst Pointer to destination dynamic array.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_copy(const dynamic_array_t* src, dynamic_array_t* dst);

/* Accessors */
/**
 * @brief Get element at index safely.
 * @param array Pointer to dynamic array.
 * @param index Index of element to retrieve.
 * @param out_value Pointer to store retrieved value.
 * @return Status code.
 */
static inline dyn_array_status_t dynamic_array_get(
    const dynamic_array_t* array,
    size_t index,
    DYN_ARRAY_TYPE* out_value
) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    if (__builtin_expect(index >= array->size, 0)) return DYN_ARRAY_ERROR_OUT_OF_RANGE;
    *out_value = array->data[index];
    return DYN_ARRAY_OK;
}

/**
 * @brief Set element at index safely.
 * @param array Pointer to dynamic array.
 * @param index Index of element to set.
 * @param value New value.
 * @return Status code.
 */
static inline __attribute__((always_inline)) dyn_array_status_t dynamic_array_set(
    dynamic_array_t *restrict array,
    size_t index,
    DYN_ARRAY_TYPE value
) {
    CHECK_PTR_RET(array, DYN_ARRAY_ERROR_NULL_POINTER);
    if (__builtin_expect(index >= array->size, 0)) return DYN_ARRAY_ERROR_OUT_OF_RANGE;
    array->data[index] = value;
    return DYN_ARRAY_OK;
}

/**
 * @brief Get current size of the array.
 * @param array Pointer to dynamic array.
 * @return Number of elements or 0 if array is NULL.
 */
static inline size_t dynamic_array_size(const dynamic_array_t* array) {
    if (!array) return 0;
    return array->size;
}

/**
 * @brief Check if the dynamic array is empty.
 * @param array Pointer to dynamic array.
 * @return true if empty or NULL, false otherwise.
 */
static inline bool dynamic_array_is_empty(const dynamic_array_t *array) {
    if (!array) return true;
    return (array->size == 0);
}

/* Iterator */
/**
 * @brief Iterator to traverse the dynamic array.
 */
typedef struct {
    const dynamic_array_t *array; /**< Pointer to dynamic array */
    size_t current;             /**< Current index */
} dynamic_array_iterator_t;

/**
 * @brief Initialize an iterator for the given array.
 * @param array Pointer to dynamic array.
 * @param iterator Pointer to iterator to initialize.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_iterator_init(const dynamic_array_t* array, dynamic_array_iterator_t* iterator);

/**
 * @brief Get next element from iterator.
 * @param iterator Pointer to iterator.
 * @param out_value Pointer to store next element.
 * @param out_has_next Pointer to store whether there is a next element.
 * @return Status code.
 */
dyn_array_status_t dynamic_array_iterator_next(dynamic_array_iterator_t* iterator, DYN_ARRAY_TYPE* out_value, bool* out_has_next);

#ifdef __cplusplus
}
#endif

#endif /* DYNAMIC_ARRAY_H */

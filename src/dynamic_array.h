// src/dynamic_array.h

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque type for dynamic array of int32_t.
 */
typedef struct dynamic_array_s dynamic_array_t;

/**
 * @brief Creates a new dynamic array.
 * @return Pointer to the dynamic array or NULL if allocation fails.
 */
dynamic_array_t* dynamic_array_create(void);

/**
 * @brief Destroys the dynamic array and frees all memory.
 * @param array Pointer to the dynamic array to destroy.
 */
void dynamic_array_destroy(dynamic_array_t* array);

/**
 * @brief Returns the number of elements stored.
 * @param array Pointer to the dynamic array.
 * @return Number of elements, or 0 if array is NULL.
 */
int32_t dynamic_array_size(const dynamic_array_t* array);

/**
 * @brief Checks if the array is empty.
 * @param array Pointer to the dynamic array.
 * @return true if empty or NULL, false otherwise.
 */
bool dynamic_array_is_empty(const dynamic_array_t* array);

/**
 * @brief Appends a value to the end of the array.
 * @param array Pointer to the dynamic array.
 * @param value The int32_t value to append.
 * @return true if successful, false if allocation fails or array is NULL.
 */
bool dynamic_array_push_back(dynamic_array_t* array, int32_t value);

/**
 * @brief Removes the last element and optionally returns it.
 * @param array Pointer to the dynamic array.
 * @param out_value Pointer to int32_t to receive removed value, can be NULL.
 * @return true if successful, false if array is empty or NULL.
 */
bool dynamic_array_pop_back(dynamic_array_t* array, int32_t* out_value);

/**
 * @brief Retrieves the last element without removing it.
 * @param array Pointer to the dynamic array.
 * @param out_value Pointer to int32_t to store last value.
 * @return true if successful, false if array is empty or NULL.
 */
bool dynamic_array_back(const dynamic_array_t* array, int32_t* out_value);

/**
 * @brief Removes the last element without returning it.
 * @param array Pointer to the dynamic array.
 * @return true if successful, false if array is empty or NULL.
 */
bool dynamic_array_remove_last(dynamic_array_t* array);

#ifdef __cplusplus
}
#endif

#endif /* DYNAMIC_ARRAY_H */

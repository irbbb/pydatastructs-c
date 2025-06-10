// monotonic_increasing_stack_da.c

#include "monotonic_increasing_stack.h"
#include <stdlib.h>
#include "dynamic_array.h"

/**
 * @brief Monotonic increasing stack structure using a dynamic array.
 */
struct monotonic_increasing_stack_s {
    dynamic_array_t* arr;  /**< Dynamic array holding stack elements */
};

mis_status_t monotonic_increasing_stack_create(monotonic_increasing_stack_t** out_stack) {
    if (out_stack == NULL) return MIS_ERROR_NULL_PTR;

    monotonic_increasing_stack_t* stack = (monotonic_increasing_stack_t*)calloc(1, sizeof(monotonic_increasing_stack_t));
    if (!stack) return MIS_ERROR_MEMORY;

    stack->arr = dynamic_array_create(0); // Initial capacity 0 = default internal policy
    if (stack->arr == NULL) {
        free(stack);
        return MIS_ERROR_MEMORY;
    }

    *out_stack = stack;
    return MIS_SUCCESS;
}

void monotonic_increasing_stack_destroy(monotonic_increasing_stack_t** stack_ptr) {
    if (stack_ptr == NULL || *stack_ptr == NULL) return;

    monotonic_increasing_stack_t* stack = *stack_ptr;

    if (stack->arr != NULL) {
        dyn_array_status_t status = dynamic_array_destroy(stack->arr);
        if (status != DYN_ARRAY_OK) return;
        stack->arr = NULL;
    }

    free(stack);
    *stack_ptr = NULL;
}

mis_status_t monotonic_increasing_stack_push(monotonic_increasing_stack_t* stack, int32_t value) {
    if (stack == NULL || stack->arr == NULL) return MIS_ERROR_NULL_PTR;

    int32_t top_val;
    while (!dynamic_array_is_empty(stack->arr)) {
        size_t array_size = dynamic_array_size(stack->arr);
        if (__builtin_expect(array_size == 0, 0)) return MIS_ERROR_EMPTY;

        dyn_array_status_t get_status = dynamic_array_get(stack->arr, array_size - 1, &top_val);
        if (get_status != DYN_ARRAY_OK) return MIS_ERROR_UNKNOWN;

        if (top_val <= value) break;

        dyn_array_status_t pop_status = dynamic_array_pop_back(stack->arr, NULL);
        if (pop_status != DYN_ARRAY_OK) return MIS_ERROR_UNKNOWN;
    }

    dyn_array_status_t push_status = dynamic_array_push_back(stack->arr, value);
    if (push_status != DYN_ARRAY_OK) return MIS_ERROR_MEMORY;

    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_pop(monotonic_increasing_stack_t* stack, int32_t* out_removed) {
    if (stack == NULL || stack->arr == NULL || out_removed == NULL) return MIS_ERROR_NULL_PTR;

    dyn_array_status_t pop_status = dynamic_array_pop_back(stack->arr, out_removed);
    if (pop_status == DYN_ARRAY_ERROR_OUT_OF_RANGE) return MIS_ERROR_EMPTY;
    if (pop_status != DYN_ARRAY_OK) return MIS_ERROR_MEMORY;

    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_top(const monotonic_increasing_stack_t* stack, int32_t* out_top) {
    if (stack == NULL || stack->arr == NULL || out_top == NULL) return MIS_ERROR_NULL_PTR;

    size_t array_size = dynamic_array_size(stack->arr);
    if (array_size == 0) return MIS_ERROR_EMPTY;

    dyn_array_status_t get_status = dynamic_array_get(stack->arr, array_size - 1, out_top);
    if (get_status != DYN_ARRAY_OK) return MIS_ERROR_MEMORY;

    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_is_empty(const monotonic_increasing_stack_t* stack, bool* out_is_empty) {
    if (stack == NULL || stack->arr == NULL || out_is_empty == NULL) return MIS_ERROR_NULL_PTR;

    *out_is_empty = dynamic_array_is_empty(stack->arr);
    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_size(const monotonic_increasing_stack_t* stack, uint32_t* out_size) {
    if (stack == NULL || stack->arr == NULL || out_size == NULL) return MIS_ERROR_NULL_PTR;

    *out_size = (uint32_t)dynamic_array_size(stack->arr);
    return MIS_SUCCESS;
}

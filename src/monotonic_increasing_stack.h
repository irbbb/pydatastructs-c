#ifndef MONOTONIC_INCREASING_STACK_H
#define MONOTONIC_INCREASING_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Result codes for monotonic_increasing_stack operations */
typedef enum {
    MIS_SUCCESS = 0,
    MIS_ERROR_NULL_PTR,
    MIS_ERROR_MEMORY,
    MIS_ERROR_EMPTY,
    MIS_ERROR_UNKNOWN
} mis_status_t;

/* Opaque type for monotonic increasing stack */
typedef struct monotonic_increasing_stack_s monotonic_increasing_stack_t;

/**
 * Creates a new empty monotonic increasing stack.
 * @return Pointer to the stack or NULL if allocation fails.
 */
mis_status_t monotonic_increasing_stack_create(monotonic_increasing_stack_t** out_stack);

/**
 * Destroys the stack and frees memory.
 * @param stack Pointer to pointer to stack; after call *stack = NULL.
 */
void monotonic_increasing_stack_destroy(monotonic_increasing_stack_t** stack);

/**
 * Pushes a value onto the stack maintaining monotonic increasing order.
 * Removes elements from the top that are greater than the new value.
 * @param stack Pointer to stack.
 * @param value Integer to push.
 * @return MIS_SUCCESS or error code.
 */
mis_status_t monotonic_increasing_stack_push(monotonic_increasing_stack_t* stack, int32_t value);

/**
 * Pops the top element.
 * @param stack Pointer to stack.
 * @param out_removed Pointer to store popped value.
 * @return MIS_SUCCESS or error code.
 */
mis_status_t monotonic_increasing_stack_pop(monotonic_increasing_stack_t* stack, int32_t* out_removed);

/**
 * Retrieves top element without removing.
 * @param stack Pointer to stack.
 * @param out_top Pointer to store top value.
 * @return MIS_SUCCESS or error code.
 */
mis_status_t monotonic_increasing_stack_top(const monotonic_increasing_stack_t* stack, int32_t* out_top);

/**
 * Checks if stack is empty.
 * @param stack Pointer to stack.
 * @param out_is_empty Pointer to store result.
 * @return MIS_SUCCESS or error code.
 */
mis_status_t monotonic_increasing_stack_is_empty(const monotonic_increasing_stack_t* stack, bool* out_is_empty);

/**
 * Returns the number of elements.
 * @param stack Pointer to stack.
 * @param out_size Pointer to store size.
 * @return MIS_SUCCESS or error code.
 */
mis_status_t monotonic_increasing_stack_size(const monotonic_increasing_stack_t* stack, int32_t* out_size);

#ifdef __cplusplus
}
#endif

#endif /* MONOTONIC_INCREASING_STACK_H */

// src/monotonic_increasing_stack_ll.c

#include "monotonic_increasing_stack.h"
#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>

struct monotonic_increasing_stack_ll_s {
    LinkedList* ll;
};

mis_status_t monotonic_increasing_stack_create(monotonic_increasing_stack_t** out_stack) {
    if (out_stack == NULL) return MIS_ERROR_NULL_PTR;

    monotonic_increasing_stack_t* stack = (monotonic_increasing_stack_t*)calloc(1, sizeof(monotonic_increasing_stack_t));
    if (stack == NULL) return MIS_ERROR_MEMORY;

    stack->ll = linked_list_create();
    if (stack->ll == NULL) {
        free(stack);
        return MIS_ERROR_MEMORY;
    }

    *out_stack = stack;
    return MIS_SUCCESS;
}

void monotonic_increasing_stack_destroy(monotonic_increasing_stack_t** stack_ptr) {
    if (stack_ptr == NULL || *stack_ptr == NULL) return;

    monotonic_increasing_stack_t* stack = *stack_ptr;

    if (stack->ll != NULL) {
        linked_list_free(stack->ll);
        stack->ll = NULL;
    }

    free(stack);
    *stack_ptr = NULL;
}

mis_status_t monotonic_increasing_stack_push(monotonic_increasing_stack_t* stack, int32_t value) {
    if (stack == NULL || stack->ll == NULL) return MIS_ERROR_NULL_PTR;

    // Pop while top element is greater than value
    while (true) {
        int32_t top_val = 0;
        bool peek_success = linked_list_peek(stack->ll, &top_val);

        if (!peek_success || top_val <= value) {
            break;
        }

        bool pop_success = linked_list_pop(stack->ll, &top_val);
        if (!pop_success) {
            // Unexpected failure
            return MIS_ERROR_UNKNOWN;
        }
    }

    bool prepend_success = linked_list_prepend(stack->ll, value);
    if (!prepend_success) {
        return MIS_ERROR_MEMORY;
    }

    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_pop(monotonic_increasing_stack_t* stack, int32_t* out_removed) {
    if (stack == NULL || stack->ll == NULL) return MIS_ERROR_NULL_PTR;
    if (out_removed == NULL) return MIS_ERROR_NULL_PTR;

    bool pop_success = linked_list_pop(stack->ll, out_removed);
    if (!pop_success) {
        return MIS_ERROR_EMPTY;  // Stack empty
    }

    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_top(const monotonic_increasing_stack_t* stack, int32_t* out_top) {
    if (stack == NULL || stack->ll == NULL) return MIS_ERROR_NULL_PTR;
    if (out_top == NULL) return MIS_ERROR_NULL_PTR;

    bool peek_success = linked_list_peek(stack->ll, out_top);
    if (!peek_success) {
        return MIS_ERROR_EMPTY;  // Stack empty
    }

    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_is_empty(const monotonic_increasing_stack_t* stack, bool* out_is_empty) {
    if (stack == NULL || stack->ll == NULL) return MIS_ERROR_NULL_PTR;
    if (out_is_empty == NULL) return MIS_ERROR_NULL_PTR;

    *out_is_empty = (linked_list_length(stack->ll) == 0);
    return MIS_SUCCESS;
}

mis_status_t monotonic_increasing_stack_size(const monotonic_increasing_stack_t* stack, int32_t* out_size) {
    if (stack == NULL || stack->ll == NULL) return MIS_ERROR_NULL_PTR;
    if (out_size == NULL) return MIS_ERROR_NULL_PTR;

    *out_size = linked_list_length(stack->ll);
    return MIS_SUCCESS;
}
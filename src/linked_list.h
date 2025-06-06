// src/linked_list.h

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>  // For 'bool' return types

/**
 * @brief Node in the singly linked list.
 */
typedef struct Node {
    struct Node* next; ///< Pointer to the next node
    int value;         ///< Integer value stored in the node
} Node;

/**
 * @brief A singly linked list of integers.
 */
typedef struct {
    Node* head; ///< Pointer to the first node
    int size;   ///< Number of elements in the list
} LinkedList;

/**
 * @brief Creates a new empty linked list.
 * @return Pointer to the new list, or NULL if allocation fails.
 */
LinkedList* linked_list_create(void);

/**
 * @brief Frees all memory associated with the linked list.
 * @param ll Pointer to the list to free. Safe to pass NULL.
 */
void linked_list_free(LinkedList* ll);

/**
 * @brief Inserts a value at the beginning of the list.
 * @param ll Pointer to the list.
 * @param value Integer to insert.
 * @return true if successful, false if allocation failed or ll is NULL.
 */
bool linked_list_prepend(LinkedList* ll, int value);

/**
 * @brief Removes the first value from the list.
 * @param ll Pointer to the list.
 * @param removed Pointer to store the removed value.
 * @return true if an element was removed, false otherwise.
 */
bool linked_list_pop(LinkedList* ll, int* removed);

/**
 * @brief Returns the number of elements in the list.
 * @param ll Pointer to the list.
 * @return The number of elements, or 0 if ll is NULL.
 */
int linked_list_length(const LinkedList* ll);

#ifdef __cplusplus
}
#endif

#endif // LINKED_LIST_H
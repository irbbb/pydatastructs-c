#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "linked_list.h"

LinkedList* linked_list_create(void) {
    LinkedList* ll = calloc(1, sizeof(LinkedList));
    return ll;  // calloc zeroes the fields
}

void linked_list_free(LinkedList* ll) {
    if (!ll) return;

    Node* current = ll->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(ll);
}

bool linked_list_prepend(LinkedList* ll, int value) {
    if (!ll) return false;

    Node* new_node = malloc(sizeof(Node));
    if (!new_node) return false;

    new_node->value = value;
    new_node->next = ll->head;
    ll->head = new_node;
    ll->size++;
    return true;
}

bool linked_list_pop(LinkedList* ll, int* removed) {
    if (!ll || !ll->head || !removed) return false;

    Node* first = ll->head;
    *removed = first->value;
    ll->head = first->next;
    free(first);
    ll->size--;
    return true;
}

bool linked_list_peek(const LinkedList* ll, int* peeked) {
    if (!ll || !ll->head || !peeked) return false;

    *peeked = ll->head->value;
    return true;
}

int linked_list_length(const LinkedList* ll) {
    return ll ? ll->size : 0;
}

// src/dllist.h

#ifndef DLLIST_H
#define DLLIST_H

typedef struct Node {
    int value;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} DoublyLinkedList;

DoublyLinkedList* dll_create();
void dll_free(DoublyLinkedList* dll);
void dll_append(DoublyLinkedList* list, int value);
void dll_prepend(DoublyLinkedList* list, int value);
int* dll_remove(DoublyLinkedList* list, int value);
int dll_length(DoublyLinkedList* list);
int* dll_to_array(DoublyLinkedList* list);

#endif

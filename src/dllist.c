// src/dllist.c

#include <stdlib.h>
#include "dllist.h"

DoublyLinkedList* dll_create() {
    DoublyLinkedList* dll = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (!dll) return NULL;
    dll->head = NULL;
    dll->tail = NULL;
    dll->size = 0;
    return dll;
}

void dll_free(DoublyLinkedList* dll) {
    if (!dll) return;
    Node* current = dll->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(dll);
}

void dll_append(DoublyLinkedList* list, int value) {
    if (!list) return;
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return;
    node->value = value;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;

    list->tail = node;
    list->size++;
}

void dll_prepend(DoublyLinkedList* list, int value) {
    if (!list) return;
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return;
    node->value = value;
    node->prev = NULL;
    node->next = list->head;

    if (list->head)
        list->head->prev = node;
    else
        list->tail = node;

    list->head = node;
    list->size++;
}

int* dll_remove(DoublyLinkedList* list, int value) {
    if (!list) return;
    Node* current = list->head;

    while (current) {
        if (current->value == value) {
            if (current->prev)
                current->prev->next = current->next;
            else
                list->head = current->next;

            if (current->next)
                current->next->prev = current->prev;
            else
                list->tail = current->prev;

            free(current);
            list->size--;
            return &value;
        }
        current = current->next;
    }
}

int dll_length(DoublyLinkedList* list) {
    if (!list) return 0;
    return list->size;
}

int* dll_to_array(DoublyLinkedList* list) {
    if (!list) return NULL;
    if (list->size == 0) return NULL;

    int* arr = (int*)malloc(sizeof(int) * list->size);
    if (!arr) return NULL;

    Node* current = list->head;
    int i = 0;
    while (current) {
        arr[i++] = current->value;
        current = current->next;
    }
    return arr;
}
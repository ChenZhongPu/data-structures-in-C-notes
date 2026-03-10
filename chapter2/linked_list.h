#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef int ElemType;

struct Node;

typedef struct Node *Link;
typedef struct Node *List;

typedef enum ListOpStatus {
  LIST_OK,
  LIST_ERROR,
  LIST_INDEX_OUT_OF_BOUNDS,
  LIST_ELEMENT_NOT_FOUND
} ListOpStatus;

List list_init(void);

void list_destroy(List lst);

void list_clear(List lst); // keep the header node, remove all other nodes

void list_display(const List lst);

void list_add_first(List lst, ElemType e);

void list_add_last(List lst, ElemType e);

void list_from_array_first(List lst, const ElemType arr[], size_t n);

void list_remove_first(List lst);

bool list_is_empty(const List list);

size_t list_length(const List list);

ListOpStatus list_get(const List list, size_t i, ElemType *result);

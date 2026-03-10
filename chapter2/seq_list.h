#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef int ElemType;

struct SqList;

typedef struct SqList *List;

List list_init(void);

void list_destroy(List L);

bool list_is_empty(const List L);

size_t list_length(const List L);

void list_display(const List L);

ElemType list_get(const List L, size_t i);

int list_locate(const List L, ElemType e);

bool list_insert(List L, size_t i, ElemType e);

bool list_delete(List L, size_t i);

void list_delete_elem(List L, ElemType e);

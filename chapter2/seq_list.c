#include "seq_list.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

struct SqList {
  ElemType data[MAX_SIZE];
  size_t length;
};

List list_init(void) {
  List L = (List)malloc(sizeof(struct SqList));
  L->length = 0;
  return L;
}

void list_destroy(List L) { free(L); }

bool list_is_empty(const List L) { return L == NULL || L->length == 0; }

size_t list_length(const List L) { return L == NULL ? 0 : L->length; }

void list_display(const List L) {
  if (L == NULL) {
    return;
  }
  for (size_t i = 0; i < L->length; ++i) {
    printf("%d ", L->data[i]);
  }
  printf("\n");
}

ElemType list_get(const List L, size_t i) { return L->data[i]; }

int list_locate(const List L, ElemType e) {
  if (L == NULL) {
    return -1;
  }
  for (size_t i = 0; i < L->length; ++i) {
    if (L->data[i] == e) {
      return (int)i;
    }
  }
  return -1;
}

bool list_insert(List L, size_t i, ElemType e) {
  if (L == NULL || i > L->length || L->length >= MAX_SIZE) {
    return false;
  }
  for (size_t j = L->length; j > i; --j) {
    L->data[j] = L->data[j - 1];
  }
  L->data[i] = e;
  ++L->length;
  return true;
}

bool list_delete(List L, size_t i) {
  if (L == NULL || i >= L->length) {
    return false;
  }
  for (size_t j = i; j < L->length - 1; ++j) {
    L->data[j] = L->data[j + 1];
  }
  --L->length;
  return true;
}

void list_delete_elem(List L, ElemType e) {
  if (L == NULL) {
    return;
  }
  int k = 0;
  for (size_t i = 0; i < L->length; ++i) {
    if (L->data[i] != e) {
      L->data[k++] = L->data[i];
    }
  }
  L->length = k;
}

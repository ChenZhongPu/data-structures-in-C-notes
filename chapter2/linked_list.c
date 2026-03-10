#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

struct Node {
  ElemType data;
  Link next;
};

List list_init(void) {
  // it returns a pointer to header (a dummy node)
  List L = (List)malloc(sizeof(struct Node));
  L->next = NULL;
  return L;
}

void list_destroy(List lst) {
  Link current = lst;
  while (current != NULL) {
    Link next = current->next;
    free(current);
    current = next;
  }
}

void list_clear(List lst) {
  Link current = lst->next; // skip the header node
  while (current != NULL) {
    Link next = current->next;
    free(current);
    current = next;
  }
  lst->next = NULL; // reset the header node's next pointer
}

void list_display(const List lst) {
  Link current = lst->next; // skip the header node
  while (current != NULL) {
    printf("%d ", current->data);
    current = current->next;
  }
  printf("\n");
}

void list_add_first(List lst, ElemType e) {
  Link new_node = (Link)malloc(sizeof(struct Node));
  new_node->data = e;
  new_node->next = lst->next;
  lst->next = new_node;
}

void list_add_last(List lst, ElemType e) {
  Link new_node = (Link)malloc(sizeof(struct Node));
  new_node->data = e;
  new_node->next = NULL;

  Link current = lst; // start from the header node
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_node;
}

void list_from_array_first(List lst, const ElemType arr[], size_t n) {
  for (size_t i = 0; i < n; i++) {
    list_add_first(lst, arr[i]);
  }
}

void list_remove_first(List lst) {
  if (lst->next != NULL) {
    Link first_node = lst->next;
    lst->next = first_node->next;
    free(first_node);
  }
}

bool list_is_empty(const List lst) { return lst->next == NULL; }

size_t list_length(const List lst) {
  size_t length = 0;
  Link current = lst->next; // skip the header node
  while (current != NULL) {
    length++;
    current = current->next;
  }
  return length;
}

ListOpStatus list_get(const List lst, size_t i, ElemType *result) {
  Link current = lst->next; // skip the header node
  size_t index = 0;
  while (current != NULL) {
    if (index == i) {
      *result = current->data;
      return LIST_OK;
    }
    index++;
    current = current->next;
  }
  return LIST_INDEX_OUT_OF_BOUNDS;
}

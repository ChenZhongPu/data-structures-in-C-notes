#include "linked_list.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  List list = list_init();

  assert(list_is_empty(list));
  assert(list_length(list) == 0);

  ElemType arr[] = {10, 20, 30, 40};
  list_from_array_first(list, arr, 4);
  // Head insertion builds: 40 -> 30 -> 20 -> 10
  assert(!list_is_empty(list));
  assert(list_length(list) == 4);

  ElemType value = 0;
  assert(list_get(list, 0, &value) == LIST_OK && value == 40);
  assert(list_get(list, 1, &value) == LIST_OK && value == 30);
  assert(list_get(list, 3, &value) == LIST_OK && value == 10);
  assert(list_get(list, 4, &value) == LIST_INDEX_OUT_OF_BOUNDS);

  list_remove_first(list); // remove 40
  assert(list_length(list) == 3);
  assert(list_get(list, 0, &value) == LIST_OK && value == 30);

  list_clear(list);
  assert(list_is_empty(list));
  assert(list_length(list) == 0);
  assert(list_get(list, 0, &value) == LIST_INDEX_OUT_OF_BOUNDS);

  list_add_first(list, 99);
  assert(list_length(list) == 1);
  assert(list_get(list, 0, &value) == LIST_OK && value == 99);

  list_add_last(list, 88);
  assert(list_length(list) == 2);
  assert(list_get(list, 1, &value) == LIST_OK && value == 88);

  printf("All linked list tests passed.\n");
  list_destroy(list);

  return 0;
}
